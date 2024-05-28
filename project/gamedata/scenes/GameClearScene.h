#pragma once
#include "Iscene.h"
#include "CJEngine.h"
#include "Audio.h"
#include "Input.h"
#include "components/3d/WorldTransform.h"
#include "components/3d/ViewProjection.h"
#include "TextureManager.h"
#include "components/2d/CreateSprite.h"

#include "numbers/numbers.h"
#include "datas/datas.h"

class GameClearScene :public Iscene {
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
	SoundData selectData_;

	//Sprite
	std::unique_ptr <CreateSprite> sprite_[4];
	EulerTransform spriteTransform_[4];
	EulerTransform SpriteuvTransform_[4];
	Vector4 spriteMaterial_[4];

	uint32_t spriteResource_[4];

	bool isSpriteDraw_[4];

	//Timer
	std::unique_ptr<Numbers> numbers_;
	int nowTime_ = 0;
	EulerTransform numbersTransform_;

	//Datas
	Datas* datas_;

	bool isSceneStart_ = true;//シーン開始時に1回だけ呼ぶ
};
