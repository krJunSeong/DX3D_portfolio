#include "Framework.h"

TerrainEditor::TerrainEditor(UINT width, UINT height)
	: width(width), height(height), heightMap(nullptr), adjustValue(10.0f),
	editType(1), selectMap(0)
{
	material = new Material(L"TerrainEditor.hlsl");
	material->SetDiffuseMap(L"Textures/Landscape/Dirt2.png");
	//material->SetNormalMap(L"Textures/Landscape/Stones_normal.png");

	secondMap = Texture::Add(L"Textures/Landscape/Dirt.png");
	thirdMap = Texture::Add(L"Textures/Landscape/Stones.png");
	fourthMap = Texture::Add(L"Textures/Landscape/Wall.png");

	worldBuffer = new WorldBuffer();

	CreateMesh();
	CreateComputeData();

	rasterizerState = new RasterizerState();
	rasterizerState->FillMode(D3D11_FILL_WIREFRAME);

	brushBuffer = new BrushBuffer();
}

TerrainEditor::~TerrainEditor()
{
	delete material;
	delete mesh;
	delete worldBuffer;
	delete rasterizerState;

	delete structuredBuffer;
	delete rayBuffer;
	delete[] input;
	delete[] output;

	delete brushBuffer;

	for (auto robot : robots)
		delete robot;
	
}

void TerrainEditor::Update()
{
	brushBuffer->data.pickingPos = ComputePicking();

	if (MOUSE_PRESS(0) && !ImGui::GetIO().WantCaptureMouse)
	{
		switch (editType)
		{
		case 0:
			AdjustHeight();
			break;
		case 1:
			AdjustAlpha();
			break;
		}
	}

	if (MOUSE_UP(0) && !ImGui::GetIO().WantCaptureMouse)
	{
		InitNormalTangent();
		CreateNormal();
		CreateTangent();
		mesh->UpdateVertex(vertices.data(), vertices.size());
	}

}

void TerrainEditor::Render()
{
	worldBuffer->Set(world);
	worldBuffer->SetVSBuffer(0);

	brushBuffer->SetPSBuffer(10);
	
	secondMap->Set(11);
	thirdMap->Set(12);
	fourthMap->Set(13);

	mesh->IASet();
	material->Set();

	//rasterizerState->SetState();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void TerrainEditor::GUIRender()
{
	ImGui::TextColored(ImVec4(0, 1, 1, 1), "TerrainEditor");

	const char* brushList[] = {"Cricle", "Rectangle"};
	ImGui::Combo("BrushType", &brushBuffer->data.type, brushList, 2);

	ImGui::DragFloat("BrushRange", &brushBuffer->data.range);
	ImGui::ColorEdit3("BrushColor", (float*)&brushBuffer->data.color);
	ImGui::DragFloat("AdjustValue", &adjustValue);

	const char* EditList[] = { "Height", "Alpha" };
	ImGui::Combo("EditType", &editType, EditList, 2);

	if(ImGui::ImageButton(secondMap->GetSRV(), ImVec2(50, 50)))
		selectMap = 0;
	ImGui::SameLine();

	if (ImGui::ImageButton(thirdMap->GetSRV(), ImVec2(50, 50)))
		selectMap = 1;
	ImGui::SameLine();

	if (ImGui::ImageButton(fourthMap->GetSRV(), ImVec2(50, 50)))
		selectMap = 2;

	SaveHeightMap();
	ImGui::SameLine();
	LoadHeightMap();

	SaveAlphaMap();
	ImGui::SameLine();
	LoadAlphaMap();

	material->GUIRender();

	/* Circle Rect Report
	if (ImGui::Button("Circle")) brushBuffer->data.type = 0;
	ImGui::SameLine();
	if (ImGui::Button("Rect")) brushBuffer->data.type = 1;
	*/
}

Vector3 TerrainEditor::Picking()
{
	Ray ray = CAM->ScreenPointToRay(mousePos);

	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			UINT index[4];
			index[0] = width * z + x;
			index[1] = width * z + x + 1;
			index[2] = width * (z + 1) + x;
			index[3] = width * (z + 1) + x + 1;

			Vector3 p[4];
			for (UINT i = 0; i < 4; i++)
				p[i] = vertices[index[i]].position;

			float distance = 0.0f;
			if (Intersects(ray.position, ray.direction,
				p[0], p[1], p[2], distance))
			{
				return ray.position + ray.direction * distance;
			}

			if (Intersects(ray.position, ray.direction,
				p[3], p[1], p[2], distance))
			{
				return ray.position + ray.direction * distance;
			}
		}
	}

	return Vector3();
}

