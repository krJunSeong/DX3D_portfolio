//Const
#define MAX_LIGHT 10
#define MAX_INSTANCE 100

//VetexLayout
struct Vertex
{
    float4 pos : POSITION;
};

struct VertexColor
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct VertexUV
{
    float4 pos : POSITION;
    float2 uv : UV;
};

struct VertexUVNormal
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
};

struct VertexUVNormalTangent
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VertexUVNormalTangentBlend
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 indices : BLENDINDICES;
    float4 weights : BLENDWEIGHTS;
};

struct VertexInstancing
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 indices : BLENDINDICES;
    float4 weights : BLENDWEIGHTS;
    
    matrix transform : INSTANCE_TRANSFORM;
    uint index : INSTANCE_INDEX;
};

//Pixel Input
struct LightPixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewPos : POSITION1;
    float3 worldPos : POSITION2;
};

//VertexShader ContBuffer
cbuffer WorldBuffer : register(b0)
{
    matrix world;
    int objectType;
}

cbuffer ViewBuffer : register(b1)
{
    matrix view;
    matrix invView;
}

cbuffer ProjectionBuffer : register(b2)
{
    matrix projection;
}

struct Frame
{
    int clip;
    uint curFrame;
    float time;
    float speed;
};

struct Motion
{
    float takeTime;
    float tweenTime;
    float runningTime;
    float padding;
    
    Frame cur;
    Frame next;
};

cbuffer FrameBuffer : register(b3)
{
    Motion motion;
}

cbuffer FrameInstancingBuffer : register(b4)
{
    Motion motions[MAX_INSTANCE];
}

Texture2DArray transformMap : register(t0);

//PixelShader ConstBuffer
SamplerState samp : register(s0);

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);

struct Light
{
    float4 color;
    
    float3 direction;
    int type;
    
    float3 position;
    float range;
    
    float inner;
    float outer;
    float length;
    int active;
};

cbuffer LightBuffer : register(b0)
{
    Light lights[MAX_LIGHT];
    
    int lightCount;
    
    float3 ambientLight;
    float3 ambientCeil;
}

cbuffer MaterialBuffer : register(b1)
{
    float4 mDiffuse;
    float4 mSpecular;
    float4 mAmbient;
    float4 mEmissive;
    
    float shininess;
    
    int hasDiffuseMap;
    int hasSpecularMap;
    int hasNormalMap;
}

//Struct
struct Material
{
    float3 normal;
    float4 diffuseColor;
    float4 specularIntensity;
    float3 viewPos;
    float3 worldPos;
};

//Vertex Function
matrix SkinWorld(float4 indices, float4 weights)
{
    matrix transform = 0;
    matrix cur, next;
    matrix curAnim, nextAnim;
    
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    for (int i = 0; i < 4; i++)
    {
        int curFrame = motion.cur.curFrame;
        int clip = motion.cur.clip;
        
        c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0));
        c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0));
        c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0));
        c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0));
        
        cur = matrix(c0, c1, c2, c3);
        
        n0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame + 1, clip, 0));
        n1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame + 1, clip, 0));
        n2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame + 1, clip, 0));
        n3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame + 1, clip, 0));
        
        next = matrix(n0, n1, n2, n3);
        
        curAnim = lerp(cur, next, motion.cur.time);
        
        curFrame = motion.next.curFrame;
        clip = motion.next.clip;
        
        [flatten]        
        if (clip > -1)
        {
            c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0));
            c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0));
            c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0));
            c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0));
        
            cur = matrix(c0, c1, c2, c3);
        
            n0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame + 1, clip, 0));
            n1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame + 1, clip, 0));
            n2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame + 1, clip, 0));
            n3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame + 1, clip, 0));
        
            next = matrix(n0, n1, n2, n3);
        
            nextAnim = lerp(cur, next, motion.next.time);
            
            curAnim = lerp(curAnim, nextAnim, motion.tweenTime);
        }
        
        transform += mul(weights[i], curAnim);
    }
    
    return transform;
}

