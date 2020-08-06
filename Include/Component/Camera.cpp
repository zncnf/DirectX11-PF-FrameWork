#include "D3DUtil.h"
#include "Camera.h"
#include "Transform.h"

Camera::Camera(GameObject * _object)
{
	gameObject = _object;
	
}

Camera::~Camera()
{
}

void Camera::Init()
{
	GameManager::GetInstance()->camera_Position = gameObject->transform->position + D3DXVECTOR3(0, 0, -1);
	GameManager::GetInstance()->camera_Lookat   = gameObject->transform->position + gameObject->transform->forward;
	GameManager::GetInstance()->camera_Up       = gameObject->transform->up;

	float width = WindowManager::GetInstance()->GetWindowWidth();
	float hight = WindowManager::GetInstance()->GetWindowHeight();

	float fieldOfView  = 3.141592654f / 4.0f;
	float screenAspect = width / hight;

	D3DXMatrixIdentity(&GameManager::GetInstance()->worldMatrix);
	D3DXMatrixLookAtLH(&GameManager::GetInstance()->viewMatrix, &GameManager::GetInstance()->camera_Position, &GameManager::GetInstance()->camera_Lookat, &GameManager::GetInstance()->camera_Up);
	D3DXMatrixLookAtLH(&GameManager::GetInstance()->viewMatrix_2D, &GameManager::GetInstance()->camera_Position, &GameManager::GetInstance()->camera_Lookat, &GameManager::GetInstance()->camera_Up);
	D3DXMatrixPerspectiveFovLH(&GameManager::GetInstance()->projectionMatrix, fieldOfView, screenAspect, 0.3f, 1000);
	D3DXMatrixOrthoLH(&GameManager::GetInstance()->orthoMatrix, width, hight, 0.1f, 1000);
}

void Camera::Update()
{
	CameraController();

	GameManager::GetInstance()->camera_Position = gameObject->transform->position;
	GameManager::GetInstance()->camera_Lookat = gameObject->transform->position + gameObject->transform->forward;
	GameManager::GetInstance()->camera_Up = D3DXVECTOR3(0, 1, 0);

	D3DXMatrixLookAtLH(&GameManager::GetInstance()->viewMatrix, &GameManager::GetInstance()->camera_Position, &GameManager::GetInstance()->camera_Lookat, &GameManager::GetInstance()->camera_Up);
}

void Camera::CameraController()
{
	float moveSpeed = 5.0f;

	if (InputManager::GetInstance()->KeyboardDown(DIK_LSHIFT))
	{
		moveSpeed = 0.5f;
	}

	if (InputManager::GetInstance()->KeyboardDown(DIK_S))
	{
		gameObject->transform->Translate(D3DXVECTOR3(0, 0, 1) * moveSpeed * -1);
	}

	if (InputManager::GetInstance()->KeyboardDown(DIK_W))
	{
		gameObject->transform->Translate(D3DXVECTOR3(0, 0, 1) * moveSpeed);
	}

	if (InputManager::GetInstance()->KeyboardDown(DIK_D))
	{
		gameObject->transform->Translate(D3DXVECTOR3(1, 0, 0) * moveSpeed);
	}

	if (InputManager::GetInstance()->KeyboardDown(DIK_A))
	{
		gameObject->transform->Translate(D3DXVECTOR3(1, 0, 0) * moveSpeed * -1);
	}

	if (InputManager::GetInstance()->KeyboardDown(DIK_Q))
	{
		gameObject->transform->Translate(D3DXVECTOR3(0, 1, 0) * moveSpeed * -1);
	}

	if (InputManager::GetInstance()->KeyboardDown(DIK_E))
	{
		gameObject->transform->Translate(D3DXVECTOR3(0, 1, 0) * moveSpeed);
	}
}
