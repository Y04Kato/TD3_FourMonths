#pragma once

#include <stdint.h>
#include <array>
#include <list>
#include <memory>
#include "TextureManager.h"
#include "components/2d/CreateSprite.h"


class Timer {
public:
	void Initialize();
	void Draw();

	void SetNum(float nowNum) { numFloat_ = nowNum; }
	void SetInitialNum(int initialNum) { initialNum_ = initialNum; };
	void SetSpace(int space) { space_ = space; };
	void SetColor(Vector4 color) { color_ = color; };

	void SetTransform(EulerTransform transform);

private:

	float numFloat_;
	//初期数字
	int32_t initialNum_;

	//桁数
	static const int32_t kDigits = 4;
	// 小数点
	static const int32_t kDecimalPoint = 2;
	// 加算秒数
	static const int32_t kPlusSecond = 3;

	EulerTransform transform_[kDigits];
	EulerTransform decimalPosintTransform_;

	//開始位置
	const int32_t kLeft = 140;
	const int32_t kTop = 30;

	//一桁ごとのサイズ
	const int32_t kWidth = 64;
	const int32_t kHeight = 64;
	//桁ごとの空間
	int32_t space_ = -20;

	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };

	// 数字のスプライト
	std::array<std::unique_ptr<CreateSprite>, kDigits> Sprites_;
	uint32_t numberTextureHandle_ = 0;
	// 小数点のスプライト
	std::unique_ptr<CreateSprite> sprite_;
	uint32_t decimalPointTextureHandle_ = 0;
	// プラスのスプライト
	std::array<std::unique_ptr<CreateSprite>, kPlusSecond> plusSprites_;
	uint32_t plusTextureHandle_ = 0;

};