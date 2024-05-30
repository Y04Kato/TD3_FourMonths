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
#include "timer/Timer.h"

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
	uint32_t spriteResource_[4];
	EulerTransform spriteTransform_[4];
	EulerTransform SpriteuvTransform_[4];
	Vector4 spriteMaterial_[4];
	// 星のスプライト
	std::unique_ptr <CreateSprite> starSprite_[3];
	uint32_t starTextureHandle_;
	EulerTransform starTransform_;
	const float starTextureSize_ = 96.0f;

	bool isSpriteDraw_[4];

	// 達成数
	uint32_t achievementCount_;

	// アイテム
	std::unique_ptr<Numbers> itemNumbers_;
	EulerTransform itemNumbersTransform_;
	//int nowTime_ = 0;
	// クリアタイム
	std::unique_ptr<Timer> timer_;
	float nowTime_ = 0.0f;
	EulerTransform timerTransform_;
	// 落ちる
	std::unique_ptr<Numbers> fall_;

	//Datas
	Datas* datas_;

	bool isSceneStart_ = true;//シーン開始時に1回だけ呼ぶ
};
