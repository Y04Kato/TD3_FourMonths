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
	struct SpriteData {
		std::unique_ptr <CreateSprite> Sprite_[3];
		EulerTransform starTransform_;
	};


	static int stageNum;

	static bool isFirstTransition;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void DrawUI() override;
	void DrawPostEffect() override;
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
	std::unique_ptr <CreateSprite> sprite_[3];
	EulerTransform spriteTransform_[3];
	EulerTransform spriteuvTransform_[3];
	Vector4 spriteMaterial_[3];

	uint32_t spriteResource_[3];

	bool isSpriteDraw_[3];

	int titleTimer_ = 60;

	//Datas
	Datas* datas_;

	//Transition用のSprite
	std::unique_ptr <CreateSprite> transitionSprite_;
	EulerTransform transitionSpriteTransform_;
	EulerTransform transitionSpriteuvTransform_;
	Vector4 transitionSpriteMaterial_;

	bool isTransitionStart_ = false;
	bool isTransitionEnd_ = false;

	// 星のスプライト
	uint32_t starResource_;
	uint32_t emptyStarTextureHandle_;
	std::unique_ptr<SpriteData> starSprites_[6];
	std::unique_ptr<SpriteData> emptyStarSprites_[6];
	const float starTextureSize_ = 40.0f;
	const float emptyStarTextureSize_ = 40.0f;
	bool isGameStart_ = true;//ゲーム開始時に1回だけ呼ぶ
};

