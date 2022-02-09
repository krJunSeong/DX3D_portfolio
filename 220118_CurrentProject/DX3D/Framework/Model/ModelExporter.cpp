#include "Framework.h"

ModelExporter::ModelExporter(string name, string file)
    : name(name)
{
    importer = new Assimp::Importer();

    scene = importer->ReadFile(file,
        aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);
    assert(scene != nullptr);
}

ModelExporter::~ModelExporter()
{
    delete importer;
}

void ModelExporter::ExportMaterial()
{
    ReadMaterial();
    WriteMaterial();
}

void ModelExporter::ExportMesh()
{
    ReadNode(scene->mRootNode, -1, -1);
    ReadMesh(scene->mRootNode);
    WriteMesh();
}

void ModelExporter::ExportClip(string clipName)
{
    for (UINT i = 0; i < scene->mNumAnimations; i++)
    {
        Clip* clip = ReadClip(scene->mAnimations[i]);
        WriteClip(clip, clipName, i);
    }
}

void ModelExporter::ReadMaterial()
{
    for (UINT i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* srcMaterial = scene->mMaterials[i];
        Material* material = new Material();

        material->SetName(srcMaterial->GetName().C_Str());

        aiColor3D color;
        MaterialBuffer::Data& data = material->GetData();

        srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        data.diffuse = Float4(color.r, color.g, color.b, 1.0f);

        srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
        data.specular = Float4(color.r, color.g, color.b, 1.0f);

        srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
        data.ambient = Float4(color.r, color.g, color.b, 1.0f);

        srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
        data.emissive = Float4(color.r, color.g, color.b, 1.0f);

        srcMaterial->Get(AI_MATKEY_SHININESS, data.shininess);

        aiString file;

        srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
        material->SetDiffuseMap(ToWString(CreateTexture(file.C_Str())));
        file.Clear();

        srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
        material->SetSpecularMap(ToWString(CreateTexture(file.C_Str())));
        file.Clear();

        srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
        material->SetNormalMap(ToWString(CreateTexture(file.C_Str())));
        file.Clear();

        string savePath = "TextData/Materials/" + name + "/" + material->GetName() + ".mat";

        CreateFolders(savePath);

        material->Save(savePath);

        materials.push_back(material);
    }
}

void ModelExporter::WriteMaterial()
{
    string savePath = "ModelData/Materials/" + name + ".mats";

    CreateFolders(savePath);

    BinaryWriter w(savePath);

    w.UInt(materials.size());

    for (Material* material : materials)
    {
        w.String(material->GetName());

        delete material;
    }

    materials.clear();
}

string ModelExporter::CreateTexture(string file)
{
    if (file.length() == 0)
        return "";

    string fileName = GetFileNameWidthoutExtention(file) + ".png";
    const aiTexture* texture = scene->GetEmbeddedTexture(file.c_str());

    string path = "Textures/Model/" + name + "/" + fileName;

    CreateFolders(path);

    if (ExistFile(path))
        return path;

    if (texture == nullptr)
        return "";

    if (texture->mHeight < 1)
    {
        BinaryWriter w(path);
        w.Byte(texture->pcData, texture->mWidth);
    }
    else
    {
        Image image;
        image.width = texture->mWidth;
        image.height = texture->mHeight;
        image.pixels = (uint8_t*)(texture->pcData);
        image.rowPitch = image.width * 4;
        image.slicePitch = image.width * image.height * 4;

        SaveToWICFile(image, WIC_FLAGS_NONE,
            GetWICCodec(WIC_CODEC_PNG), ToWString(path).c_str());
    }

    return path;
}