matrix SkinWorld(uint instanceID, float4 indices, float4 weights)
{
    matrix transform = 0;
    matrix cur, next;
    matrix curAnim, nextAnim;
    
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    for (int i = 0; i < 4; i++)
    {
        int curFrame = motions[instanceID].cur.curFrame;
        int clip = motions[instanceID].cur.clip;
        
        c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0));
        c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0));
        c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0));
        c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0));
        
        cur = matrix(c0, c1, c2, c3);
        
        n0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame + 1, clip, 0));
        n1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame + 1, clip, 0));
        n2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame + 1, clip, 0));
        n3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame + 1, clip, 0));
        
        next = matrix(n0, n1, n2, n3);
        
        curAnim = lerp(cur, next, motions[instanceID].cur.time);
        
        curFrame = motion.next.curFrame;
        clip = motion.next.clip;
        
        [flatten]        
        if (clip > -1)
        {
            c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0));
            c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0));
            c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0));
            c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0));
        
            cur = matrix(c0, c1, c2, c3);
        
            n0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame + 1, clip, 0));
            n1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame + 1, clip, 0));
            n2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame + 1, clip, 0));
            n3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame + 1, clip, 0));
        
            next = matrix(n0, n1, n2, n3);
        
            nextAnim = lerp(cur, next, motions[instanceID].next.time);
            
            curAnim = lerp(curAnim, nextAnim, motions[instanceID].tweenTime);
        }
        
        transform += mul(weights[i], curAnim);
    }
    
    return transform;
}

//Pixel Function
float3 NormalMapping(float3 T, float3 B, float3 N, float2 uv)
{
    T = normalize(T);
    B = normalize(B);
    N = normalize(N);
    
    [flatten]
    if (hasNormalMap)
    {
        float3 normal = normalMap.Sample(samp, uv).rgb;
        
        float3x3 TBN = float3x3(T, B, N);
        N = normal * 2.0f - 1.0f;
        N = normalize(mul(N, TBN));
    }
    
    return N;
}

float4 SpecularMapping(float2 uv)
{
    [flatten]
    if (hasSpecularMap)
        return specularMap.Sample(samp, uv);
    
    return float4(1, 1, 1, 1);
}

Material GetMaterial(LightPixelInput input)
{
    Material material;
    material.normal = NormalMapping(input.tangent, input.binormal, input.normal, input.uv);
    
    [branch]
    if (hasDiffuseMap)
        material.diffuseColor = diffuseMap.Sample(samp, input.uv);
    else
        material.diffuseColor = 1;
    
    material.viewPos = input.viewPos;
    material.specularIntensity = SpecularMapping(input.uv);
    material.worldPos = input.worldPos;
    
    return material;
}

Material GetMaterial(LightPixelInput input, float4 albedo)
{
    Material material;
    material.normal = NormalMapping(input.tangent, input.binormal, input.normal, input.uv);
    material.diffuseColor = albedo;
    material.viewPos = input.viewPos;
    material.specularIntensity = SpecularMapping(input.uv);
    material.worldPos = input.worldPos;
    
    return material;
}

float4 CalcAmbient(Material material)
{
    float up = material.normal.y * 0.5f + 0.5f;
    
    float4 resultAmbient = float4(ambientLight + up * ambientCeil, 1.0f);
    
    return resultAmbient * material.diffuseColor;
}

float4 CalcEmissive(Material material)
{
    float emissiveIntensity = 0.0f;
    
    [flatten]
    if (mEmissive.a > 0.0f)
    {
        float3 viewDir = normalize(material.viewPos - material.worldPos);
        
        float t = saturate(dot(material.normal, viewDir));
        emissiveIntensity = smoothstep(1.0f - mEmissive.a, 1.0f, 1.0f - t);
    }
    
    return float4(mEmissive.rgb * emissiveIntensity, 1.0f);
}

