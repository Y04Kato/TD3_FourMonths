#include"GameTitleScene.h"

bool GameTitleScene::isFirstTransition = false;

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
	starResource_ = textureManager_->Load("project/gamedata/resources/UI/star.png");

	testResource_ = textureManager_->Load("project/gamedata/resources/UI/star.png");

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

	starSpriteMaterial_ = { 0.0f,0.0f,0.0f,0.0f };
	starSpriteTransform_ = { {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f} };

	starSpriteuvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	starSprite_ = std::make_unique <CreateSprite>();

	starSprite_->Initialize(Vector2{ 512.0f,512.0f }, starResource_);
	starSprite_->SetAnchor(Vector2{ 0.5f,0.5f });

	//Test用
	testSpriteMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
	testSpriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f} };

	testSpriteuvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	testSprite_ = std::make_unique <CreateSprite>();

	testSprite_->Initialize(Vector2{ 64.0f,64.0f }, testResource_);
	testSprite_->SetAnchor(Vector2{ 0.5f,0.5f });
}

void GameTitleScene::Update(){
	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(0, joyState);

	if (input_->PressKey(DIK_L))
	{
		testSpriteTransform_.scale.num[0] += 0.04f;
		testSpriteTransform_.scale.num[1] += 0.04f;

		if (testSpriteTransform_.scale.num[0] >= 3.0f)
		{
			testSpriteTransform_.scale.num[0] = 3.0f;
		}
	}

	if (input_->PressKey(DIK_K))
	{
		testSpriteTransform_.scale.num[0] -= 0.06f;
		testSpriteTransform_.scale.num[1] -= 0.06f;

		testSpriteTransform_.rotate.num[1] += 0.6f;

		if (testSpriteTransform_.scale.num[0] <= 1.0f)
		{
			testSpriteTransform_.scale.num[0] = 1.0f;
		}
	}

	if (isFirstTransition)
	{
		starSpriteMaterial_ = { 0.0f,0.0f,0.0f,1.0f };
		starSpriteTransform_ = { {10.0f,10.0f,10.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f} };
		isTransitionStart_ = false;
		isTransitionEnd_ = false;
		isFirstTransition = false;
	}

	if (!isTransitionEnd_)
	{
		starSpriteMaterial_.num[3] -= 0.03f;

		if (starSpriteMaterial_.num[3] <= 0.0f)
		{
			isTransitionEnd_ = true;
			starSpriteTransform_.scale.num[0] = 0.0f;
			starSpriteTransform_.scale.num[1] = 0.0f;
			starSpriteMaterial_.num[3] = 0.0f;
		}
	}

	if (isTransitionStart_)
	{
		starSpriteTransform_.scale.num[0] += 0.4f;
		starSpriteTransform_.scale.num[1] += 0.4f;
		starSpriteMaterial_.num[3] += 0.03f;

		if (starSpriteMaterial_.num[3] >= 1.0f)
		{
			isTransitionStart_ = false;
			isFirstTransition = true;
			starSpriteTransform_.scale.num[0] = 10.0f;
			starSpriteTransform_.scale.num[1] = 10.0f;
			starSpriteMaterial_.num[3] = 1.0f;
			sceneNo = SELECT_SCENE;
		}
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

	sprite_[3]->Draw(spriteTransform_[3], SpriteuvTransform_[3], spriteMaterial_[3]);

	sprite_[4]->Draw(spriteTransform_[4], SpriteuvTransform_[4], spriteMaterial_[4]);

	starSprite_->Draw(starSpriteTransform_, starSpriteuvTransform_, starSpriteMaterial_);

	testSprite_->Draw(testSpriteTransform_, testSpriteuvTransform_, testSpriteMaterial_);

#pragma endregion
}

void GameTitleScene::Finalize() {
	
}