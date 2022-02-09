#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN

#ifdef NDEBUG
#define V(hr) hr
#else
#define V(hr) assert(SUCCEEDED(hr))
#endif

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

#define WIN_START_X 100
#define WIN_START_Y 100

#define CENTER_X WIN_WIDTH * 0.5f
#define CENTER_Y WIN_HEIGHT * 0.5f

#define MAX_BONE 256
#define MAX_FRAME 256
#define MAX_LIGHT 10
#define MAX_INSTANCE 100

#define LERP(s, e, t) (s + (e - s)*t)

#define DEVICE Device::Get()->GetDevice()
#define DC Device::Get()->GetDeviceContext()

#define DELTA ImGui::GetIO().DeltaTime

#define KEY_PRESS(k) ImGui::IsKeyDown(k)
#define KEY_DOWN(k) ImGui::IsKeyPressed(k, false)
#define KEY_UP(k) ImGui::IsKeyReleased(k)

#define MOUSE_PRESS(k) ImGui::IsMouseDown(k)
#define MOUSE_CLICK(k) ImGui::IsMouseClicked(k)
#define MOUSE_DBCLICK(k) ImGui::IsMouseDoubleClicked(k)
#define MOUSE_UP(k) ImGui::IsMouseReleased(k)

#define CAM Environment::Get()->GetCamrea()
#define FRUSTUM Environment::Get()->GetFrustum()

#include <windows.h>
#include <vector>
#include <map>				// 2진 트리, 하나씩찾음 || 자동정렬
#include <algorithm>		// sort
#include <unordered_map>	// hash Table, 검색시 좀 빠름. 찾는 효율
#include <string>
#include <assert.h>			// 경고창 라이브러리
#include <functional>

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <ImGuiFileDialog.h>

#pragma comment(lib, "ImGui.lib")

// Direct Tex (Texture)
#include <DirectXTex/DirectXTex.h>

#pragma comment(lib, "DirectXTex.lib")

//DirectWrite
#include <d2d1_2.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

//Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#pragma comment(lib, "assimp-vc142-mtd.lib")

using namespace std;
using namespace DirectX; // float3
using namespace DirectX::TriangleTests;

typedef XMFLOAT4 Float4;
typedef XMFLOAT3 Float3;
typedef XMFLOAT2 Float2;
typedef XMVECTOR Vector4;
typedef XMMATRIX Matrix;
typedef XMFLOAT4X4 Float4x4;

typedef function<void()> Event;
// Framework Header
#include "Framework/Utility/Utility.h"

using namespace Utility;

#include "Framework/Utility/Singleton.h"
#include "Framework/Utility/BinaryReader.h"
#include "Framework/Utility/BinaryWriter.h"
#include "Framework/Utility/Font.h"

#include "Framework/System/Device.h"

#include "Framework/Shader/Shader.h"
#include "Framework/Shader/VertexShader.h"
#include "Framework/Shader/PixelShader.h"
#include "Framework/Shader/ComputeShader.h"
#include "Framework/Shader/GeometryShader.h"

#include "Framework/Buffer/VertexLayouts.h"
#include "Framework/Buffer/VertexBuffer.h"
#include "Framework/Buffer/IndexBuffer.h"
#include "Framework/Buffer/ConstBuffer.h"
#include "Framework/Buffer/RowBuffer.h"
#include "Framework/Buffer/StructuredBuffer.h"
#include "Framework/Buffer/GlobalBuffer.h"

#include "Framework/Render/Texture.h"
#include "Framework/Render/Material.h"
#include "Framework/Render/Mesh.h"

#include "Framework/Math/Vector3.h"
#include "Framework/Math/GameMath.h"
#include "Framework/Math/Transform.h"

using namespace GameMath;

#include "Framework/State/SamplerState.h"
#include "Framework/State/RasterizerState.h"
#include "Framework/State/BlendState.h"
#include "Framework/State/DepthStacilState.h"

#include "Framework/Environment/Camera.h"
#include "Framework/Environment/Frustum.h"
#include "Framework/Environment/Environment.h"

#include "Framework/Model/ModelType.h"
#include "Framework/Model/ModelExporter.h"
#include "Framework/Model/ModelMesh.h"
#include "Framework/Model/ModelReader.h"
#include "Framework/Model/Model.h"
#include "Framework/Model/ModelClip.h"
#include "Framework/Model/ModelAnimator.h"
#include "Framework/Model/ModelInstancing.h"
#include "Framework/Model/ModelAnimatorInstancing.h"

#include "Framework/Collision/Collider.h"
#include "Framework/Collision/BoxCollider.h"
#include "Framework/Collision/SphereCollider.h"
#include "Framework/Collision/CapsuleCollider.h"

#include "Algorithm/Node.h"
#include "Algorithm/Heap.h"
#include "Algorithm/AStar.h"
// Object Header
#include "Object/Basic/Quad.h"
#include "Object/Basic/Cube.h"
#include "Object/Basic/Grid.h"
#include "Object/Basic/Sphere.h"

#include "Object/UI/Bar.h"

#include "Object/Landscape/Terrain.h"
#include "Object/Landscape/Terraineditor.h"
#include "Object/Landscape/Sky.h"
#include "Object/Landscape/Box.h"

#include "Object/Model/Sword.h"
#include "Object/Model/Zombie.h"
#include "Object/Model/horse.h"
#include "Object/Model/Gumdramon.h"
#include "Object/Model/JapanHouse.h"
#include "Object/Model/Chuchu.h"
#include "Object/Model/InstancingChuchu.h"
#include "Object/Model/Amber.h"
#include "Object/Model/Mistsplitter.h"
#include "Object/Model/Wing.h"
#include "Object/Model/Jean.h"
#include "Object/Model/RockPillar.h"
#include "Object/Model/RockShield.h"
#include "Object/Model/RockBoss.h"

// Object Report
#include "Object/ReportCube.h"
#include "Object/ReportRobot_211201.h"
#include "Object/ReportGrid.h"
#include "Object/ReportCamera_211205.h"
#include "Object/SKillBall.h"

#include "Object/Manager/InstancingMonsterManager.h"
#include "Object/Manager/MonsterManager.h"

//Manager Header
#include "Manager/SceneManager.h"

//Prgram Header;
#include "Scenes/Scene.h"
#include "Programe/Program.h"

extern HWND hWnd; // 전역변수 이런게있다 하고 알려줌
extern Vector3 mousePos;