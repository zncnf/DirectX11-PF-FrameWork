#include "D3DUtil.h"
#include "Execute.h"
#include "Component/Camera.h"
#include "Component/Transform.h"
#include "Component/RectTransform.h"
#include "Component/Light.h"
#include "Component/Text.h"

#include "Shader/DefaultShader_Specular.h"

Execute::~Execute()
{
	delete camera;
	delete skull;
}

void Execute::Init()
{
	GameManager::GetInstance()->Init();

	SetShaders();
	Set3DObject();
	Set2DObject();
}

void Execute::Update()
{
	Render3D();
	Render2D();
}

void Execute::SetShaders()
{
	shared_ptr<Shader> shader_Specular(new DefaultShader_Specular);

	ResourceManager::GetInstance()->addShader("DefaultShader_Specular", shader_Specular);
}

void Execute::Set3DObject()
{
	directionalLight = new GameObject(ObjectType::Empty);
	directionalLight->AddComponent(new Light(directionalLight, LightType::DirectionalLight));
	directionalLight->Init();

	camera = new GameObject(ObjectType::Empty);
	camera->AddComponent(new Camera(camera));
	camera->Init();

	skull = new GameObject(ObjectType::Object3D, "Resource/GameObject/12140_Skull_v3_L2.obj");
	skull->Init();
	skull->GetComponent<Transform>()->Rotate(D3DXVECTOR3(90, 0, 0));
}

void Execute::Set2DObject()
{
	text = new GameObject(ObjectType::UI);
	text->AddComponent(new Text(text, L"±Ã¼­", L"12140_Skull_v3_L2.obj", 28, D2D1::ColorF::Gold));
	text->Init();
	text->GetComponent<RectTransform>()->SetWorldPosition(D3DXVECTOR3(630, 50, 0));
}

void Execute::Render3D()
{
	directionalLight->Update();
	camera->Update();
	skull->Update();

	skull->GetComponent<Transform>()->Rotate(D3DXVECTOR3(0, 1, 0));
}

void Execute::Render2D()
{
	DirectXManager::GetInstance()->GetRenderTarget()->BeginDraw();
	{
		text->Update();

	}
	DirectXManager::GetInstance()->GetRenderTarget()->EndDraw();
}
