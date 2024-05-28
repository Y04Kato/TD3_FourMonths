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

	//Timer
	numbers_ = std::make_unique<Numbers>();
	numbers_->Initialize();
	numbers_->SetInitialNum(0 / 60);

	numbersTransform_.scale = { 1.5f,1.5f,1.5f };
	numbersTransform_.rotate = { 0.0f,0.0f,0.0f };
	numbersTransform_.translate = { 500.0f,292.0f,0.0f };

	//Datas
	datas_ = Datas::GetInstance();
}

void GameClearScene::Update() {
	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(0, joyState);

	if (isSceneStart_ == true) {//ゲーム開始時の処理
		spriteTransform_[3].translate.num[0] = 1061.0f;
		spriteTransform_[3].translate.num[1] = 632.0f;

		isSceneStart_ = false;
	}

	numbers_->SetNum(datas_->GetClearTime() / 60);
	numbers_->SetTransform(numbersTransform_);

	if (input_->TriggerKey(DIK_A) && spriteTransform_[3].translate.num[0] == 1061.0f)
	{
		spriteTransform_[3].translate.num[0] = 680.0f;
	}

	if (input_->TriggerKey(DIK_A) && spriteTransform_[3].translate.num[0] == 1441.0f)
	{
		spriteTransform_[3].translate.num[0] = 1061.0f;
	}

	if (input_->TriggerKey(DIK_D) && spriteTransform_[3].translate.num[0] == 1061.0f)
	{
		spriteTransform_[3].translate.num[0] = 1441.0f;
	}

	if (input_->TriggerKey(DIK_D) && spriteTransform_[3].translate.num[0] == 680.0f)
	{
		spriteTransform_[3].translate.num[0] = 1061.0f;
	}

	ImGui::Begin("debug");
	ImGui::Text("GameClearScene");
	ImGui::SliderFloat3("SWTFT", &spriteTransform_[3].translate.num[0], 0.0f, 2280.0f);
	ImGui::SliderFloat3("trans", numbersTransform_.translate.num, 0.0f, 2280.0f);
	ImGui::End();

	//左側(現在のステージを繰り返し)
	if (spriteTransform_[3].translate.num[0] == 680.0f && input_->TriggerKey(DIK_SPACE) &&
		GameSelectScene::stageNum == 1) {
		sceneNo = GAME_SCENE;
		isSceneStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}

	if (spriteTransform_[3].translate.num[0] == 680.0f && input_->TriggerKey(DIK_SPACE) &&
		GameSelectScene::stageNum == 2) {
		sceneNo = GAME_SCENE2;
		isSceneStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}

	//真ん中(次のステージ)
	if (spriteTransform_[3].translate.num[0] == 1061.0f && input_->TriggerKey(DIK_SPACE) &&
		datas_->GetStageNum() == 1) {
		sceneNo = GAME_SCENE2;
		isSceneStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}

	if (spriteTransform_[3].translate.num[0] == 1061.0f && input_->TriggerKey(DIK_SPACE) &&
		datas_->GetStageNum() == 2) {
		sceneNo = TITLE_SCENE;
		isSceneStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}

	//右側(ステージセレクト)
	if (spriteTransform_[3].translate.num[0] == 1441.0f && input_->TriggerKey(DIK_SPACE)) {
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

	numbers_->Draw();

#pragma endregion
}

void GameClearScene::Finalize() {

}