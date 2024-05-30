#pragma once
#include "Iscene.h"
#include "CJEngine.h"
#include "Audio.h"
#include "Input.h"
#include "components/3d/WorldTransform.h"
#include "components/3d/ViewProjection.h"
#include "TextureManager.h"
#include "components/2d/CreateSprite.h"

#include "skydome/Skydome.h"

class GameTitleScene :public Iscene{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	CitrusJunosEngine* CJEngine_;

	Input* input_ = nullptr;

	ViewProjection viewProjection_;

	TextureManager* textureManager_;

	Audio* audio_;

	//Audio
	SoundData bgmData_;
	SoundData selectData_;

	//Sprite
	std::unique_ptr <CreateSprite> sprite_[3];
	EulerTransform spriteTransform_[3];
	EulerTransform SpriteuvTransform_[3];
	Vector4 spriteMaterial_[3];

	uint32_t titleResource_;
	uint32_t title1Resource_;
	uint32_t bgResource_;

	bool isSpriteDraw_[3];

	int titleTimer_ = 60;

	Skydome* skydome_ = nullptr;

	//トランジション用Sprite
	std::unique_ptr<CreateSprite> transitionSprite_ = nullptr;
	EulerTransform transitionSpriteTransform_;
	EulerTransform transitionSpriteuvTransform_;
	Vector4 transitionSpriteMaterial_;
	uint32_t transitionResource_;
	bool isTransitionStart_ = false;
	bool isTransitionEnd_ = false;

	bool isGameStart_ = true;//ゲーム開始時に1回だけ呼ぶ
};
