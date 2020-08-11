#pragma once
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#define NOMINMAX
#define NUM_BONES_PER_VERTEX 4

// 미리컴파일된 헤더(Precompiled Headers)를 사용하는 모든 cpp파일에 추가시켜주면 됩니다.

#pragma region C/C++ Librarys
#include <Windows.h>
//#include <wincodec.h> DirectX SDK와 호환이안되는문제

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
#include "assimp/pbrmaterial.h"

#pragma comment(lib, "assimp-vc140-mt.lib")
#pragma endregion


#pragma region Managers
#include "Managers/WindowManager.h"
#include "Managers/DirectXManager.h"
#include "Managers/GameManager.h"
#include "Managers/ResourceManager.h"
#include "Managers/InputManager.h"
#pragma endregion

#pragma region Namespace
using namespace std;
#pragma endregion

#pragma region Macros
#define ReleaseCOM(x)      { if(x){ x->Release(); x = nullptr; } }
#define SafeDelete(x)      { delete x; x = nullptr; }
#define SafeDeleteArray(x) { if(x) { delete[] (x); (x) = nullptr; } }
#define ZEROMEM(x)		    memset(x, 0, sizeof(x)) 
#define ARRAY_SIZE_IN_ELEMENTS(x) (sizeof(x)/sizeof(x[0]))
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

struct Cbuffer_BoneData
{
	D3DXMATRIX bones[100];
};

struct VertexType_SkindMesh
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 texture;
	D3DXVECTOR3 normal;
	UINT boneID[NUM_BONES_PER_VERTEX];
	float weights[NUM_BONES_PER_VERTEX];
};

struct SkinDrawData
{
	UINT vertexCount;
	UINT idicesCount;
	UINT vertexBaseStart;
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


struct BoneInfo
{
	aiMatrix4x4 boneOffset;
	aiMatrix4x4 finalTransformation;

	BoneInfo()
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				boneOffset[i][j] = 0.0f;
				finalTransformation[i][j] = 0.0f;
			}
		}
	}
};

struct VertexBoneData
{
	UINT Ids[NUM_BONES_PER_VERTEX];
	float Weights[NUM_BONES_PER_VERTEX];

	VertexBoneData()
	{
		Reset();
	}

	void Reset()
	{
		ZEROMEM(Ids);
		ZEROMEM(Weights);
	}

	void AddBoneData(UINT boneID, float weight)
	{
		for (UINT i = 0; i < ARRAY_SIZE_IN_ELEMENTS(Ids); i++)
		{
			if (Weights[i] = 0.0)
			{
				Ids[i] = boneID;
				Weights[i] = weight;
				return;
			}
		}
	}
};







