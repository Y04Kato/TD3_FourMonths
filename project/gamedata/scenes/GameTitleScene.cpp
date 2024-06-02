#include"GameTitleScene.h"

void GameTitleScene::Initialize(){
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
	bgResource_ = textureManager_->Load("project/gamedata/resources/UI/bg.png");
	annotationResource_ = textureManager_->Load("project/gamedata/resources/UI/Annotention.png");
	escapeResource_ = textureManager_->Load("project/gamedata/resources/UI/ESC.png");

	for (int i = 0; i < 5; i++)
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

	sprite_[2]->Initialize(Vector2{ 1280.0f,720.0f }, bgResource_);
	sprite_[2]->SetAnchor(Vector2{ 0.5f,0.5f });

	sprite_[3]->Initialize(Vector2{ 1280.0f,720.0f }, annotationResource_);
	sprite_[3]->SetAnchor(Vector2{ 0.5f,0.5f });

	sprite_[4]->Initialize(Vector2{ 1280.0f,720.0f }, escapeResource_);
	sprite_[4]->SetAnchor(Vector2{ 0.5f,0.5f });
}

void GameTitleScene::Update(){
	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(0, joyState);

	//Title表示の処理
	titleTimer_--;

	if (titleTimer_ <= 60 && titleTimer_ > 30)
	{
		isSpriteDraw_[0] = true;
	}
	else if (titleTimer_ <= 30 && titleTimer_ > 0)
	{
		isSpriteDraw_[0] = false;
	}
	else
	{
		titleTimer_ = 60;
	}

	ImGui::Begin("debug");
	ImGui::Text("GameTitleScene");
	ImGui::End();

	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = CLEAR_SCENE;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}

	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	if (input_->PushAButton(joyState)) {
		sceneNo = SELECT_SCENE;
	}
}

void GameTitleScene::Draw(){
#pragma region 背景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);
	sprite_[2]->Draw(spriteTransform_[2], SpriteuvTransform_[2], spriteMaterial_[2]);
#pragma endregion

#pragma region 3Dオブジェクト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard3D);

#pragma endregion

#pragma region パーティクル描画
	CJEngine_->renderer_->Draw(PipelineType::Particle);

#pragma endregion

#pragma region 前景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);
	//Sprite描画
	sprite_[0]->Draw(spriteTransform_[0], SpriteuvTransform_[0], spriteMaterial_[0]);

	if (isSpriteDraw_[0]){
		sprite_[1]->Draw(spriteTransform_[1], SpriteuvTransform_[1], spriteMaterial_[1]);
	}

	sprite_[3]->Draw(spriteTransform_[3], SpriteuvTransform_[3], spriteMaterial_[3]);

	sprite_[4]->Draw(spriteTransform_[4], SpriteuvTransform_[4], spriteMaterial_[4]);

#pragma endregion
}

void GameTitleScene::Finalize() {
	
}