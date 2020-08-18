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
#include "Component/Animator.h"

#include "Shader/DefaultShader_Specular.h"
#include "Shader/DefaultShader_Skinned.h"

#include "Asset/AnimationClip.h"
#include "Asset/AnimationController.h"

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
	shared_ptr<Shader> shader_Skinned(new DefaultShader_Skinned);

	shader_Specular->Init();
	shader_Skinned->Init();

	ResourceManager::GetInstance()->AddShader("DefaultShader_Specular", shader_Specular);
	ResourceManager::GetInstance()->AddShader("DefaultShader_Skinned", shader_Skinned);
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

	AnimationClip* attack1 = new AnimationClip("attack1", "Resource/GameObject/Crusader knight/animation/crusader@atack1.fbx");
	AnimationClip* death1  = new AnimationClip("death1",  "Resource/GameObject/Crusader knight/animation/crusader@death1.fbx");
	AnimationClip* gethit  = new AnimationClip("gethit",  "Resource/GameObject/Crusader knight/animation/crusader@gethit.fbx");
	AnimationClip* idle1   = new AnimationClip("idle1",   "Resource/GameObject/Crusader knight/animation/crusader@idle1.fbx");
	AnimationClip* jump    = new AnimationClip("jump",    "Resource/GameObject/Crusader knight/animation/crusader@jump.fbx");
	AnimationClip* run     = new AnimationClip("run",     "Resource/GameObject/Crusader knight/animation/crusader@run.fbx");
	AnimationClip* walk    = new AnimationClip("walk",    "Resource/GameObject/Crusader knight/animation/crusader@walk.fbx");

	AnimationController* controller = new AnimationController();
	controller->AddAnimationClip(attack1);
	controller->AddAnimationClip(death1);
	controller->AddAnimationClip(gethit);
	controller->AddAnimationClip(idle1);
	controller->AddAnimationClip(jump);
	controller->AddAnimationClip(run);
	controller->AddAnimationClip(walk);	

	crusaderKnight = new GameObject(ObjectType::Object3D, "Resource/GameObject/Crusader knight/Base mesh/crusader base mesh.fbx");
	crusaderKnight->AddComponent(new Animator(crusaderKnight));
	crusaderKnight->GetComponent<Animator>()->AddController(controller);
	crusaderKnight->Init();	

	//파일자체 문제로 Assimp 가 텍스쳐 경로를 원할하게 받아오지못하여 수동으로 셋팅해줌.
	for (auto i : crusaderKnight->GetComponentsInAllChildren<SkinnedMeshRenderer>())
	{
		if (i->name == "mech:RetopoGroup1") // 칼
			i->SetTexturePath("Resource/GameObject/Crusader knight/textures/mech_Albedo.png");
		else
			i->SetTexturePath("Resource/GameObject/Crusader knight/textures/crusader body_Albedopng.png");
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
	if (GameManager::GetInstance()->gameTime > 5.0f)
	{
		if (crusaderKnight->GetComponent<Animator>()->animationOver)
		{
			crusaderKnight->GetComponent<Animator>()->PlayAnimationWithClipName("run");
		}
		else
		{
			if (!crusaderKnight->GetComponent<Animator>()->animationPlaying)
			{
				crusaderKnight->GetComponent<Animator>()->PlayAnimationWithClipName("walk");
			}
		}
	}

	directionalLight->Update();
	camera->Update();
	crusaderKnight->Update();
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
