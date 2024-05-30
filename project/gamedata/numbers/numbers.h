#pragma once
#include <stdint.h>
#include <array>
#include <list>
#include <memory>
#include "TextureManager.h"
#include "components/2d/CreateSprite.h"

class Numbers {
public:
	void Initialize();
	void Draw();

	void SetNum(int nowNum) { num_ = nowNum; };
	void SetInitialNum(int initialNum) { initialNum_ = initialNum; };
	void SetSpace(int space) { space_ = space; };
	void SetColor(Vector4 color) { color_ = color; };

	void SetTransform(EulerTransform transform);

private:

	int32_t num_;
	//初期数字
	int32_t initialNum_;

	//桁数
	static const int32_t kDigits = 3;

	EulerTransform transform_[kDigits];

	//開始位置
	const int32_t kLeft = 140;
	const int32_t kTop = 30;

	//一桁ごとのサイズ
	const int32_t kWidth = 64;
	const int32_t kHeight = 64;
	//桁ごとの空間
	int32_t space_ = -20;

	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };

	std::array<std::unique_ptr<CreateSprite>, kDigits> Sprites_;
	uint32_t numberTextureHandle_ = 0;
};