void ModelExporter::ReadMesh(aiNode* node)
{
    for (UINT i = 0; i < node->mNumMeshes; i++)
    {
        MeshData* mesh = new MeshData();
        mesh->name = node->mName.C_Str();

        UINT index = node->mMeshes[i];
        aiMesh* srcMesh = scene->mMeshes[index];

        mesh->materialIndex = srcMesh->mMaterialIndex;

        UINT startVertex = mesh->vertices.size();

        vector<VertexWeights> vertexWeights(srcMesh->mNumVertices);

        ReadBone(srcMesh, vertexWeights);

        mesh->vertices.resize(srcMesh->mNumVertices);
        for (UINT v = 0; v < srcMesh->mNumVertices; v++)
        {
            ModelVertex vertex;
            memcpy(&vertex.position, &srcMesh->mVertices[v], sizeof(Float3));

            if (srcMesh->HasTextureCoords(0))
                memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(Float2));

            if (srcMesh->HasNormals())
                memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(Float3));

            if (srcMesh->HasTangentsAndBitangents())
                memcpy(&vertex.tangent, &srcMesh->mTangents[v], sizeof(Float3));

            if (!vertexWeights.empty())
            {
                vertexWeights[v].Normalize();

                vertex.indices.x = (float)vertexWeights[v].indices[0];
                vertex.indices.y = (float)vertexWeights[v].indices[1];
                vertex.indices.z = (float)vertexWeights[v].indices[2];
                vertex.indices.w = (float)vertexWeights[v].indices[3];

                vertex.weights.x = vertexWeights[v].weights[0];
                vertex.weights.y = vertexWeights[v].weights[1];
                vertex.weights.z = vertexWeights[v].weights[2];
                vertex.weights.w = vertexWeights[v].weights[3];
            }

            mesh->vertices[v] = vertex;
        }

        mesh->indices.resize(srcMesh->mNumFaces * 3);
        for (UINT f = 0; f < srcMesh->mNumFaces; f++)
        {
            aiFace& face = srcMesh->mFaces[f];

            for (UINT k = 0; k < face.mNumIndices; k++)
            {
                mesh->indices[f * 3 + k] = face.mIndices[k] + startVertex;
            }
        }

        meshes.push_back(mesh);
    }

    for (UINT i = 0; i < node->mNumChildren; i++)
        ReadMesh(node->mChildren[i]);
}

void ModelExporter::ReadNode(aiNode* node, int index, int parent)
{
    NodeData* nodeData = new NodeData();
    nodeData->index = index;
    nodeData->parent = parent;
    nodeData->name = node->mName.C_Str();

    Matrix matrix(node->mTransformation[0]);
    nodeData->transform = XMMatrixTranspose(matrix);

    nodes.push_back(nodeData);

    for (UINT i = 0; i < node->mNumChildren; i++)
        ReadNode(node->mChildren[i], nodes.size(), index);
}

void ModelExporter::ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights)
{
    for (UINT i = 0; i < mesh->mNumBones; i++)
    {
        UINT boneIndex = 0;
        string name = mesh->mBones[i]->mName.C_Str();

        if (boneMap.count(name) == 0)
        {
            boneIndex = boneCount++;
            boneMap[name] = boneIndex;

            BoneData* boneData = new BoneData();
            boneData->name = name;
            boneData->index = boneIndex;

            Matrix matrix(mesh->mBones[i]->mOffsetMatrix[0]);
            boneData->offset = XMMatrixTranspose(matrix);

            bones.push_back(boneData);
        }
        else
        {
            boneIndex = boneMap[name];
        }

        for (UINT j = 0; j < mesh->mBones[i]->mNumWeights; j++)
        {
            UINT index = mesh->mBones[i]->mWeights[j].mVertexId;
            vertexWeights[index].Add(boneIndex, mesh->mBones[i]->mWeights[j].mWeight);
        }
    }
}

void ModelExporter::WriteMesh()
{
    string path = "ModelData/Meshes/" + name + ".mesh";

    CreateFolders(path);

    BinaryWriter w(path);

    w.UInt(meshes.size());
    for (MeshData* mesh : meshes)
    {
        w.String(mesh->name);
        w.UInt(mesh->materialIndex);

        w.UInt(mesh->vertices.size());
        w.Byte(mesh->vertices.data(), sizeof(ModelVertex) * mesh->vertices.size());

        w.UInt(mesh->indices.size());
        w.Byte(mesh->indices.data(), sizeof(UINT) * mesh->indices.size());

        delete mesh;
    }
    meshes.clear();

    w.UInt(nodes.size());
    for (NodeData* node : nodes)
    {
        w.Int(node->index);
        w.String(node->name);
        w.Int(node->parent);
        w.Matrix(node->transform);

        delete node;
    }
    nodes.clear();

    w.UInt(bones.size());
    for (BoneData* bone : bones)
    {
        w.String(bone->name);
        w.Int(bone->index);
        w.Matrix(bone->offset);

        delete bone;
    }
    bones.clear();
}

