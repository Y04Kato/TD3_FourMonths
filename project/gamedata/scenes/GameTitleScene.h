#pragma once
#include "Iscene.h"
#include "CJEngine.h"
#include "Audio.h"
#include "Input.h"
#include "TextureManager.h"
#include "components/2d/CreateSprite.h"

class GameTitleScene :public Iscene{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	CitrusJunosEngine* CJEngine_;

	Input* input_ = nullptr;

	TextureManager* textureManager_;

	Audio* audio_;

	//audio
	SoundData bgmData_;
	SoundData selectData_;

	//sprite
	std::unique_ptr <CreateSprite> sprite_[2];
	EulerTransform spriteTransform_[2];
	EulerTransform SpriteuvTransform_[2];
	Vector4 spriteMaterial_[2];

	uint32_t titleResource_;
	uint32_t title1Resource_;

	bool isSpriteDraw_[2];

	int titleTimer_ = 60;
};
