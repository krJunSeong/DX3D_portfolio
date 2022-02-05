#include "Framework.h"

Material::Material()
    : vertexShader(nullptr), pixelShader(nullptr),
    diffuseMap(nullptr), specularMap(nullptr), normalMap(nullptr)
{
    buffer = new MaterialBuffer();
}

Material::Material(wstring shaderFile)
    : diffuseMap(nullptr), specularMap(nullptr), normalMap(nullptr)
{
    SetShader(shaderFile);

    buffer = new MaterialBuffer();
}

Material::~Material()
{
    delete buffer;
}

void Material::Set()
{
    buffer->SetPSBuffer(1);

    if (diffuseMap != nullptr)
        diffuseMap->Set(0);

    if (specularMap != nullptr)
        specularMap->Set(1);

    if (normalMap != nullptr)
        normalMap->Set(2);

    vertexShader->Set();
    pixelShader->Set();
}

void Material::GUIRender()
{
    string title = name + " Material";

    if (ImGui::TreeNode(title.c_str()))
    {
        char str[128];
        strcpy_s(str, 128, editName.c_str());
        ImGui::InputText("Name", str, 128);
        editName = str;

        ImGui::SameLine();
        if (ImGui::Button("Edit"))
        {
            name = editName;
        }

        SelectShader();

        ImGui::ColorEdit3("Diffuse", (float*)&buffer->data.diffuse, ImGuiColorEditFlags_PickerHueWheel);
        ImGui::ColorEdit3("Specular", (float*)&buffer->data.specular, ImGuiColorEditFlags_PickerHueWheel);
        ImGui::ColorEdit3("Ambient", (float*)&buffer->data.ambient, ImGuiColorEditFlags_PickerHueWheel);
        ImGui::ColorEdit4("Emissive", (float*)&buffer->data.emissive, ImGuiColorEditFlags_PickerHueWheel);

        ImGui::SliderFloat("Shininess", &buffer->data.shininess, 1.0f, 50.0f);

        SelectMap("DM", 0);
        ImGui::SameLine();
        UnselectMap(0);
        ImGui::SameLine();
        SelectMap("SM", 1);
        ImGui::SameLine();
        UnselectMap(1);
        ImGui::SameLine();
        SelectMap("NM", 2);
        ImGui::SameLine();
        UnselectMap(2);

        SaveDialog();
        ImGui::SameLine();
        LoadDialog();

        ImGui::TreePop();
    }
}

void Material::SetShader(wstring file)
{
    vertexShader = Shader::AddVS(file);
    pixelShader = Shader::AddPS(file);
}

void Material::SetDiffuseMap(wstring file)
{
    if (file.length() == 0)
    {
        diffuseMap = nullptr;
        buffer->data.hasDiffuseMap = 0;
        return;
    }

    buffer->data.hasDiffuseMap = 1;
    diffuseMap = Texture::Add(file);
}

void Material::SetSpecularMap(wstring file)
{
    if (file.length() == 0)
    {
        specularMap = nullptr;
        buffer->data.hasSpecularMap = 0;
        return;
    }

    buffer->data.hasSpecularMap = 1;
    specularMap = Texture::Add(file);
}

void Material::SetNormalMap(wstring file)
{
    if (file.length() == 0)
    {
        normalMap = nullptr;
        buffer->data.hasNormalMap = 0;
        return;
    }
    buffer->data.hasNormalMap = 1;
    normalMap = Texture::Add(file);
}

void Material::Save(string file)
{
    BinaryWriter w(file);

    w.String(name);

    if (vertexShader)
        w.WString(vertexShader->GetFile());
    else
        w.WString(L"");

    if (pixelShader)
        w.WString(pixelShader->GetFile());
    else
        w.WString(L"");

    if (diffuseMap)
        w.WString(diffuseMap->GetFile());
    else
        w.WString(L"");

    if (specularMap)
        w.WString(specularMap->GetFile());
    else
        w.WString(L"");

    if (normalMap)
        w.WString(normalMap->GetFile());
    else
        w.WString(L"");

    w.Float(buffer->data.diffuse.x);
    w.Float(buffer->data.diffuse.y);
    w.Float(buffer->data.diffuse.z);
    w.Float(buffer->data.diffuse.w);

    w.Float(buffer->data.specular.x);
    w.Float(buffer->data.specular.y);
    w.Float(buffer->data.specular.z);
    w.Float(buffer->data.specular.w);

    w.Float(buffer->data.ambient.x);
    w.Float(buffer->data.ambient.y);
    w.Float(buffer->data.ambient.z);
    w.Float(buffer->data.ambient.w);

    w.Float(buffer->data.emissive.x);
    w.Float(buffer->data.emissive.y);
    w.Float(buffer->data.emissive.z);
    w.Float(buffer->data.emissive.w);

    w.Float(buffer->data.shininess);
}

