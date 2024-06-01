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

	selectData_ = audio_->SoundLoad("project/gamedata/resources/sounds/select.mp3");

	//テクスチャ
	spriteResource_[0] = textureManager_->Load("project/gamedata/resources/UI/bg.png");

	spriteResource_[1] = textureManager_->Load("project/gamedata/resources/UI/Select.png");

	spriteResource_[2] = textureManager_->Load("project/gamedata/resources/UI/Cursor.png");

	starResource_ = textureManager_->Load("project/gamedata/resources/UI/star.png");

	for (int i = 0; i < 3; i++)
	{
		spriteMaterial_[i] = { 1.0f,1.0f,1.0f,1.0f };
		spriteTransform_[i] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f}};

		SpriteuvTransform_[i] = {
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

	datas_ = Datas::GetInstance();
	datas_->Initialize();
}

void GameSelectScene::Update() {
	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(0, joyState);

	if (isFirstTransition)
	{
		starSpriteMaterial_ = { 0.0f,0.0f,0.0f,1.0f };
		starSpriteTransform_ = { {10.0f,10.0f,10.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f} };
		isTransitionStart_ = false;
		isTransitionEnd_ = false;
		isFirstTransition = false;
	}

	//タイトルに戻る処理(デバッグ用)
	if (input_->TriggerKey(DIK_L))
	{
		sceneNo = TITLE_SCENE;
		/*if (isTransitionEnd_)
		{
			isTransitionStart_ = true;
		}*/
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

		if (starSpriteTransform_.scale.num[0] >= 10.0f)
		{
			isTransitionStart_ = false;
			isFirstTransition = true;
			starSpriteTransform_.scale.num[0] = 10.0f;
			starSpriteTransform_.scale.num[1] = 10.0f;
			starSpriteMaterial_.num[3] = 1.0f;

			sceneNo = GAME_SCENE;
		}
	}

	if (isTransitionEnd_ && !isTransitionStart_)
	{
		//Selectのカーソル移動の処理
		if (input_->TriggerKey(DIK_A) && spriteTransform_[2].translate.num[0] == 1048.0f)
		{
			spriteTransform_[2].translate.num[0] = 640.0f;
		}

		if (input_->TriggerKey(DIK_A) && spriteTransform_[2].translate.num[0] == 1460.0f)
		{
			spriteTransform_[2].translate.num[0] = 1048.0f;
		}

		if (input_->TriggerKey(DIK_D) && spriteTransform_[2].translate.num[0] == 1048.0f)
		{
			spriteTransform_[2].translate.num[0] = 1460.0f;
		}

		if (input_->TriggerKey(DIK_D) && spriteTransform_[2].translate.num[0] == 640.0f)
		{
			spriteTransform_[2].translate.num[0] = 1048.0f;
		}

		if (input_->TriggerKey(DIK_S) && spriteTransform_[2].translate.num[1] == 360.0f)
		{
			spriteTransform_[2].translate.num[1] = 610.0f;
		}

		if (input_->TriggerKey(DIK_W) && spriteTransform_[2].translate.num[1] == 610.0f)
		{
			spriteTransform_[2].translate.num[1] = 360.0f;
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

	ImGui::Begin("debug");
	ImGui::Text("GameSelectScene");
	ImGui::SliderFloat3("SWTFT", &spriteTransform_[2].translate.num[0], 0.0f, 2280.0f);
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
			sprite_[i]->Draw(spriteTransform_[i], SpriteuvTransform_[i], spriteMaterial_[i]);
		}
	}

	starSprite_->Draw(starSpriteTransform_, starSpriteuvTransform_, starSpriteMaterial_);

#pragma endregion
}

void GameSelectScene::Finalize() {

}