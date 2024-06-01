#include"GameClearScene.h"
#include "GameSelectScene.h"

void GameClearScene::Initialize() {
	//CJEngine
	CJEngine_ = CitrusJunosEngine::GetInstance();

	//Input
	input_ = Input::GetInstance();

	//TextureManager
	textureManager_ = TextureManager::GetInstance();

	//Audio
	audio_ = Audio::GetInstance();

	selectData_ = audio_->SoundLoad("project/gamedata/resources/sounds/select.mp3");

	//テクスチャ
	spriteResource_[0] = textureManager_->Load("project/gamedata/resources/UI/bg.png");
	spriteResource_[1] = textureManager_->Load("project/gamedata/resources/UI/ResultUI.png");
	spriteResource_[2] = textureManager_->Load("project/gamedata/resources/UI/GoalUI.png");
	spriteResource_[3] = textureManager_->Load("project/gamedata/resources/UI/Cursor.png");
	starTextureHandle_ = textureManager_->Load("project/gamedata/resources/UI/star.png");

	for (int i = 0; i < 4; i++)
	{
		spriteMaterial_[i] = { 1.0f,1.0f,1.0f,1.0f };
		spriteTransform_[i] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f} };

		SpriteuvTransform_[i] = {
			{1.0f,1.0f,1.0f},
			{0.0f,0.0f,0.0f},
			{0.0f,0.0f,0.0f},
		};

		sprite_[i] = std::make_unique <CreateSprite>();
		isSpriteDraw_[i] = false;

		sprite_[i]->Initialize(Vector2{ 1280.0f,720.0f }, spriteResource_[i]);
		sprite_[i]->SetAnchor(Vector2{ 0.5f,0.5f });
	}

	spriteTransform_[1].translate.num[0] = 520.0f;
	spriteTransform_[1].translate.num[1] = 300.0f;

	for (uint32_t index = 0; index < 3; index++) {
		starSprite_[index] = std::make_unique<CreateSprite>();
		starSprite_[index]->Initialize({ starTextureSize_, starTextureSize_ }, starTextureHandle_);
		starSprite_[index]->SetAnchor(Vector2{ 0.5f, 0.5f });
	}
	starTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{800.0f, 150.0f,0.0f} };


	itemNumbers_ = std::make_unique<Numbers>();
	itemNumbers_->Initialize();
	itemNumbers_->SetInitialNum(0 / 60);
	itemNumbersTransform_ = { {1.0f, 1.0f, 1.0f}, { 0.0f,0.0f,0.0f}, {430.0f,215.0f,0.0f} };

	//クリアタイム
	timer_ = std::make_unique<Timer>();
	timer_->Initialize();
	timer_->SetInitialNum(0 / 60);
	timerTransform_ = { {1.0f, 1.0f, 1.0f}, { 0.0f,0.0f,0.0f}, {420.0f,310.0f,0.0f} };

	//Datas
	datas_ = Datas::GetInstance();
}

