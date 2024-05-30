#include"SceneManager.h"

void SceneManager::Run() {
	Initialize();
	Update();
	Finalize();
}

void SceneManager::Initialize() {
	//WindowTitle
	const char kWindowTitle[] = "title";
	//COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);
	//Engine
	CJEngine_ = CitrusJunosEngine::GetInstance();
	CJEngine_->Initialize(kWindowTitle, 1280, 720);
	//DirectX
	dxCommon_ = DirectXCommon::GetInstance();
	//Audio
	audio_ = Audio::GetInstance();
	audio_->Initialize();

	//Input
	input_ = Input::GetInstance();
	input_->Initialize();

	//Light
	directionalLight_ = DirectionalLights::GetInstance();
	directionalLight_->Initialize();
	pointLight_ = PointLights::GetInstance();
	pointLight_->Initialize();

	//PostEffect
	postEffectList_ = PostEffectLists::GetInstance();
	postEffectList_->Initialize();

	//TextureManager
	textureManager_ = TextureManager::GetInstance();
	textureManager_->Initialize();

	//ImGui
	imGuiManager_ = ImGuiManager::GetInstance();
	imGuiManager_->Initialize(WinApp::GetInstance(), dxCommon_);

	//CSV
	GlobalVariables::GetInstance()->LoadFiles();

	//Scene
	scene_[TITLE_SCENE] = std::make_unique<GameTitleScene>();
	scene_[SELECT_SCENE] = std::make_unique<GameSelectScene>();
	scene_[GAME_SCENE] = std::make_unique<GamePlayScene>();
	scene_[GAME_SCENE2] = std::make_unique<GamePlayScene2>();
	scene_[GAME_SCENE3] = std::make_unique<GamePlayScene3>();
	scene_[GAME_SCENE4] = std::make_unique<GamePlayScene4>();
	scene_[GAME_SCENE5] = std::make_unique<GamePlayScene5>();
	scene_[GAME_SCENE6] = std::make_unique<GamePlayScene6>();
	scene_[CLEAR_SCENE] = std::make_unique<GameClearScene>();
	scene_[DEMO_SCENE] = std::make_unique<GameDemoScene>();
	for (int i = 0; i < SCENE_MAX; i++) {
		scene_[i]->Initialize();
	}

	//タイトルシーンから開始
	Iscene::sceneNo = TITLE_SCENE;

	postEffect_ = new PostEffect();
	postEffect_->Initialize();
	postEffect_->ALLCreate();
}


void SceneManager::Update() {
	while (true) {
		//windowのメッセージを最優先で処理させる
		if (WinApp::GetInstance()->Procesmessage()) {
			break;
		}
		postEffect_->PreDraw();

		imGuiManager_->Begin();
		input_->Update();
		GlobalVariables::GetInstance()->Update();
		directionalLight_->Update();
		pointLight_->Update();
		postEffectList_->Update();
		scene_[Iscene::sceneNo]->Update();
		scene_[Iscene::sceneNo]->Draw();
		imGuiManager_->End();

		CJEngine_->BeginFrame();
		postEffect_->PreCopy();

		CJEngine_->renderer_->Draw(PipelineType::PostProcess);
		postEffect_->Draw();

		imGuiManager_->Draw();

		postEffect_->PostCopy();

		CJEngine_->EndFrame();

		//// ESCキーが押されたらループを抜ける
		if (input_->TriggerKey(DIK_ESCAPE)) {
			break;
		}
	}
}

//void SceneManager::Draw() {
//
//}

void SceneManager::Finalize() {
	CJEngine_->Finalize();
	audio_->Finalize();
	imGuiManager_->Finalize();
	for (int i = 0; i < SCENE_MAX; i++) {
		scene_[i]->Finalize();
	}
	CoUninitialize();
}