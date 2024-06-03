#include"GameClearScene.h"
#include "GameSelectScene.h"

bool GameClearScene::isFirstTransition = false;

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
	spriteResource_[2] = textureManager_->Load("project/gamedata/resources/UI/GoalTab.png");
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

	spriteTransform_[1].translate.num[0] = 580.0f;
	spriteTransform_[1].translate.num[1] = 385.0f;

	starTextureHandle_ = textureManager_->Load("project/gamedata/resources/UI/star.png");
	for (uint32_t index = 0; index < 3; index++) {
		starSprite_[index] = std::make_unique<CreateSprite>();
		starSprite_[index]->Initialize({ starTextureSize_, starTextureSize_ }, starTextureHandle_);
		starSprite_[index]->SetAnchor(Vector2{ 0.5f, 0.5f });
	}
	starTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{849.0f, 185.0f,0.0f} };

	emptyStarTextureHandle_ = textureManager_->Load("project/gamedata/resources/UI/emptyStar.png");
	for (uint32_t index = 0; index < 3; index++) {
		emptyStarSprite_[index] = std::make_unique<CreateSprite>();
		emptyStarSprite_[index]->Initialize({ emptyStarTextureSize_, emptyStarTextureSize_ }, emptyStarTextureHandle_);
		emptyStarSprite_[index]->SetAnchor(Vector2{ 0.5f, 0.5f });
	}
	emptyStarTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{500.0f, 90.0f,0.0f} };

	targetItemNumbers_ = std::make_unique<Numbers>();
	targetItemNumbers_->Initialize();
	targetItemNumbers_->SetInitialNum(0 / 60);
	targetItemNumbersTransform_ = { {1.0f, 1.0f, 1.0f}, { 0.0f,0.0f,0.0f}, {580.0f,165.0f,0.0f} };

	getItemNumbers_ = std::make_unique<Numbers>();
	getItemNumbers_->Initialize();
	getItemNumbers_->SetInitialNum(0 / 60);
	getItemNumbersTransform_ = { {1.0f, 1.0f, 1.0f}, { 0.0f,0.0f,0.0f}, {500.0f,165.0f,0.0f} };

	fractionTextureHandle_ = textureManager_->Load("project/gamedata/resources/fraction.png");
	for (int32_t index = 0; index < 2; index++) {
		fractionSprites_[index] = std::make_unique<CreateSprite>();
		fractionSprites_[index]->Initialize({ 48.0f, 48.0f }, fractionTextureHandle_);
		fractionSprites_[index]->SetAnchor({ 0.5f, 0.5f });
	}
	fractionTransform_[0] = { {1.0f, 1.0f ,1.0f}, {0.0f, 0.0f, 0.0f}, {715.0f,190.0f,0.0f} };
	fractionTransform_[1] = { {1.0f, 1.0f ,1.0f}, {0.0f, 0.0f, 0.0f}, {830.0f,285.0f,0.0f} };

	// 目標タイム
	targetTime_ = std::make_unique<Timer>();
	targetTime_->Initialize();
	targetTime_->SetInitialNum(0 / 60);
	targetTimeTransform_ = { {1.0f, 1.0f, 1.0f}, { 0.0f,0.0f,0.0f}, {700.0f,260.0f,0.0f} };

	//クリアタイム
	time_ = std::make_unique<Timer>();
	time_->Initialize();
	time_->SetInitialNum(0 / 60);
	timeTransform_ = { {1.0f, 1.0f, 1.0f}, { 0.0f,0.0f,0.0f}, {480.0f,260.0f,0.0f} };

	//Transition
	transitionSpriteMaterial_ = { 0.0f,0.0f,0.0f,0.0f };
	transitionSpriteTransform_ = { {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f} };

	transitionSpriteuvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	transitionSprite_ = std::make_unique <CreateSprite>();

	transitionSprite_->Initialize(Vector2{ 512.0f,512.0f }, starTextureHandle_);
	transitionSprite_->SetAnchor(Vector2{ 0.5f,0.5f });

	//Datas
	datas_ = Datas::GetInstance();

	targets_[0] = {};
	targets_[1] = {};
	targets_[2] = {};
	targets_[3] = {};
	targets_[4] = {};
	targets_[5] = {};
}

