#include "GameSelectScene.h"

int GameSelectScene::stageNum = 0;

bool GameSelectScene::isFirstTransition = false;

void GameSelectScene::Initialize() {
	//CJEngine
	CJEngine_ = CitrusJunosEngine::GetInstance();

	//Input
	input_ = Input::GetInstance();

	//TextureManager
	textureManager_ = TextureManager::GetInstance();

	//Audio
	audio_ = Audio::GetInstance();

	bgmData_ = audio_->SoundLoad("project/gamedata/resources/sounds/Title.mp3");
	selectData_ = audio_->SoundLoad("project/gamedata/resources/sounds/select.mp3");
	cursolData_ = audio_->SoundLoad("project/gamedata/resources/sounds/cursol.mp3");

	//テクスチャ
	spriteResource_[0] = textureManager_->Load("project/gamedata/resources/UI/bg.png");

	spriteResource_[1] = textureManager_->Load("project/gamedata/resources/UI/Select.png");

	spriteResource_[2] = textureManager_->Load("project/gamedata/resources/UI/Cursor.png");

	starResource_ = textureManager_->Load("project/gamedata/resources/UI/star.png");

	emptyStarTextureHandle_ = textureManager_->Load("project/gamedata/resources/UI/emptyStar.png");

	for (int i = 0; i < 3; i++)
	{
		spriteMaterial_[i] = { 1.0f,1.0f,1.0f,1.0f };
		spriteTransform_[i] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f}};

		spriteuvTransform_[i] = {
			{1.0f,1.0f,1.0f},
			{0.0f,0.0f,0.0f},
			{0.0f,0.0f,0.0f},
		};

		sprite_[i] = std::make_unique <CreateSprite>();
		isSpriteDraw_[i] = true;
	}

	sprite_[0]->Initialize(Vector2{ 1280.0f,720.0f }, spriteResource_[0]);
	sprite_[0]->SetAnchor(Vector2{ 0.5f,0.5f });

	sprite_[1]->Initialize(Vector2{ 1280.0f,720.0f }, spriteResource_[1]);
	sprite_[1]->SetAnchor(Vector2{ 0.5f,0.5f });

	sprite_[2]->Initialize(Vector2{ 1280.0f,720.0f }, spriteResource_[2]);
	sprite_[2]->SetAnchor(Vector2{ 0.5f,0.5f });

	//Transition
	transitionSpriteMaterial_ = { 0.0f,0.0f,0.0f,0.0f };
	transitionSpriteTransform_ = { {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f} };

	transitionSpriteuvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	transitionSprite_ = std::make_unique <CreateSprite>();

	transitionSprite_->Initialize(Vector2{ 512.0f,512.0f }, starResource_);
	transitionSprite_->SetAnchor(Vector2{ 0.5f,0.5f });


	//星
	for (uint32_t index0 = 0; index0 < 6; index0++) {
		starSprites_[index0] = std::make_unique<SpriteData>();
		starSprites_[index0]->starTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{270.0f, 190.0f,0.0f} };
		emptyStarSprites_[index0] = std::make_unique<SpriteData>();
		emptyStarSprites_[index0]->starTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{270.0f, 190.0f,0.0f} };
		for (uint32_t index1 = 0; index1 < 3; index1++) {
			starSprites_[index0]->Sprite_[index1] = std::make_unique<CreateSprite>();
			starSprites_[index0]->Sprite_[index1]->Initialize({ starTextureSize_, starTextureSize_ }, starResource_);
			starSprites_[index0]->Sprite_[index1]->SetAnchor({ 0.5f, 0.5f });
			emptyStarSprites_[index0]->Sprite_[index1] = std::make_unique<CreateSprite>();
			emptyStarSprites_[index0]->Sprite_[index1]->Initialize({ emptyStarTextureSize_, emptyStarTextureSize_ }, emptyStarTextureHandle_);
			emptyStarSprites_[index0]->Sprite_[index1]->SetAnchor({ 0.5f, 0.5f });

		}
	}


	datas_ = Datas::GetInstance();
	datas_->Initialize();

}

