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
	struct Target {
		int item;
		float time;
	};

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
	SoundData cursolData_;

	//Sprite
	std::unique_ptr <CreateSprite> sprite_[7];
	uint32_t spriteResource_[7];
	EulerTransform spriteTransform_[7];
	EulerTransform SpriteuvTransform_[7];
	Vector4 spriteMaterial_[7];
	// 星のスプライト
	std::unique_ptr <CreateSprite> starSprite_[3];
	uint32_t starTextureHandle_;
	EulerTransform starTransform_;
	const float starTextureSize_ = 96.0f;

	std::unique_ptr <CreateSprite> emptyStarSprite_[3];
	uint32_t emptyStarTextureHandle_;
	EulerTransform emptyStarTransform_;
	const float emptyStarTextureSize_ = 97.0f;

	bool isSpriteDraw_[4];

	// ステージごとの目標
	Target targets_[6];
	// 達成数
	Vector3 achievement_;

	// 目標アイテム数
	std::unique_ptr<Numbers> targetItemNumbers_;
	EulerTransform targetItemNumbersTransform_;
	// 獲得アイテム
	std::unique_ptr<Numbers> getItemNumbers_;
	EulerTransform getItemNumbersTransform_;

	// 
	uint32_t fractionTextureHandle_;
	std::unique_ptr<CreateSprite> fractionSprites_[2];
	EulerTransform fractionTransform_[2];

	//int nowTime_ = 0;
	// 目標タイム
	std::unique_ptr<Timer> targetTime_;
	EulerTransform targetTimeTransform_;
	
	
	// クリアタイム
	std::unique_ptr<Timer> time_;
	float nowTime_ = 0.0f;
	EulerTransform timeTransform_;
	// 落ちる
	std::unique_ptr<Numbers> fall_;

	//Datas
	Datas* datas_;

	bool isSceneStart_ = true;//シーン開始時に1回だけ呼ぶ

	//Transition用のSprite
	std::unique_ptr <CreateSprite> transitionSprite_;
	EulerTransform transitionSpriteTransform_;
	EulerTransform transitionSpriteuvTransform_;
	Vector4 transitionSpriteMaterial_;
	uint32_t transitionResource_;

	bool isTransitionStart_ = false;
	bool isTransitionEnd_ = false;
};
