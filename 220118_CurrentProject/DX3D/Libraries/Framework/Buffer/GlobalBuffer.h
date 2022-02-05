#pragma once

enum class ObjectType
{
    NORMAL, ANIM
};

class MatrixBuffer : public ConstBuffer
{
private:
    struct Data
    {
        Matrix matrix;
    }data;

public:
    MatrixBuffer() : ConstBuffer(&data, sizeof(Data))
    {
        data.matrix = XMMatrixIdentity();
    }

    void Set(Matrix value)
    {
        data.matrix = XMMatrixTranspose(value);
    }
};

class WorldBuffer : public ConstBuffer
{
private:
    struct Data
    {
        Matrix matrix;

        int type = 0;

        float padding[3];
    }data;

public:
    WorldBuffer() : ConstBuffer(&data, sizeof(Data))
    {
        data.matrix = XMMatrixIdentity();
    }

    void Set(Matrix value)
    {
        data.matrix = XMMatrixTranspose(value);
    }

    void SetType(ObjectType type)
    {
        data.type = (int)type;
    }
};

class ViewBuffer : public ConstBuffer
{
private:
    struct Data
    {
        Matrix view;
        Matrix invView;
    }data;

public:
    ViewBuffer() : ConstBuffer(&data, sizeof(Data))
    {
        data.view = XMMatrixIdentity();
        data.invView = XMMatrixIdentity();
    }

    void Set(Matrix view)
    {
        data.view = XMMatrixTranspose(view);
        Matrix invView = XMMatrixInverse(nullptr, view);
        data.invView = XMMatrixTranspose(invView);
    }
};

class ColorBuffer : public ConstBuffer
{
public:
    struct Data
    {
        Float4 color;
    }data;

    ColorBuffer() : ConstBuffer(&data, sizeof(Data))
    {
        data.color = { 1, 1, 1, 1 };
    }

    void Set(float r, float g, float b)
    {
        data.color = { r, g, b, 1 };
    }
};

enum class LightType
{
    DIRECTIONAL, POINT, SPOT, CAPSULE
};

class LightBuffer : public ConstBuffer
{
public:
    struct Light
    {
        Float4 color = { 1, 1, 1, 1 };

        Float3 direction = { 0, -1, 1 };
        LightType type = LightType::DIRECTIONAL;

        Float3 position = { 0, 0, 0 };
        float range = 50.0f;

        float inner = 55.0f;
        float outer = 70.0f;
        float length = 50.0f;
        int active = 1;
    };

    struct Data
    {
        Light lights[MAX_LIGHT];

        int lightCount = 1;
        Float3 ambient = { 0.0f, 0.0f, 0.0f };
        Float3 ambientCeil = { 0.0f, 0.0f, 0.0f };

        float padding;
    }data;

    LightBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    }
};

class MaterialBuffer : public ConstBuffer
{
public:
    struct Data
    {
        Float4 diffuse = { 1, 1, 1, 1 };
        Float4 specular = { 1, 1, 1, 1 };
        Float4 ambient = { 1, 1, 1, 1 };
        Float4 emissive = { 0, 0, 0, 0 };

        float shininess = 24.0f;

        int hasDiffuseMap = 0;
        int hasSpecularMap = 0;
        int hasNormalMap = 0;
    }data;

    MaterialBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    }
};

class RayBuffer : public ConstBuffer
{
public:
    struct Data
    {
        Float3 position = { 0, 0, 0 };
        float size = 0.0f;

        Float3 direction = { 0, 0, 0 };
        float padding;
    }data;

    RayBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    }
};

class BrushBuffer : public ConstBuffer
{
public:
    struct Data
    {
        int type = 0;
        Float3 pickingPos = { 0, 0, 0 };

        float range = 10.0f;
        Float3 color = { 0, 0.5f, 0 };
    }data;

    BrushBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    }
};

class FrameBuffer : public ConstBuffer
{
public:
    struct Frame
    {
        int clip = 0;
        UINT curFrame = 0;
        float time = 0.0f;
        float speed = 1.0f;
    };

    struct Data
    {
        float takeTime = 0.2f;
        float tweenTime = 0.0f;
        float runningTime = 0.0f;
        float padding;

        Frame cur;
        Frame next;
    }data;

    FrameBuffer() : ConstBuffer(&data, sizeof(Data))
    {
        data.next.clip = -1;
    }
};

class FrameInstancingBuffer : public ConstBuffer
{
public:
    struct Frame
    {
        int clip = 0;
        UINT curFrame = 0;
        float time = 0.0f;
        float speed = 1.0f;
    };
    struct Motion
    {
        float takeTime = 0.2f;
        float tweenTime = 0.0f;
        float runningTime = 0.0f;
        float padding;

        Frame cur;
        Frame next;

        Motion()
        {
            next.clip = -1;
        }
    };

    struct Data
    {
        Motion motions[MAX_INSTANCE];
    }data;

    FrameInstancingBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    }
};

class ValueBuffer : public ConstBuffer
{
public:
    struct Data
    {
        float values[4] = {};
    }data;

    ValueBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    }
};