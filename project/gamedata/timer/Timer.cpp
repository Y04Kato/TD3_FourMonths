#include "Timer.h"

void Timer::Initialize() {
	numberTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/Numbers.png");
	for (int index = 0; index < kDigits; index++) {
		Sprites_[index].reset(new CreateSprite());
		Sprites_[index]->Initialize(Vector2{ 64.0f,640.0f }, numberTextureHandle_);
		Sprites_[index]->SetTextureInitialSize();
		Sprites_[index]->SetFlip(false, false);
	}
	decimalPointTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/DecimalPoint.png");
	sprite_.reset(new CreateSprite());
	sprite_->Initialize(Vector2{ 64.0f, 64.0f }, decimalPointTextureHandle_);
	sprite_->SetTextureInitialSize();
	sprite_->SetFlip(false, false);
	plusTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/DecimalPoint.png");
	/*for (int index = 0; index < ; index++) {
		plusSprites_[index].reset(new CreateSprite());
		plusSprites_[index]->Initialize(Vector2{ 64.0f,640.0f }, numberTextureHandle_);
		plusSprites_[index]->SetTextureInitialSize();
		plusSprites_[index]->SetFlip(false, false);
	}*/
}

void Timer::Draw() {
	int32_t drawNum;
	EulerTransform uv = { {1.0f,1.0f / 10.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	int32_t num = static_cast<int32_t>(numFloat_ * std::pow(10, kDecimalPoint));
	for (int index = 0; index < kDigits; index++) {
		drawNum = num / int(std::pow(10, kDigits - index - 1));
		drawNum %= 10;
		uv.translate.num[1] = 0.1f * float(drawNum);
		Sprites_[index]->Draw(transform_[index], uv, color_);
	}
	sprite_->Draw(decimalPosintTransform_, { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} }, color_);
}

void Timer::SetTransform(EulerTransform transform) {
	for (int index = 0; index < kDigits; index++) {
		transform_[index] = transform;
		transform_[index].scale.num[1] = transform.scale.num[1] * 0.1f;
		float leftTop = (float)kLeft + (index * (space_ + kWidth));
		if (index == kDecimalPoint) {
			decimalPosintTransform_ = transform;
			decimalPosintTransform_.translate.num[0] += leftTop - 18.0f;
			decimalPosintTransform_.translate.num[1] -= 15.0f;
		}
		if (index >= kDecimalPoint) {
			leftTop += 5.0f;
		}
		transform_[index].translate.num[0] += leftTop;
	}
}