#include "D3DUtil.h"
#include "Execute.h"
#include "Component/Camera.h"
#include "Component/Transform.h"
#include "Component/RectTransform.h"
#include "Component/MeshFilter.h"
#include "Component/MeshRenderer.h"
#include "Component/Light.h"
#include "Component/Text.h"

#include "Shader/DefaultShader_Specular.h"

Execute::~Execute()
{
	delete directionalLight;
	delete camera;
	delete crusaderKnight;
	delete text;
}

void Execute::Init()
{
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

	shader_Specular->Init();

	ResourceManager::GetInstance()->AddShader("DefaultShader_Specular", shader_Specular);
}

void Execute::Set3DObject()
{
	directionalLight = new GameObject(ObjectType::Empty);
	directionalLight->AddComponent(new Light(directionalLight, LightType::DirectionalLight));
	directionalLight->Init();

	camera = new GameObject(ObjectType::Empty);
	camera->AddComponent(new Camera(camera));
	camera->Init();
	camera->GetComponent<Transform>()->SetWorldPosition(D3DXVECTOR3(0, 150, -350));

	crusaderKnight = new GameObject(ObjectType::Object3D, "Resource/GameObject/Crusader knight/animation/crusader@atack1.fbx");
	crusaderKnight->Init();

	crusaderKnight->childs[0]->childs[0]->childs[0]->childs[0]->GetComponent<MeshRenderer>()->SetTexturePath("Resource/GameObject/Crusader knight/textures/mech_Albedo.png");

	for (auto i : crusaderKnight->childs[1]->childs[0]->childs)
	{
		i->childs[0]->GetComponent<MeshRenderer>()->SetTexturePath("Resource/GameObject/Crusader knight/textures/crusader body_Albedopng.png");
	}
}

void Execute::Set2DObject()
{
	text = new GameObject(ObjectType::UI);
	text->AddComponent(new Text(text, L"Arial", L"crusader base mesh.fbx", 28, D2D1::ColorF::Gold));
	text->Init();
	text->GetComponent<RectTransform>()->SetWorldPosition(D3DXVECTOR3(630, 50, 0));
}

void Execute::Render3D()
{
	directionalLight->Update();
	camera->Update();
	crusaderKnight->Update();

	crusaderKnight->GetComponent<Transform>()->Rotate(D3DXVECTOR3(0, 1, 0));
}

void Execute::Render2D()
{
	DirectXManager::GetInstance()->GetRenderTarget()->BeginDraw();
	{
		text->Update();
	}
	DirectXManager::GetInstance()->GetRenderTarget()->EndDraw();
}
