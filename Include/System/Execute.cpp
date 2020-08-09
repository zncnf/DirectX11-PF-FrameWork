#include "D3DUtil.h"
#include "Execute.h"
#include "Component/Camera.h"
#include "Component/Transform.h"
#include "Component/RectTransform.h"
#include "Component/MeshFilter.h"
#include "Component/MeshRenderer.h"
#include "Component/SkinnedMeshRenderer.h"
#include "Component/Light.h"
#include "Component/Text.h"
#include "Component/Image.h"

#include "Shader/DefaultShader_Specular.h"

Execute::~Execute()
{
	delete directionalLight;
	delete camera;
	delete crusaderKnight;
	delete fps;
	delete mspf;
	delete gameTime;
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

	crusaderKnight->childs[0]->childs[0]->childs[0]->childs[0]->GetComponent<SkinnedMeshRenderer>()->SetTexturePath("Resource/GameObject/Crusader knight/textures/mech_Albedo.png");

	for (auto i : crusaderKnight->childs[1]->childs[0]->childs)
	{
		i->childs[0]->GetComponent<SkinnedMeshRenderer>()->SetTexturePath("Resource/GameObject/Crusader knight/textures/crusader body_Albedopng.png");
	}	
}

void Execute::Set2DObject()
{
	fps = new GameObject(ObjectType::UI);
	fps->AddComponent(new Text(fps, L"Arial", L"FPS : ", 25, D2D1::ColorF::Gold));
	fps->Init();
	fps->GetComponent<RectTransform>()->SetWorldPosition(D3DXVECTOR3(55, 30, 0));

	mspf = new GameObject(ObjectType::UI);
	mspf->AddComponent(new Text(mspf, L"Arial", L"Frame Time : ", 25, D2D1::ColorF::Gold));
	mspf->Init();
	mspf->GetComponent<RectTransform>()->SetWorldPosition(D3DXVECTOR3(55, 55, 0));


	gameTime = new GameObject(ObjectType::UI);
	gameTime->AddComponent(new Text(gameTime, L"Arial", L"Game Time : ", 25, D2D1::ColorF::Gold));
	gameTime->Init();
	gameTime->GetComponent<RectTransform>()->SetWorldPosition(D3DXVECTOR3(55, 80, 0));
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
		std::wstring str;
		wchar_t* str_;

		str = L"FPS : " + std::to_wstring(GameManager::GetInstance()->fps);
		str_ = const_cast<wchar_t*>(str.c_str());

		fps->GetComponent<Text>()->SetText(str_);
		fps->Update();

		str = L"Frame Time : " + std::to_wstring(GameManager::GetInstance()->mspf);
		str_ = const_cast<wchar_t*>(str.c_str());

		mspf->GetComponent<Text>()->SetText(str_);
		mspf->Update();

		str = L"Game Time : " + std::to_wstring(GameManager::GetInstance()->gameTime);
		str_ = const_cast<wchar_t*>(str.c_str());

		gameTime->GetComponent<Text>()->SetText(str_);
		gameTime->Update();
	}
	DirectXManager::GetInstance()->GetRenderTarget()->EndDraw();
}