void Material::Load(string file)
{
    BinaryReader r(file);

    if (r.IsFailed()) assert(false);

    this->file = file;

    name = r.String();

    vertexShader = Shader::AddVS(r.WString());
    pixelShader = Shader::AddPS(r.WString());

    SetDiffuseMap(r.WString());
    SetSpecularMap(r.WString());
    SetNormalMap(r.WString());

    buffer->data.diffuse.x = r.Float();
    buffer->data.diffuse.y = r.Float();
    buffer->data.diffuse.z = r.Float();
    buffer->data.diffuse.w = r.Float();

    buffer->data.specular.x = r.Float();
    buffer->data.specular.y = r.Float();
    buffer->data.specular.z = r.Float();
    buffer->data.specular.w = r.Float();

    buffer->data.ambient.x = r.Float();
    buffer->data.ambient.y = r.Float();
    buffer->data.ambient.z = r.Float();
    buffer->data.ambient.w = r.Float();

    buffer->data.emissive.x = r.Float();
    buffer->data.emissive.y = r.Float();
    buffer->data.emissive.z = r.Float();
    buffer->data.emissive.w = r.Float();

    buffer->data.shininess = r.Float();
}

void Material::SelectShader()
{
    if (ImGui::Button("Select Shader"))
    {
        ImGuiFileDialog::Instance()->OpenDialog(name + "SelectShader", "SelectShader",
            ".hlsl", ".");
    }

    if (ImGuiFileDialog::Instance()->Display(name + "SelectShader"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            string file = ImGuiFileDialog::Instance()->GetFilePathName();

            file = GetFileName(file);

            SetShader(ToWString(file));
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void Material::SelectMap(string name, int type)
{
    ImTextureID textureID = nullptr;
    wstring key = ToWString(this->name + name);

    switch (type)
    {
    case 0:
        if (diffuseMap)
            textureID = diffuseMap->GetSRV();
        else
            textureID = Texture::Add(L"Textures/UI/diffuseNone.png", key)->GetSRV();
        break;
    case 1:
        if (specularMap)
            textureID = specularMap->GetSRV();
        else
            textureID = Texture::Add(L"Textures/UI/specularNone.png", key)->GetSRV();
        break;
    case 2:
        if (normalMap)
            textureID = normalMap->GetSRV();
        else
            textureID = Texture::Add(L"Textures/UI/normalNone.png", key)->GetSRV();
        break;
    }

    if (ImGui::ImageButton(textureID, ImVec2(50, 50)))
    {
        ImGuiFileDialog::Instance()->OpenDialog(this->name + name, name,
            ".png,.jpg,.tga", ".");
    }

    if (ImGuiFileDialog::Instance()->Display(this->name + name))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            string file = ImGuiFileDialog::Instance()->GetFilePathName();

            char path[128];
            GetCurrentDirectoryA(128, path);

            file = file.substr(strlen(path) + 1, file.length());

            switch (type)
            {
            case 0:
                SetDiffuseMap(ToWString(file));
                break;
            case 1:
                SetSpecularMap(ToWString(file));
                break;
            case 2:
                SetNormalMap(ToWString(file));
                break;
            }
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void Material::UnselectMap(int type)
{
    switch (type)
    {
    case 0:
    {
        Texture* tex = Texture::Add(L"Textures/UI/cancel.png", L"DiffuseCancle");
        if (ImGui::ImageButton(tex->GetSRV(), ImVec2(10, 10)))
            SetDiffuseMap(L"");
    }
    break;
    case 1:
    {
        Texture* tex = Texture::Add(L"Textures/UI/cancel.png", L"SpecularCancle");
        if (ImGui::ImageButton(tex->GetSRV(), ImVec2(10, 10)))
            SetSpecularMap(L"");
    }
    break;
    case 2:
    {
        Texture* tex = Texture::Add(L"Textures/UI/cancel.png", L"NormalCancle");
        if (ImGui::ImageButton(tex->GetSRV(), ImVec2(10, 10)))
            SetNormalMap(L"");
    }
    break;
    }

}

void Material::SaveDialog()
{
    if (ImGui::Button("Save"))
    {
        if (file.empty())
            file = "TextData/Materials/" + name + ".mat";

        Save(file);
    }

    ImGui::SameLine();

    if (ImGui::Button("SaveAs"))
    {
        ImGuiFileDialog::Instance()->OpenDialog(name + "SaveMaterial", "SaveMaterial",
            ".mat", ".");
    }

    if (ImGuiFileDialog::Instance()->Display(name + "SaveMaterial"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            string file = ImGuiFileDialog::Instance()->GetFilePathName();

            char path[128];
            GetCurrentDirectoryA(128, path);

            file = file.substr(strlen(path) + 1, file.length());

            Save(file);
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void Material::LoadDialog()
{
    if (ImGui::Button("Load"))
    {
        ImGuiFileDialog::Instance()->OpenDialog(name + "LoadMaterial", "LoadMaterial",
            ".mat", ".");
    }

    if (ImGuiFileDialog::Instance()->Display(name + "LoadMaterial"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            string file = ImGuiFileDialog::Instance()->GetFilePathName();

            char path[128];
            GetCurrentDirectoryA(128, path);

            file = file.substr(strlen(path) + 1, file.length());

            Load(file);
        }

        ImGuiFileDialog::Instance()->Close();
    }
}