void GameClearScene::Update() {
	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(0, joyState);

	if (isSceneStart_ == true) {//ゲーム開始時の処理
		spriteTransform_[3].translate.num[0] = 1061.0f;

		spriteTransform_[3].translate.num[1] = 690.0f;

		achievementCount_ = 0;

		isSceneStart_ = false;
	}

	if (datas_->GetClearTime() < 10.0f) {
		achievementCount_ = 1;
	}

	itemNumbers_->SetNum(1 / 60);
	itemNumbers_->SetTransform(itemNumbersTransform_);
	timer_->SetNum(datas_->GetClearTime());
	timer_->SetTransform(timerTransform_);

	if (input_->TriggerKey(DIK_A) && spriteTransform_[3].translate.num[0] == 1061.0f)
	{
		spriteTransform_[3].translate.num[0] = 678.0f;
	}

	if (input_->TriggerKey(DIK_A) && spriteTransform_[3].translate.num[0] == 1440.0f)
	{
		spriteTransform_[3].translate.num[0] = 1061.0f;
	}

	if (input_->TriggerKey(DIK_D) && spriteTransform_[3].translate.num[0] == 1061.0f)
	{
		spriteTransform_[3].translate.num[0] = 1440.0f;
	}

	if (input_->TriggerKey(DIK_D) && spriteTransform_[3].translate.num[0] == 678.0f)
	{
		spriteTransform_[3].translate.num[0] = 1061.0f;
	}

	ImGui::Begin("debug");
	ImGui::Text("GameClearScene");
	ImGui::DragFloat3("SWTFT", &spriteTransform_[3].translate.num[0], 0.0f, 2280.0f);
	ImGui::DragFloat3("trans", itemNumbersTransform_.translate.num, 0.0f, 2280.0f);
	ImGui::DragFloat3("star", starTransform_.translate.num, 0.0f, 2280.0f);
	ImGui::DragFloat3("item", itemNumbersTransform_.translate.num, 0.0f, 2280.0f);
	ImGui::DragFloat3("time", timerTransform_.translate.num, 0.0f, 2280.0f);
	ImGui::End();

	//左側(現在のステージを繰り返し)
	if (spriteTransform_[3].translate.num[0] == 678.0f && input_->TriggerKey(DIK_SPACE) &&
		datas_->GetStageNum() == 1) {
		sceneNo = GAME_SCENE;
		isSceneStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}

	if (spriteTransform_[3].translate.num[0] == 678.0f && input_->TriggerKey(DIK_SPACE) &&
		datas_->GetStageNum() == 2) {
		sceneNo = GAME_SCENE;
		isSceneStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}

	if (spriteTransform_[3].translate.num[0] == 678.0f && input_->TriggerKey(DIK_SPACE) &&
		datas_->GetStageNum() == 3) {
		sceneNo = GAME_SCENE;
		isSceneStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}

	if (spriteTransform_[3].translate.num[0] == 678.0f && input_->TriggerKey(DIK_SPACE) &&
		datas_->GetStageNum() == 4) {
		sceneNo = GAME_SCENE;
		isSceneStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}

	if (spriteTransform_[3].translate.num[0] == 678.0f && input_->TriggerKey(DIK_SPACE) &&
		datas_->GetStageNum() == 5) {
		sceneNo = GAME_SCENE;
		isSceneStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}

	if (spriteTransform_[3].translate.num[0] == 678.0f && input_->TriggerKey(DIK_SPACE) &&
		datas_->GetStageNum() == 6) {
		sceneNo = GAME_SCENE;
		isSceneStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}

	//真ん中(次のステージ)
	if (spriteTransform_[3].translate.num[0] == 1061.0f && input_->TriggerKey(DIK_SPACE) &&
		datas_->GetStageNum() == 1) {
		sceneNo = GAME_SCENE;
		datas_->SetStageNum(2);
		isSceneStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}
	else if (spriteTransform_[3].translate.num[0] == 1061.0f && input_->TriggerKey(DIK_SPACE) &&
		datas_->GetStageNum() == 2) {
		sceneNo = GAME_SCENE;
		datas_->SetStageNum(3);
		isSceneStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}
	else if (spriteTransform_[3].translate.num[0] == 1061.0f && input_->TriggerKey(DIK_SPACE) &&
		datas_->GetStageNum() == 3) {
		sceneNo = GAME_SCENE;
		datas_->SetStageNum(4);
		isSceneStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}
	else if (spriteTransform_[3].translate.num[0] == 1061.0f && input_->TriggerKey(DIK_SPACE) &&
		datas_->GetStageNum() == 4) {
		sceneNo = GAME_SCENE;
		datas_->SetStageNum(5);
		isSceneStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}
	else if (spriteTransform_[3].translate.num[0] == 1061.0f && input_->TriggerKey(DIK_SPACE) &&
		datas_->GetStageNum() == 5) {
		sceneNo = GAME_SCENE;
		datas_->SetStageNum(6);
		isSceneStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}
	else if (spriteTransform_[3].translate.num[0] == 1061.0f && input_->TriggerKey(DIK_SPACE) &&
		datas_->GetStageNum() == 6) {
		sceneNo = TITLE_SCENE;
		isSceneStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}

	//右側(ステージセレクト)
	if (spriteTransform_[3].translate.num[0] == 1440.0f && input_->TriggerKey(DIK_SPACE)) {
		sceneNo = SELECT_SCENE;
		isSceneStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}

	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	if (input_->PushAButton(joyState)) {
		sceneNo = SELECT_SCENE;
		isSceneStart_ = true;
	}
}

void GameClearScene::Draw() {
#pragma region 背景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);
	sprite_[0]->Draw(spriteTransform_[0], SpriteuvTransform_[0], spriteMaterial_[0]);
#pragma endregion

#pragma region 3Dオブジェクト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard3D);

#pragma endregion

#pragma region パーティクル描画
	CJEngine_->renderer_->Draw(PipelineType::Particle);

#pragma endregion

#pragma region 前景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);

	sprite_[1]->Draw(spriteTransform_[1], SpriteuvTransform_[1], spriteMaterial_[1]);

	sprite_[2]->Draw(spriteTransform_[2], SpriteuvTransform_[2], spriteMaterial_[2]);

	sprite_[3]->Draw(spriteTransform_[3], SpriteuvTransform_[3], spriteMaterial_[3]);

	for (uint32_t index = 0; index < achievementCount_; index++) {
		EulerTransform transform = starTransform_;
		transform.translate.num[1] += starTextureSize_ * index;
		starSprite_[index]->Draw(transform, SpriteuvTransform_[0], { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	itemNumbers_->Draw();
	timer_->Draw();
#pragma endregion
}

void GameClearScene::Finalize() {

}