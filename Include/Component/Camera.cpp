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
	//UpdateViewMatrix();
	CameraController();

	GameManager::GetInstance()->camera_Position = gameObject->transform->position;
	GameManager::GetInstance()->camera_Lookat = gameObject->transform->position + gameObject->transform->forward;
	GameManager::GetInstance()->camera_Up = gameObject->transform->up;

	D3DXMatrixLookAtLH(&GameManager::GetInstance()->viewMatrix, &GameManager::GetInstance()->camera_Position, &GameManager::GetInstance()->camera_Lookat, &GameManager::GetInstance()->camera_Up);
}

void Camera::Pitch(float angle)
{
	D3DXMATRIX R;

	D3DXMatrixRotationAxis(&R, &gameObject->transform->right, D3DXToRadian(angle));

	D3DXVec3TransformNormal(&gameObject->transform->up, &gameObject->transform->up, &R);
	D3DXVec3TransformNormal(&gameObject->transform->forward, &gameObject->transform->forward, &R);
}

void Camera::RotateY(float angle)
{
	D3DXMATRIX Y;

	D3DXMatrixRotationY(&Y, D3DXToRadian(angle));

	D3DXVec3TransformNormal(&gameObject->transform->right, &gameObject->transform->right, &Y);
	D3DXVec3TransformNormal(&gameObject->transform->up, &gameObject->transform->up, &Y);
	D3DXVec3TransformNormal(&gameObject->transform->forward, &gameObject->transform->forward, &Y);
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
		gameObject->transform->Translate(gameObject->transform->forward * moveSpeed * -1);
	}

	if (InputManager::GetInstance()->KeyboardDown(DIK_W))
	{
		gameObject->transform->Translate(gameObject->transform->forward * moveSpeed);
	}

	if (InputManager::GetInstance()->KeyboardDown(DIK_D))
	{
		gameObject->transform->Translate(gameObject->transform->right * moveSpeed);
	}

	if (InputManager::GetInstance()->KeyboardDown(DIK_A))
	{
		gameObject->transform->Translate(gameObject->transform->right * moveSpeed * -1);
	}

	if (InputManager::GetInstance()->KeyboardDown(DIK_Q))
	{
		gameObject->transform->Translate(D3DXVECTOR3(0, 1, 0) * moveSpeed * -1);
	}

	if (InputManager::GetInstance()->KeyboardDown(DIK_E))
	{
		gameObject->transform->Translate(D3DXVECTOR3(0, 1, 0) * moveSpeed);
	}

	RotateY(InputManager::GetInstance()->m_mouseState.lX / 4);
	Pitch(InputManager::GetInstance()->m_mouseState.lY / 4);
}

void Camera::UpdateViewMatrix()
{
	D3DXVECTOR3 F;
	D3DXVECTOR3 U;
	D3DXVECTOR3 R;

	D3DXVec3Normalize(&F, &gameObject->transform->forward);
	D3DXVec3Cross(&U, &gameObject->transform->right, &gameObject->transform->forward);
	D3DXVec3Normalize(&U, &U);
	D3DXVec3Cross(&R, &U, &F);

	float x = -1 * D3DXVec3Dot(&gameObject->transform->position, &R);
	float y = -1 * D3DXVec3Dot(&gameObject->transform->position, &U);
	float z = -1 * D3DXVec3Dot(&gameObject->transform->position, &F);

	gameObject->transform->right = R;
	gameObject->transform->up = U;
	gameObject->transform->forward = F;

	GameManager::GetInstance()->viewMatrix._11 = gameObject->transform->right.x;
	GameManager::GetInstance()->viewMatrix._21 = gameObject->transform->right.y;
	GameManager::GetInstance()->viewMatrix._31 = gameObject->transform->right.z;
	GameManager::GetInstance()->viewMatrix._41 = x;

	GameManager::GetInstance()->viewMatrix._12 = gameObject->transform->up.x;
	GameManager::GetInstance()->viewMatrix._22 = gameObject->transform->up.y;
	GameManager::GetInstance()->viewMatrix._32 = gameObject->transform->up.z;
	GameManager::GetInstance()->viewMatrix._42 = y;

	GameManager::GetInstance()->viewMatrix._13 = gameObject->transform->forward.x;
	GameManager::GetInstance()->viewMatrix._23 = gameObject->transform->forward.y;
	GameManager::GetInstance()->viewMatrix._33 = gameObject->transform->forward.z;
	GameManager::GetInstance()->viewMatrix._43 = z;

	GameManager::GetInstance()->viewMatrix._14 = 0.0f;
	GameManager::GetInstance()->viewMatrix._24 = 0.0f;
	GameManager::GetInstance()->viewMatrix._34 = 0.0f;
	GameManager::GetInstance()->viewMatrix._44 = 1.0f;
}