Clip* ModelExporter::ReadClip(aiAnimation* animation)
{
    Clip* clip = new Clip();
    clip->name = animation->mName.C_Str();
    clip->tickPerSecond = (float)animation->mTicksPerSecond;
    clip->frameCount = (UINT)(animation->mDuration) + 1;

    vector<ClipNode> clipNodes;
    for (UINT i = 0; i < animation->mNumChannels; i++)
    {
        aiNodeAnim* srcNode = animation->mChannels[i];

        ClipNode node;
        node.name = srcNode->mNodeName;

        UINT keyCount = max(srcNode->mNumPositionKeys, srcNode->mNumRotationKeys);
        keyCount = max(keyCount, srcNode->mNumScalingKeys);

        KeyTransform transform;
        for (UINT k = 0; k < keyCount; k++)
        {
            bool isFound = false;
            float t = node.keyFrame.size();

            if (k < srcNode->mNumPositionKeys &&
                abs((float)srcNode->mPositionKeys[k].mTime - t) <= FLT_EPSILON)//mTime == t
            {
                aiVectorKey key = srcNode->mPositionKeys[k];
                memcpy_s(&transform.position, sizeof(Float3), &key.mValue, sizeof(aiVector3D));

                isFound = true;
            }

            if (k < srcNode->mNumRotationKeys &&
                abs((float)srcNode->mRotationKeys[k].mTime - t) <= FLT_EPSILON)
            {
                aiQuatKey key = srcNode->mRotationKeys[k];

                transform.rotation.x = (float)key.mValue.x;
                transform.rotation.y = (float)key.mValue.y;
                transform.rotation.z = (float)key.mValue.z;
                transform.rotation.w = (float)key.mValue.w;

                isFound = true;
            }

            if (k < srcNode->mNumScalingKeys &&
                abs((float)srcNode->mScalingKeys[k].mTime - t) <= FLT_EPSILON)//mTime == t
            {
                aiVectorKey key = srcNode->mScalingKeys[k];
                memcpy_s(&transform.scale, sizeof(Float3), &key.mValue, sizeof(aiVector3D));

                isFound = true;
            }

            if (isFound)
                node.keyFrame.push_back(transform);
        }

        if (node.keyFrame.size() < clip->frameCount)
        {
            UINT count = clip->frameCount - node.keyFrame.size();
            KeyTransform keyTrasform = node.keyFrame.back();

            for (UINT n = 0; n < count; n++)
                node.keyFrame.push_back(keyTrasform);
        }

        clipNodes.push_back(node);
    }

    ReadKeyFrame(clip, scene->mRootNode, clipNodes);

    return clip;
}

void ModelExporter::ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& clipNodes)
{
    KeyFrame* keyFrame = new KeyFrame();
    keyFrame->boneName = node->mName.C_Str();

    for (UINT i = 0; i < clip->frameCount; i++)
    {
        ClipNode* clipNode = nullptr;
        for (ClipNode& temp : clipNodes)
        {
            if (temp.name == node->mName)
            {
                clipNode = &temp;
                break;
            }
        }

        KeyTransform keyTransform;
        if (clipNode == nullptr)
        {
            Matrix transform(node->mTransformation[0]);
            transform = XMMatrixTranspose(transform);

            Vector3 S, R, T;
            XMMatrixDecompose(&S.data, &R.data, &T.data, transform);
            keyTransform.scale = S;
            XMStoreFloat4(&keyTransform.rotation, R.data);
            keyTransform.position = T;
        }
        else
        {
            keyTransform = clipNode->keyFrame[i];
        }
        keyFrame->transforms.push_back(keyTransform);
    }
    clip->keyFrame.push_back(keyFrame);

    for (UINT i = 0; i < node->mNumChildren; i++)
        ReadKeyFrame(clip, node->mChildren[i], clipNodes);
}

void ModelExporter::WriteClip(Clip* clip, string clipName, UINT index)
{
    string file = "ModelData/Clips/" + name + "/" + clipName + to_string(index) + ".clip";

    CreateFolders(file);

    BinaryWriter w(file);
    w.String(clip->name);
    w.UInt(clip->frameCount);
    w.Float(clip->tickPerSecond);

    w.UInt(clip->keyFrame.size());
    for (KeyFrame* keyFrame : clip->keyFrame)
    {
        w.String(keyFrame->boneName);

        w.UInt(keyFrame->transforms.size());
        w.Byte(keyFrame->transforms.data(), sizeof(KeyTransform) * keyFrame->transforms.size());

        delete keyFrame;
    }

    delete clip;
}
