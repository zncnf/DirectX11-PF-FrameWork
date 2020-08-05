#pragma once
class GameManager
{
private:
	GameManager() = default;
public:
	~GameManager() = default;
public:
	static GameManager* instance;

	static GameManager* GetInstance();
	void   ReleaseInstance();

public:
	void Init();

public:
	D3DXMATRIX worldMatrix;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX viewMatrix_2D;
	D3DXMATRIX projectionMatrix;
	D3DXMATRIX orthoMatrix;

	D3DXVECTOR3 camera_Up;
	D3DXVECTOR3 camera_Position;
	D3DXVECTOR3 camera_Lookat;

	D3DXVECTOR3 drectionalLight;
};