Vector3 TerrainEditor::ComputePicking()
{
	Ray ray = CAM->ScreenPointToRay(mousePos);

	rayBuffer->data.position = ray.position;
	rayBuffer->data.direction = ray.direction;
	rayBuffer->data.size = size;    // 폴리곤 수 넣어줌

	rayBuffer->SetCSBuffer(0);

	DC->CSSetShaderResources(0, 1, &structuredBuffer->GetSRV());
	DC->CSSetUnorderedAccessViews(0, 1, &structuredBuffer->GetUAV(), nullptr);

	computeShader->Set();

	// 올림처리
	UINT x = ceil((float)size / 64.0f);

	DC->Dispatch(x, 1, 1);

	structuredBuffer->Copy(output, sizeof(OutputDesc) * size);

	float minDistance = FLT_MAX;
	int minIndex = -1;

	for (UINT i = 0; i < size; i++)
	{
		OutputDesc temp = output[i];
		if (temp.picked)
		{// 충돌이 났으면
			if (minDistance > temp.distance)
			{
				minDistance = temp.distance;
				minIndex = i;
			}
		}
	}

	if (minIndex >= 0)
	{
		// 반직선 공식으로 어디에 충돌났는지 리턴
		return ray.position + ray.direction * minDistance;
	}

	return Vector3();
}

void TerrainEditor::AdjustHeight()
{
	Vector3 pickingPos = brushBuffer->data.pickingPos;

	switch (brushBuffer->data.type)
	{
	case 0:
	{
		for (VertexType& vertex : vertices)
		{
			Vector3 p1 = Vector3(vertex.position.x, 0, vertex.position.z);
			pickingPos.y = 0;

			float distance = (p1 - pickingPos).Length();

			float temp = adjustValue * max(0, cos(XM_PIDIV2 * distance / brushBuffer->data.range));

			if (distance <= brushBuffer->data.range)
			{
				vertex.position.y += temp * DELTA;

				vertex.position.y = max(MIN_HEIGHT, vertex.position.y);
				vertex.position.y = min(MAX_HEIGHT, vertex.position.y);
			}
		}
	}
	break;

	case 1:
	{
		// 사각형의 반을 구한다
		float size = brushBuffer->data.range * 0.5f;

		float left   = max(0, pickingPos.x - size);
		float right  = max(0, pickingPos.x + size);
		float top    = max(0, pickingPos.z + size);
		float bottom = max(0, pickingPos.z - size);

		for (UINT z = (UINT)bottom; z <= (UINT)top; z++)
		{
			for(UINT x = (UINT)left; x <= (UINT)right; x++)
			{
				UINT index = width * (height - z  - 1) + x;

				if(index >= vertices.size()) continue;

				vertices[index].position.y += adjustValue * DELTA;

				vertices[index].position.y = max(MIN_HEIGHT, vertices[index].position.y);
				vertices[index].position.y = min(MAX_HEIGHT, vertices[index].position.y);
			}
		}

		/* Report Rect
		for (VertexType& vertex : vertices)
		{
			Vector3 p1 = Vector3(vertex.position.x, 0, vertex.position.z);

			float x = abs(p1.x - pickingPos.x);
			float z = abs(p1.z - pickingPos.z);

			if (x <= brushBuffer->data.range && z <= brushBuffer->data.range)
			{
				vertex.position.y += adjustValue * DELTA;

				vertex.position.y = max(MIN_HEIGHT, vertex.position.y);
				vertex.position.y = min(MAX_HEIGHT, vertex.position.y);
			}
		}*/
	}
	break;

	}
	mesh->UpdateVertex(vertices.data(), vertices.size());
}

void TerrainEditor::AdjustAlpha()
{
	Vector3 pickingPos = brushBuffer->data.pickingPos;

	switch (brushBuffer->data.type)
	{
	case 0:
	{
		for (VertexType& vertex : vertices)
		{
			Vector3 p1 = Vector3(vertex.position.x, 0, vertex.position.z);
			pickingPos.y = 0;

			float distance = (p1 - pickingPos).Length();

			float temp = adjustValue * max(0, cos(XM_PIDIV2 * distance / brushBuffer->data.range));

			if (distance <= brushBuffer->data.range)
			{
				vertex.alpha[selectMap] += temp * DELTA;

				vertex.alpha[selectMap] = max(0.0f, vertex.alpha[selectMap]);
				vertex.alpha[selectMap] = min(1.0f, vertex.alpha[selectMap]);
			}
		}
	}
	break;

	case 1:
	{
		// 사각형의 반을 구한다
		float size = brushBuffer->data.range * 0.5f;

		float left = max(0, pickingPos.x - size);
		float right = max(0, pickingPos.x + size);
		float top = max(0, pickingPos.z + size);
		float bottom = max(0, pickingPos.z - size);

		for (UINT z = (UINT)bottom; z <= (UINT)top; z++)
		{
			for (UINT x = (UINT)left; x <= (UINT)right; x++)
			{
				UINT index = width * (height - z - 1) + x;

				if (index >= vertices.size()) continue;

				vertices[index].alpha[selectMap] += adjustValue * DELTA;

				vertices[index].alpha[selectMap] = max(0.0f, vertices[index].alpha[selectMap]);
				vertices[index].alpha[selectMap] = min(1.0f, vertices[index].alpha[selectMap]);
			}
		}
	}
	break;

	}
	mesh->UpdateVertex(vertices.data(), vertices.size());
}

