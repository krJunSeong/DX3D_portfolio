#include "Framework.h"

Transform::Transform()
    : scale(1, 1, 1), parent(nullptr), isActive(true)
{
    world = XMMatrixIdentity();
}

void Transform::UpdateWorld(bool isQuat)
{
    if (isQuat)
    {
        world = XMMatrixTransformation(pivot, XMQuaternionIdentity(), scale,
            pivot, rotation, position);
    }
    else
    {
        world = XMMatrixTransformation(pivot, XMQuaternionIdentity(), scale,
            pivot, XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z),
            position);
    }

    if (parent != nullptr)
        world *= *parent;

    XMMatrixDecompose(&globalScale.data, &globalRotation.data,
        &globalPosition.data, world);
}

void Transform::GUIRender()
{
    if (ImGui::TreeNode(tag.c_str()))
    {
        ImGui::Checkbox(tag.c_str(), &isActive);
        ImGui::DragFloat3((tag + "_Pos").c_str(), (float*)&position, 0.1f);
        Vector3 degree;
        degree.x = XMConvertToDegrees(rotation.x);
        degree.y = XMConvertToDegrees(rotation.y);
        degree.z = XMConvertToDegrees(rotation.z);
        ImGui::DragFloat3((tag + "_Rot").c_str(), (float*)&degree, 0.1f);
        rotation.x = XMConvertToRadians(degree.x);
        rotation.y = XMConvertToRadians(degree.y);
        rotation.z = XMConvertToRadians(degree.z);
        ImGui::DragFloat3((tag + "_Scale").c_str(), (float*)&scale, 0.1f);

        if (ImGui::Button("Save"))
            Save();
        ImGui::SameLine();
        if (ImGui::Button("Load"))
            Load();

        ImGui::TreePop();
    }
}

Vector3 Transform::Forward()
{
    return XMVector3Normalize(XMVector3TransformNormal(Vector3(0, 0, 1), world));
}

Vector3 Transform::Up()
{
    return XMVector3Normalize(XMVector3TransformNormal(Vector3(0, 1, 0), world));
}

Vector3 Transform::Right()
{
    return XMVector3Normalize(XMVector3TransformNormal(Vector3(1, 0, 0), world));
}

void Transform::Save()
{
    BinaryWriter w("TextData/Transforms/" + tag + ".srt");

    w.Vector(position);
    w.Vector(rotation);
    w.Vector(scale);
}

void Transform::Load()
{
    BinaryReader r("TextData/Transforms/" + tag + ".srt");

    if (r.IsFailed()) return;

    position = r.Vector();
    rotation = r.Vector();
    scale = r.Vector();
}
