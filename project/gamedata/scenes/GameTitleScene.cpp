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
	transitionResource_ = textureManager_->Load("project/gamedata/resources/UI/transitionStar.png");

	for (int i = 0; i < 3; i++)
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

	//トランジション用Sprite
	transitionSpriteMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
	transitionSpriteTransform_ = { {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f} };

	transitionSpriteuvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	transitionSprite_ = std::make_unique <CreateSprite>();

	transitionSprite_->Initialize(Vector2{ 1280.0f,720.0f }, transitionResource_);
	transitionSprite_->SetAnchor(Vector2{ 0.5f,0.5f });
}

void GameTitleScene::Update(){
	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(0, joyState);

	if (isGameStart_ == true) {//ゲーム開始時の処理
		
		isTransitionEnd_ = false;
		isGameStart_ = false;
	}

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

	//トランジション
	if (!isTransitionEnd_)
	{
		transitionSpriteTransform_.scale.num[0] -= 0.3f;
		transitionSpriteTransform_.scale.num[1] -= 0.3f;
		transitionSpriteMaterial_.num[3] -= 0.03f;

		if (transitionSpriteTransform_.scale.num[0] <= 0.0f && transitionSpriteTransform_.scale.num[1] <= 0.0f)
		{
			isTransitionEnd_ = true;
			transitionSpriteTransform_.scale = { 0.0f,0.0f,0.0f };
			transitionSpriteMaterial_ = { 1.0f,1.0f,1.0f,0.0f };
		}
	}

	if (isTransitionStart_)
	{
		transitionSpriteTransform_.scale.num[0] += 0.3f;
		transitionSpriteTransform_.scale.num[1] += 0.35f;
		transitionSpriteMaterial_.num[3] += 0.03f;

		if (transitionSpriteTransform_.scale.num[0] >= 9.0f && transitionSpriteTransform_.scale.num[1] >= 9.0f)
		{
			sceneNo = SELECT_SCENE;
			isTransitionStart_ = false;
			isGameStart_ = true;
			transitionSpriteTransform_.scale = { 9.0f,9.0f,9.0f };
			transitionSpriteMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
		}
	}

	ImGui::Begin("debug");
	ImGui::Text("GameTitleScene");
	ImGui::End();

	if (input_->TriggerKey(DIK_SPACE)) {
		if (isTransitionEnd_)
		{
			isTransitionStart_ = true;
			audio_->SoundPlayWave(selectData_, 0.1f, false);
		}
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

	transitionSprite_->Draw(transitionSpriteTransform_, transitionSpriteuvTransform_, transitionSpriteMaterial_);

#pragma endregion
}

void GameTitleScene::Finalize() {
	
}