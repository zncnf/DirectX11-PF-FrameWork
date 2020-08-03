#pragma once
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#define NOMINMAX

// 미리컴파일된 헤더(Precompiled Headers)를 사용하는 모든 cpp파일에 추가시켜주면 됩니다.

#pragma region C/C++ Librarys
#include <Windows.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>	
#include <memory>
#include <cassert>
#pragma endregion

#pragma region DirectX Librarys
#include <d3dcompiler.h>
#include <d3d11.h>
#include <D3DX11async.h>
#include <DirectXMath.h>
#include <D3DX10math.h>
#include <dxerr.h>
#include <dinput.h>
#include <d2d1.h>
#include <dwrite.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx11d.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "D2d1.lib")
#pragma endregion

#pragma region External Librarys
#include "assimp/Importer.hpp"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#pragma comment(lib, "assimp-vc140-mt.lib")
#pragma endregion


#pragma region Managers
#include "WindowManager.h"
#include "DirectXManager.h"
#include "Managers/GameManager.h"
#include "Managers/ResourceManager.h"
#pragma endregion

#pragma region Namespace
using namespace std;
#pragma endregion

#pragma region Macros
#define ReleaseCOM(x)      { if(x){ x->Release(); x = nullptr; } }
#define SafeDelete(x)      { delete x; x = nullptr; }
#define SafeDeleteArray(x) { if(x) { delete[] (x); (x) = nullptr; } }
#pragma endregion

#pragma region Structs
struct CBuffer_Matrix
{
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_projectionMatrix;
};

struct CBuffer_Camera
{
	D3DXVECTOR3 cameraPosition;
	float		padding;
};

struct VertexType_PTN
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 texture;
	D3DXVECTOR3 normal;
};

struct ShaderType_Specular
{
	D3DXVECTOR4 ambientColor;
	D3DXVECTOR4 diffuseColor;
	D3DXVECTOR3 lightDirection;
	float	    specularPower;
	D3DXVECTOR4 specularColor;
};
#pragma endregion