void GameSelectScene::Update() {

	if (isGameStart_ == true) {//ゲーム開始時の処理
		audio_->SoundPlayWave(bgmData_, 0.1f, true);
		isGameStart_ = false;
	}

	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(0, joyState);

	if (isFirstTransition)
	{
		spriteTransform_[2].translate = { 1280 / 2.0f,720 / 2.0f,0.0f };
		transitionSpriteMaterial_ = { 0.0f,0.0f,0.0f,1.0f };
		transitionSpriteTransform_ = { {10.0f,10.0f,10.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f} };
		isTransitionStart_ = false;
		isTransitionEnd_ = false;
		isFirstTransition = false;
	}

	//タイトルに戻る処理(デバッグ用)
	if (input_->TriggerKey(DIK_L))
	{
		sceneNo = TITLE_SCENE;
		audio_->SoundStopWave(&bgmData_);
		isGameStart_ = true;
		datas_->Initialize();
		/*if (isTransitionEnd_)
		{
			isTransitionStart_ = true;
		}*/
	}

	if (!isTransitionEnd_)
	{
		transitionSpriteMaterial_.num[3] -= 0.03f;
		
		if (transitionSpriteMaterial_.num[3] <= 0.0f)
		{
			isTransitionEnd_ = true;
			transitionSpriteTransform_.scale.num[0] = 0.0f;
			transitionSpriteTransform_.scale.num[1] = 0.0f;
			transitionSpriteMaterial_.num[3] = 0.0f;
		}
	}

	if (isTransitionStart_)
	{
		transitionSpriteTransform_.scale.num[0] += 0.4f;
		transitionSpriteTransform_.scale.num[1] += 0.4f;
		transitionSpriteMaterial_.num[3] += 0.03f;

		if (transitionSpriteTransform_.scale.num[0] >= 10.0f)
		{
			isTransitionStart_ = false;
			isFirstTransition = true;
			transitionSpriteTransform_.scale.num[0] = 10.0f;
			transitionSpriteTransform_.scale.num[1] = 10.0f;
			transitionSpriteMaterial_.num[3] = 1.0f;

			sceneNo = GAME_SCENE;
			isGameStart_ = true;
			audio_->SoundStopWave(&bgmData_);
		}
	}

	if (isTransitionEnd_ && !isTransitionStart_)
	{
		//Selectのカーソル移動の処理
		if (input_->TriggerKey(DIK_A) && spriteTransform_[2].translate.num[0] == 1048.0f)
		{
			spriteTransform_[2].translate.num[0] = 640.0f;
			audio_->SoundPlayWave(cursolData_, 0.1f, false);
		}

		if (input_->TriggerKey(DIK_A) && spriteTransform_[2].translate.num[0] == 1460.0f)
		{
			spriteTransform_[2].translate.num[0] = 1048.0f;
			audio_->SoundPlayWave(cursolData_, 0.1f, false);
		}

		if (input_->TriggerKey(DIK_D) && spriteTransform_[2].translate.num[0] == 1048.0f)
		{
			spriteTransform_[2].translate.num[0] = 1460.0f;
			audio_->SoundPlayWave(cursolData_, 0.1f, false);
		}

		if (input_->TriggerKey(DIK_D) && spriteTransform_[2].translate.num[0] == 640.0f)
		{
			spriteTransform_[2].translate.num[0] = 1048.0f;
			audio_->SoundPlayWave(cursolData_, 0.1f, false);
		}

		if (input_->TriggerKey(DIK_S) && spriteTransform_[2].translate.num[1] == 360.0f)
		{
			spriteTransform_[2].translate.num[1] = 610.0f;
			audio_->SoundPlayWave(cursolData_, 0.1f, false);
		}

		if (input_->TriggerKey(DIK_W) && spriteTransform_[2].translate.num[1] == 610.0f)
		{
			spriteTransform_[2].translate.num[1] = 360.0f;
			audio_->SoundPlayWave(cursolData_, 0.1f, false);
		}

		//ステージ番号
		//左上
		if (spriteTransform_[2].translate.num[0] == 640.0f && spriteTransform_[2].translate.num[1] == 360.0f
			&& input_->TriggerKey(DIK_SPACE))
		{
			stageNum = 1;
			datas_->SetStageNum(stageNum);

			isTransitionStart_ = true;
			audio_->SoundPlayWave(selectData_, 0.1f, false);
		}

		//真ん中上
		if (spriteTransform_[2].translate.num[0] == 1048.0f && spriteTransform_[2].translate.num[1] == 360.0f
			&& input_->TriggerKey(DIK_SPACE))
		{
			stageNum = 2;
			datas_->SetStageNum(stageNum);

			isTransitionStart_ = true;
			audio_->SoundPlayWave(selectData_, 0.1f, false);
		}

		//右上
		if (spriteTransform_[2].translate.num[0] == 1460.0f && spriteTransform_[2].translate.num[1] == 360.0f
			&& input_->TriggerKey(DIK_SPACE))
		{
			stageNum = 3;
			datas_->SetStageNum(stageNum);

			isTransitionStart_ = true;
			audio_->SoundPlayWave(selectData_, 0.1f, false);
		}

		//左下
		if (spriteTransform_[2].translate.num[0] == 640.0f && spriteTransform_[2].translate.num[1] == 610.0f
			&& input_->TriggerKey(DIK_SPACE))
		{
			stageNum = 4;
			datas_->SetStageNum(stageNum);

			isTransitionStart_ = true;
			audio_->SoundPlayWave(selectData_, 0.1f, false);
		}

		//真ん中下
		if (spriteTransform_[2].translate.num[0] == 1048.0f && spriteTransform_[2].translate.num[1] == 610.0f
			&& input_->TriggerKey(DIK_SPACE))
		{
			stageNum = 5;
			datas_->SetStageNum(stageNum);

			isTransitionStart_ = true;
			audio_->SoundPlayWave(selectData_, 0.1f, false);
		}

		//右下
		if (spriteTransform_[2].translate.num[0] == 1460.0f && spriteTransform_[2].translate.num[1] == 610.0f
			&& input_->TriggerKey(DIK_SPACE))
		{
			stageNum = 6;
			datas_->SetStageNum(stageNum);

			isTransitionStart_ = true;
			audio_->SoundPlayWave(selectData_, 0.1f, false);
		}
	}

	/*if (input_->TriggerKey(DIK_7)) {
		for (int i = 0; i < 6; i++) {
			datas_->SetStarsEarned(i, 1.0f, false, 1.0f);
		}
	}

	if (input_->TriggerKey(DIK_8)) {
		for (int i = 0; i < 6; i++) {
			datas_->SetStarsEarned(i, true, true, false);
		}
	}

	if (input_->TriggerKey(DIK_9)) {
		for (int i = 0; i < 6; i++) {
			datas_->SetStarsEarned(i, false, true, true);
		}
	}*/


	ImGui::Begin("debug");
	ImGui::Text("GameSelectScene");
	ImGui::SliderFloat3("SWTFT", &spriteTransform_[2].translate.num[0], 0.0f, 2280.0f);
	ImGui::DragFloat3("Star", emptyStarSprites_[0]->starTransform_.translate.num, 0.0f, 2280.0f);
	ImGui::End();

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

	for (int i = 0; i < 3; i++)
	{
		if (isSpriteDraw_[i])
		{
			//Sprite描画
			sprite_[i]->Draw(spriteTransform_[i], spriteuvTransform_[i], spriteMaterial_[i]);
		}
	}

	transitionSprite_->Draw(transitionSpriteTransform_, transitionSpriteuvTransform_, transitionSpriteMaterial_);

	for (uint32_t index0 = 0; index0 < 6; index0++) {
		for (uint32_t index1 = 0; index1 < 3; index1++) {
			EulerTransform emptyTransform = emptyStarSprites_[index0]->starTransform_;
			emptyTransform.translate.num[0] += 410.0f * (index0 % 3);
			emptyTransform.translate.num[1] += 250.0f * (index0 / 3);
			emptyTransform.translate.num[0] += emptyStarTextureSize_ * index1;
			emptyStarSprites_[index0]->Sprite_[index1]->Draw(emptyTransform, spriteTransform_[0], {1.0f, 1.0f, 1.0f, 1.0f});

			EulerTransform transform = starSprites_[index0]->starTransform_;
			transform.translate.num[0] += 410.0f * (index0 % 3);
			transform.translate.num[1] += 250.0f * (index0 / 3);
			transform.translate.num[0] += starTextureSize_ * index1;
			if (datas_->GetStarsEarned()[index0].item && index1 == 0) {
				starSprites_[index0]->Sprite_[0]->Draw(transform, spriteTransform_[0], { 1.0f, 1.0f, 1.0f, 1.0f });
			}
			if (datas_->GetStarsEarned()[index0].time && index1 == 1) {
				starSprites_[index0]->Sprite_[1]->Draw(transform, spriteTransform_[0], { 1.0f, 1.0f, 1.0f, 1.0f });
			}
			if (datas_->GetStarsEarned()[index0].hit && index1 == 2) {
				starSprites_[index0]->Sprite_[2]->Draw(transform, spriteTransform_[0], { 1.0f, 1.0f, 1.0f, 1.0f });
			}
		}
	}

#pragma endregion
}

void GameSelectScene::Finalize() {

}