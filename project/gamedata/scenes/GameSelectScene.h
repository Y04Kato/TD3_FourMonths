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
#include "datas/datas.h"

class GameSelectScene :public Iscene {
public:
	static int stageNum;

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
	SoundData selectData_;

	//Sprite
	std::unique_ptr <CreateSprite> sprite_[3];
	EulerTransform spriteTransform_[3];
	EulerTransform SpriteuvTransform_[3];
	Vector4 spriteMaterial_[3];

	uint32_t spriteResource_[3];

	bool isSpriteDraw_[3];

	int titleTimer_ = 60;

	//Datas
	Datas* datas_;

	//Transition用のSprite
	std::unique_ptr <CreateSprite> starSprite_;
	EulerTransform starSpriteTransform_;
	EulerTransform starSpriteuvTransform_;
	Vector4 starSpriteMaterial_;
	uint32_t starResource_;

	bool isTransitionStart_ = false;
	bool isTransitionEnd_ = false;
};

