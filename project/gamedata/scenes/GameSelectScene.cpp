#include "GameSelectScene.h"

int GameSelectScene::stageNum = 0;

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

	transitionResource_ = textureManager_->Load("project/gamedata/resources/UI/transitionStar.png");

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

	//トランジション用Sprite
	transitionSpriteMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
	transitionSpriteTransform_ = { {9.0f,9.0f,9.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f} };

	transitionSpriteuvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	transitionSprite_ = std::make_unique <CreateSprite>();

	transitionSprite_->Initialize(Vector2{ 1280.0f,720.0f }, transitionResource_);
	transitionSprite_->SetAnchor(Vector2{ 0.5f,0.5f });
}

void GameSelectScene::Update() {
	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(0, joyState);

	if (isGameStart_ == true) {//ゲーム開始時の処理

		isTransitionEnd_ = false;
		isGameStart_ = false;
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
			sceneNo = TITLE_SCENE;
			isTransitionStart_ = false;
			isGameStart_ = true;
			transitionSpriteTransform_.scale = { 9.0f,9.0f,9.0f };
			transitionSpriteMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
		}
	}

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
		isTransitionStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}

	//真ん中上
	if (spriteTransform_[2].translate.num[0] == 1048.0f && spriteTransform_[2].translate.num[1] == 360.0f
		&& input_->TriggerKey(DIK_SPACE))
	{
		stageNum = 2;
		isTransitionStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}

	//右上
	if (spriteTransform_[2].translate.num[0] == 1460.0f && spriteTransform_[2].translate.num[1] == 360.0f
		&& input_->TriggerKey(DIK_SPACE))
	{
		stageNum = 3;
		isTransitionStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}

	//左下
	if (spriteTransform_[2].translate.num[0] == 640.0f && spriteTransform_[2].translate.num[1] == 610.0f
		&& input_->TriggerKey(DIK_SPACE))
	{
		stageNum = 4;
		isTransitionStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}

	//真ん中下
	if (spriteTransform_[2].translate.num[0] == 1048.0f && spriteTransform_[2].translate.num[1] == 610.0f
		&& input_->TriggerKey(DIK_SPACE))
	{
		stageNum = 5;
		isTransitionStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
	}

	//右下
	if (spriteTransform_[2].translate.num[0] == 1460.0f && spriteTransform_[2].translate.num[1] == 610.0f
		&& input_->TriggerKey(DIK_SPACE))
	{
		stageNum = 6;
		isTransitionStart_ = true;
		audio_->SoundPlayWave(selectData_, 0.1f, false);
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

	transitionSprite_->Draw(transitionSpriteTransform_, transitionSpriteuvTransform_, transitionSpriteMaterial_);

#pragma endregion
}

void GameSelectScene::Finalize() {

}