void TerrainEditor::InitNormalTangent()
{
	for (VertexType& vertex : vertices)
	{
		vertex.normal = Vector3();
		vertex.tangent = Vector3();
	}
}

void TerrainEditor::CreateMesh()
{
	vector<Float4> pixels;
	pixels.resize(width * height);

	vertices.clear();
	indices.clear();

	if (heightMap)
	{
		width = heightMap->Width();
		height = heightMap->Height();

		pixels = heightMap->ReadPixels();
	}

	//Vertices
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			VertexType vertex;
			vertex.position = { (float)x, 0.0f, width - (float)z - 1.0f };
			vertex.uv.x = x / (float)(width - 1);
			vertex.uv.y = z / (float)(height - 1);

			//Float4 color = pixels[vertices.size()];
			//vertex.position.y = color.x * MAX_HEIGHT;
			UINT index = width * z + x;
			vertex.position.y = pixels[index].x * MAX_HEIGHT;

			vertices.emplace_back(vertex);
		}
	}

	//Indices
	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			indices.push_back(width * z + x);//0            
			indices.push_back(width * (z + 1) + x + 1);//2
			indices.push_back(width * (z + 1) + x);//1

			indices.push_back(width * z + x);//0            
			indices.push_back(width * z + x + 1);//3
			indices.push_back(width * (z + 1) + x + 1);//2
		}
	}

	CreateNormal();
	CreateTangent();

	mesh = new Mesh(vertices.data(), sizeof(VertexType), (UINT)vertices.size(),
		indices.data(), indices.size());
}

void TerrainEditor::CreateNormal()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v0 = vertices[index0].position;
		Vector3 v1 = vertices[index1].position;
		Vector3 v2 = vertices[index2].position;

		Vector3 A = v1 - v0;
		Vector3 B = v2 - v0;

		Vector3 normal = Vector3::Cross(A, B).GetNormalized();

		vertices[index0].normal = normal + vertices[index0].normal;
		vertices[index1].normal = normal + vertices[index1].normal;
		vertices[index2].normal = normal + vertices[index2].normal;
	}
}

void TerrainEditor::CreateTangent()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		VertexType vertex0 = vertices[index0];
		VertexType vertex1 = vertices[index1];
		VertexType vertex2 = vertices[index2];

		Vector3 p0 = vertex0.position;
		Vector3 p1 = vertex1.position;
		Vector3 p2 = vertex2.position;

		Float2 uv0 = vertex0.uv;
		Float2 uv1 = vertex1.uv;
		Float2 uv2 = vertex2.uv;

		Vector3 e0 = p1 - p0;
		Vector3 e1 = p2 - p0;

		float u0 = uv1.x - uv0.x;
		float v0 = uv1.y - uv0.y;
		float u1 = uv2.x - uv0.x;
		float v1 = uv2.y - uv0.y;

		float d = 1.0f / (u0 * v1 - v0 * u1);
		Vector3 tangent = d * (e0 * v1 - e1 * v0);

		vertices[index0].tangent = vertices[index0].tangent + tangent;
		vertices[index1].tangent = vertices[index1].tangent + tangent;
		vertices[index2].tangent = vertices[index2].tangent + tangent;
	}

	for (VertexType& vertex : vertices)
	{
		Vector3 t = vertex.tangent;
		Vector3 n = vertex.normal;

		vertex.tangent = (t - n * Vector3::Dot(n, t)).GetNormalized();
	}
}

void TerrainEditor::SaveHeightMap()
{
	if (ImGui::Button("SaveHeight"))
		ImGuiFileDialog::Instance()->OpenDialog("SaveHeightMap", "SaveHeightMap", ".png", ".");

	if (ImGuiFileDialog::Instance()->Display("SaveHeightMap"))
	{
		string file = ImGuiFileDialog::Instance()->GetFilePathName();

		char path[128];
		GetCurrentDirectoryA(128, path);

		file = file.substr(strlen(path) + 1, file.length());

		// 한 정점당 4개의 값이 들어간다.
		UINT size = width * height * 4;
		uint8_t* pixels = new uint8_t[size];

		for (UINT i = 0; i < size / 4; i++)
		{
			float y = vertices[i].position.y;

			//uint: 0 ~ 255, 비율로 뽑을려고 이렇게 계산함.
            uint8_t height = (y + MIN_HEIGHT) / (MAX_HEIGHT - MIN_HEIGHT) * 255;

            pixels[i * 4 + 0] = height;
            pixels[i * 4 + 1] = height;
            pixels[i * 4 + 2] = height;
            pixels[i * 4 + 3] = 255;
        }

        Image image;
        image.width = width;
        image.height = height;
        image.pixels = pixels;
        image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
        image.rowPitch = width * 4;
        image.slicePitch = size;

		SaveToWICFile(image, WIC_FLAGS_FORCE_RGB, GetWICCodec(WIC_CODEC_PNG),
			ToWString(file).c_str());

        ImGuiFileDialog::Instance()->Close();
    }
}