float4 CalcDirectional(Material material, Light light)
{
    float3 toLight = -normalize(light.direction);
    
    float NDotL = saturate(dot(toLight, material.normal));
    float4 finalColor = light.color * NDotL * mDiffuse;
    
    [flatten]
    if (NDotL > 0)
    {
        float3 toEye = normalize(material.viewPos - material.worldPos);
        float3 halfWay = normalize(toEye + toLight);
        float NDotH = saturate(dot(material.normal, halfWay));
        
        finalColor += light.color * pow(NDotH, shininess) * material.specularIntensity * mSpecular;
    }
    
    return finalColor * material.diffuseColor;
}

float4 CalcPoint(Material material, Light light)
{
    float3 toLight = light.position - material.worldPos;
    float distanceToLight = length(toLight);
    toLight /= distanceToLight;
    
    float NDotL = saturate(dot(toLight, material.normal));
    float4 finalColor = light.color * NDotL * mDiffuse;
    
    [flatten]
    if (NDotL > 0)
    {
        float3 toEye = normalize(material.viewPos - material.worldPos);
        float3 halfWay = normalize(toEye + toLight);
        float NDotH = saturate(dot(material.normal, halfWay));
        
        finalColor += light.color * pow(NDotH, shininess) * material.specularIntensity * mSpecular;
    }
    
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attention = distanceToLightNormal * distanceToLightNormal;
    
    return finalColor * material.diffuseColor * attention;
}

float4 CalcSpot(Material material, Light light)
{
    float3 toLight = light.position - material.worldPos;
    float distanceToLight = length(toLight);
    toLight /= distanceToLight;
    
    float NDotL = saturate(dot(toLight, material.normal));
    float4 finalColor = light.color * NDotL * mDiffuse;
    
    [flatten]
    if (NDotL > 0)
    {
        float3 toEye = normalize(material.viewPos - material.worldPos);
        float3 halfWay = normalize(toEye + toLight);
        float NDotH = saturate(dot(material.normal, halfWay));
        
        finalColor += light.color * pow(NDotH, shininess) * material.specularIntensity * mSpecular;
    }
    
    float3 dir = -normalize(light.direction);
    float cosAngle = dot(dir, toLight);
    
    float outer = cos(radians(light.outer));
    float inner = 1.0f / cos(radians(light.inner));
    
    float conAttention = saturate((cosAngle - outer) * inner);
    conAttention *= conAttention;
    
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attention = distanceToLightNormal * distanceToLightNormal;
    
    return finalColor * material.diffuseColor * attention * conAttention;
}

float4 CalcCapsule(Material material, Light light)
{
    float3 direction = normalize(light.direction);
    float3 start = material.worldPos - light.position;
    float distanceOnLine = dot(start, direction) / light.length;
    distanceOnLine = saturate(distanceOnLine) * light.length;
    
    float3 pointOnLine = light.position + direction * distanceOnLine;
    
    float3 toLight = pointOnLine - material.worldPos;
    float distanceToLight = length(toLight);
    toLight /= distanceToLight;
    
    float NDotL = saturate(dot(toLight, material.normal));
    float4 finalColor = light.color * NDotL * mDiffuse;
    
    [flatten]
    if (NDotL > 0)
    {
        float3 toEye = normalize(material.viewPos - material.worldPos);
        float3 halfWay = normalize(toEye + toLight);
        float NDotH = saturate(dot(material.normal, halfWay));
        
        finalColor += light.color * pow(NDotH, shininess) * material.specularIntensity * mSpecular;
    }
    
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attention = distanceToLightNormal * distanceToLightNormal;
    
    return finalColor * material.diffuseColor * attention;
}

float4 CalcLights(Material material)
{
    float4 result = 0;
    
    for (int i = 0; i < lightCount; i++)
    {
        [flatten]
        if (!lights[i].active)
            continue;
        
        [flatten]
        if (lights[i].type == 0)
            result += CalcDirectional(material, lights[i]);
        else if (lights[i].type == 1)
            result += CalcPoint(material, lights[i]);
        else if (lights[i].type == 2)
            result += CalcSpot(material, lights[i]);
        else if (lights[i].type == 3)
            result += CalcCapsule(material, lights[i]);
    }
    
    return result;
}