void GameClearScene::Update() {
	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(0, joyState);

	if (isSceneStart_ == true) {//ゲーム開始時の処理
		spriteTransform_[3].translate.num[0] = 1061.0f;

		spriteTransform_[3].translate.num[1] = 690.0f;

		achievement_ = { 0, 0, 0 };

		if (datas_->GetItem() < 10) {
			getItemNumbers_->SetDrawCount(1);
		}

		targetItemNumbers_->SetNum(1);
		targetItemNumbers_->SetDrawCount(1);
		targetItemNumbers_->SetTransform(targetItemNumbersTransform_);

		getItemNumbers_->SetNum(datas_->GetItem());
		getItemNumbers_->SetTransform(getItemNumbersTransform_);

		isSceneStart_ = false;
	}
	targetItemNumbers_->SetTransform(targetItemNumbersTransform_);
	getItemNumbers_->SetTransform(getItemNumbersTransform_);

	targets_[datas_->GetStageNum()].item = datas_->GetMaxItem();
	targets_[datas_->GetStageNum()].time = (float)datas_->GetClearResultTimeNum();

	if (datas_->GetItem() == targets_[datas_->GetStageNum()].item) {
		achievement_.num[0] = 1.0f;
	}
	if (datas_->GetClearTime() <= targets_[datas_->GetStageNum()].time) {
		achievement_.num[1] = 1.0f;
	}
	if (!datas_->GetFell()) {
		achievement_.num[2] = 1.0f;
	}

	//2回目以降のTransitionの初期化処理
	if (isFirstTransition)
	{
		transitionSpriteMaterial_ = { 0.0f,0.0f,0.0f,1.0f };
		transitionSpriteTransform_ = { {10.0f,10.0f,10.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f} };
		isTransitionStart_ = false;
		isTransitionEnd_ = false;
		isFirstTransition = false;
	}

	//Transition
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

		if (spriteTransform_[3].translate.num[0] == 678.0f && transitionSpriteMaterial_.num[3] >= 1.0f)
		{
			isTransitionStart_ = false;
			isFirstTransition = true;
			transitionSpriteTransform_.scale.num[0] = 10.0f;
			transitionSpriteTransform_.scale.num[1] = 10.0f;
			transitionSpriteMaterial_.num[3] = 1.0f;

			sceneNo = GAME_SCENE;
			isSceneStart_ = true;
		}
		else if (transitionSpriteMaterial_.num[3] >= 1.0f && spriteTransform_[3].translate.num[0] == 1061.0f &&
			datas_->GetStageNum() == 1)
		{
			isTransitionStart_ = false;
			isFirstTransition = true;
			transitionSpriteTransform_.scale.num[0] = 10.0f;
			transitionSpriteTransform_.scale.num[1] = 10.0f;
			transitionSpriteMaterial_.num[3] = 1.0f;

			sceneNo = GAME_SCENE;
			datas_->SetStageNum(2);
			isSceneStart_ = true;
		}
		else if (transitionSpriteMaterial_.num[3] >= 1.0f && spriteTransform_[3].translate.num[0] == 1061.0f &&
			datas_->GetStageNum() == 2)
		{
			isTransitionStart_ = false;
			isFirstTransition = true;
			transitionSpriteTransform_.scale.num[0] = 10.0f;
			transitionSpriteTransform_.scale.num[1] = 10.0f;
			transitionSpriteMaterial_.num[3] = 1.0f;

			sceneNo = GAME_SCENE;
			datas_->SetStageNum(3);
			isSceneStart_ = true;
		}
		else if (transitionSpriteMaterial_.num[3] >= 1.0f && spriteTransform_[3].translate.num[0] == 1061.0f &&
			datas_->GetStageNum() == 3)
		{
			isTransitionStart_ = false;
			isFirstTransition = true;
			transitionSpriteTransform_.scale.num[0] = 10.0f;
			transitionSpriteTransform_.scale.num[1] = 10.0f;
			transitionSpriteMaterial_.num[3] = 1.0f;

			sceneNo = GAME_SCENE;
			datas_->SetStageNum(4);
			isSceneStart_ = true;
		}
		else if (transitionSpriteMaterial_.num[3] >= 1.0f && spriteTransform_[3].translate.num[0] == 1061.0f &&
			datas_->GetStageNum() == 4)
		{
			isTransitionStart_ = false;
			isFirstTransition = true;
			transitionSpriteTransform_.scale.num[0] = 10.0f;
			transitionSpriteTransform_.scale.num[1] = 10.0f;
			transitionSpriteMaterial_.num[3] = 1.0f;

			sceneNo = GAME_SCENE;
			datas_->SetStageNum(5);
			isSceneStart_ = true;
		}
		else if (transitionSpriteMaterial_.num[3] >= 1.0f && spriteTransform_[3].translate.num[0] == 1061.0f &&
			datas_->GetStageNum() == 5)
		{
			isTransitionStart_ = false;
			isFirstTransition = true;
			transitionSpriteTransform_.scale.num[0] = 10.0f;
			transitionSpriteTransform_.scale.num[1] = 10.0f;
			transitionSpriteMaterial_.num[3] = 1.0f;

			sceneNo = GAME_SCENE;
			datas_->SetStageNum(6);
			isSceneStart_ = true;
		}
		else if (transitionSpriteMaterial_.num[3] >= 1.0f && spriteTransform_[3].translate.num[0] == 1061.0f &&
			datas_->GetStageNum() == 6)
		{
			isTransitionStart_ = false;
			isFirstTransition = true;
			transitionSpriteTransform_.scale.num[0] = 10.0f;
			transitionSpriteTransform_.scale.num[1] = 10.0f;
			transitionSpriteMaterial_.num[3] = 1.0f;

			sceneNo = TITLE_SCENE;
			isSceneStart_ = true;
		}
		else if (transitionSpriteMaterial_.num[3] >= 1.0f && spriteTransform_[3].translate.num[0] == 1440.0f)
		{
			isTransitionStart_ = false;
			isFirstTransition = true;
			transitionSpriteTransform_.scale.num[0] = 10.0f;
			transitionSpriteTransform_.scale.num[1] = 10.0f;
			transitionSpriteMaterial_.num[3] = 1.0f;

			sceneNo = SELECT_SCENE;
			isSceneStart_ = true;
		}

	}

	/*numbers_->SetNum(datas_->GetClearTime() / 60);
	numbers_->SetTransform(numbersTransform_);*/
  
	targetTime_->SetNum(targets_[datas_->GetStageNum()].time);
	targetTime_->SetTransform(targetTimeTransform_);
	time_->SetNum(datas_->GetClearTime());
	time_->SetTransform(timeTransform_);

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
	ImGui::DragFloat3("Result", &spriteTransform_[1].translate.num[0], 0.0f, 2280.0f);
	ImGui::DragFloat3("star", starTransform_.translate.num, 0.0f, 2280.0f);
	ImGui::DragFloat3("TargetItem", targetItemNumbersTransform_.translate.num, 0.0f, 2280.0f);
	ImGui::DragFloat3("GetItem", getItemNumbersTransform_.translate.num, 0.0f, 2280.0f);
	ImGui::DragFloat3("time", timeTransform_.translate.num, 0.0f, 2280.0f);
	ImGui::DragFloat3("fraction", fractionTransform_[1].translate.num, 0.0f, 2280.0f);
	ImGui::DragFloat3("empty", emptyStarTransform_.translate.num, 0.0f, 2280.0f);
	ImGui::DragFloat3("targetTime", targetTimeTransform_.translate.num, 0.0f, 2280.0f);
	ImGui::End();

	if (!isTransitionStart_ && isTransitionEnd_)
	{
		//左側(現在のステージを繰り返し)
		if (spriteTransform_[3].translate.num[0] == 678.0f && input_->TriggerKey(DIK_SPACE)) {
			isTransitionStart_ = true;
			audio_->SoundPlayWave(selectData_, 0.1f, false);
		}

		//真ん中(次のステージ)
		if (spriteTransform_[3].translate.num[0] == 1061.0f && input_->TriggerKey(DIK_SPACE)) {

			isTransitionStart_ = true;
			audio_->SoundPlayWave(selectData_, 0.1f, false);
		}

		//右側(ステージセレクト)
		if (spriteTransform_[3].translate.num[0] == 1440.0f && input_->TriggerKey(DIK_SPACE)) {
			isTransitionStart_ = true;
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

	for (uint32_t index = 0; index < 3; index++) {
		EulerTransform emptyTransform = emptyStarTransform_;
		emptyTransform.translate.num[0] += (emptyStarTextureSize_ + 70.0f) * index;
		emptyStarSprite_[index]->Draw(emptyTransform, spriteTransform_[0], { 1.0f, 1.0f, 1.0f, 1.0f });

		if (achievement_.num[index] > 0.0f) {
			EulerTransform transform = starTransform_;
			switch (index)
			{
			case 0:
				transform.translate.num[1] += starTextureSize_ * index;
				break;
			case 1:
				transform.translate.num[1] += starTextureSize_ * index - 5.0f;
				break;
			case 2:
				transform.translate.num[1] += starTextureSize_ * index - 4.5f;
				break;
			}
			starSprite_[index]->Draw(emptyTransform, SpriteuvTransform_[0], { 1.0f, 1.0f, 1.0f, 1.0f });
		}
	}

	targetItemNumbers_->Draw();
	for (uint32_t index = 0; index < 2; index++) {
		fractionSprites_[index]->Draw(fractionTransform_[index], SpriteuvTransform_[0], { 1.0f, 1.0f, 1.0f, 1.0f });
	}
	getItemNumbers_->Draw();

	time_->Draw();
	targetTime_->Draw();
  
    transitionSprite_->Draw(transitionSpriteTransform_, transitionSpriteuvTransform_, transitionSpriteMaterial_);
  
#pragma endregion
}

void GameClearScene::Finalize() {

}