void TerrainEditor::LoadHeightMap()
{
	if (ImGui::Button("LoadHieght"))
		ImGuiFileDialog::Instance()->OpenDialog("LoadHeightMap", "LoadHeightMap", ".png", ".");

	if (ImGuiFileDialog::Instance()->Display("LoadHeightMap"))
	{
		string file = ImGuiFileDialog::Instance()->GetFilePathName();

		char path[128];
		GetCurrentDirectoryA(128, path);

		file = file.substr(strlen(path) + 1, file.length());

		heightMap = Texture::Add(ToWString(file));

		delete mesh;

		CreateMesh();

		delete structuredBuffer;
		delete rayBuffer;
		delete[] input;
		delete[] output;

		CreateComputeData();

		ImGuiFileDialog::Instance()->Close();
	}
}

void TerrainEditor::SaveAlphaMap()
{
	if (ImGui::Button("SaveAlpha"))
		ImGuiFileDialog::Instance()->OpenDialog("SaveAlphaMap", "SaveAlphaMap", ".png", ".");

	if (ImGuiFileDialog::Instance()->Display("SaveAlphaMap"))
	{
		string file = ImGuiFileDialog::Instance()->GetFilePathName();

		char path[128];
		GetCurrentDirectoryA(128, path);

		file = file.substr(strlen(path) + 1, file.length());

		// 한 정점당 4개의 값이 들어간다.
		UINT size = width * height * 4;
		uint8_t* pixels = new uint8_t[size];

		for (UINT i = 0; i < size / 4; i++)
		{
			pixels[i * 4 + 0] = vertices[i].alpha[0] * 255;
			pixels[i * 4 + 1] = vertices[i].alpha[1] * 255;
			pixels[i * 4 + 2] = vertices[i].alpha[2] * 255;
			pixels[i * 4 + 3] = vertices[i].alpha[3] * 255;; // fourth할 때 이것도 설정
		}

		Image image;
		image.width = width;
		image.height = height;
		image.pixels = pixels;
		image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
		image.rowPitch = width * 4;
		image.slicePitch = size;

		SaveToWICFile(image, WIC_FLAGS_FORCE_RGB, GetWICCodec(WIC_CODEC_PNG),
			ToWString(file).c_str());

		ImGuiFileDialog::Instance()->Close();
	}
}

void TerrainEditor::LoadAlphaMap()
{
	if (ImGui::Button("LoadAlpha"))
		ImGuiFileDialog::Instance()->OpenDialog("LoadAlphaMap", "LoadAlphaMap", ".png", ".");

	if (ImGuiFileDialog::Instance()->Display("LoadAlphaMap"))
	{
		string file = ImGuiFileDialog::Instance()->GetFilePathName();

		char path[128];
		GetCurrentDirectoryA(128, path);

		file = file.substr(strlen(path) + 1, file.length());

		Texture* alphaMap = Texture::Add(ToWString(file));

		vector<Float4> pixels = alphaMap->ReadPixels();

		for(UINT i = 0; i < vertices.size(); i++)
		{
			vertices[i].alpha[0] = pixels[i].x;
			vertices[i].alpha[1] = pixels[i].y;
			vertices[i].alpha[2] = pixels[i].z;
			vertices[i].alpha[3] = pixels[i].w;
		}

		mesh->UpdateVertex(vertices.data(), vertices.size());

		ImGuiFileDialog::Instance()->Close();
	}
}

ReportRobot_211201* TerrainEditor::CreateRobot(Float3 pos)
{
	ReportRobot_211201* robot = new ReportRobot_211201(pos);

	return robot;
}

void TerrainEditor::CreateComputeData()
{
	computeShader = Shader::AddCS(L"ComputePicking.hlsl");

	size = indices.size() / 3; // 폴리곤수

	input = new InputDesc[size];
	output = new OutputDesc[size];

	for (UINT i = 0; i < size; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		input[i].v0 = vertices[index0].position;
		input[i].v1 = vertices[index1].position;
		input[i].v2 = vertices[index2].position;

		input[i].index = i;
	}

	structuredBuffer = new StructuredBuffer(input, sizeof(InputDesc), size,
		sizeof(OutputDesc), size);

	rayBuffer = new RayBuffer();

}
