#include "GameSelectScene.h"

void GameSelectScene::Initialize() {
	//CJEngine
	CJEngine_ = CitrusJunosEngine::GetInstance();

	//Input
	input_ = Input::GetInstance();

	//TextureManager
	textureManager_ = TextureManager::GetInstance();

	//Audio
	audio_ = Audio::GetInstance();

	bgmData_ = audio_->SoundLoad("project/gamedata/resources/sounds/bgm.mp3");
	selectData_ = audio_->SoundLoad("project/gamedata/resources/sounds/select.mp3");
	audio_->SoundPlayWave(bgmData_, 0.1f, true);

	//テクスチャ
	titleResource_ = textureManager_->Load("project/gamedata/resources/UI/Title.png");
	title1Resource_ = textureManager_->Load("project/gamedata/resources/UI/Title1.png");

	for (int i = 0; i < 2; i++)
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
	}

	sprite_[0]->Initialize(Vector2{ 1280.0f,720.0f }, titleResource_);
	sprite_[0]->SetAnchor(Vector2{ 0.5f,0.5f });

	sprite_[1]->Initialize(Vector2{ 1280.0f,720.0f }, title1Resource_);
	sprite_[1]->SetAnchor(Vector2{ 0.5f,0.5f });
}

void GameSelectScene::Update() {
	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(0, joyState);

	//Title表示の処理
	titleTimer_--;

	if (titleTimer_ <= 60 && titleTimer_ > 30)
	{
		isSpriteDraw_[0] = true;
		isSpriteDraw_[1] = false;
	}
	else if (titleTimer_ <= 30 && titleTimer_ > 0)
	{
		isSpriteDraw_[0] = false;
		isSpriteDraw_[1] = true;
	}
	else
	{
		titleTimer_ = 60;
	}

	ImGui::Begin("debug");
	ImGui::Text("GameTitleScene");
	ImGui::Text("DemoScene:N key or A button");
	ImGui::End();

	if (input_->TriggerKey(DIK_R)) {
		sceneNo = GAME_SCENE;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}

	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	if (input_->PushAButton(joyState)) {
		sceneNo = GAME_SCENE;
	}
}

void GameSelectScene::Draw() {
#pragma region 背景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);

#pragma endregion

#pragma region 3Dオブジェクト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard3D);

#pragma endregion

#pragma region パーティクル描画
	CJEngine_->renderer_->Draw(PipelineType::Particle);

#pragma endregion

#pragma region 前景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);

	for (int i = 0; i < 2; i++)
	{
		if (isSpriteDraw_[i])
		{
			//Sprite描画
			sprite_[i]->Draw(spriteTransform_[i], SpriteuvTransform_[i], spriteMaterial_[i]);
		}
	}

#pragma endregion
}

void GameSelectScene::Finalize() {

}