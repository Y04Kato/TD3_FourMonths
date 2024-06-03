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
	static bool isFirstTransition;

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
	std::unique_ptr <CreateSprite> sprite_[5];
	EulerTransform spriteTransform_[5];
	EulerTransform SpriteuvTransform_[5];
	Vector4 spriteMaterial_[5];

	uint32_t titleResource_;
	uint32_t title1Resource_;
	uint32_t bgResource_;
	uint32_t annotationResource_;
	uint32_t escapeResource_;
	uint32_t starResource_;

	bool isSpriteDraw_[5];

	int titleTimer_ = 60;

	Skydome* skydome_ = nullptr;

	//Transition用のSprite
	std::unique_ptr <CreateSprite> starSprite_;
	EulerTransform starSpriteTransform_;
	EulerTransform starSpriteuvTransform_;
	Vector4 starSpriteMaterial_;

	bool isTransitionStart_ = false;
	bool isTransitionEnd_ = false;

	bool isGameStart_ = true;//ゲーム開始時に1回だけ呼ぶ
};
