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
	for (int index = 0; index < 2; index++) {
		decimalPosintSprites_[index].reset(new CreateSprite());
		decimalPosintSprites_[index]->Initialize(Vector2{ 64.0f, 64.0f }, decimalPointTextureHandle_);
		decimalPosintSprites_[index]->SetTextureInitialSize();
		decimalPosintSprites_[index]->SetFlip(false, false);
	}
	for (int index = 0; index < kPlusSecond; index++) {
		plusSecondSprites_[index].reset(new CreateSprite());
		plusSecondSprites_[index]->Initialize(Vector2{ 64.0f,640.0f }, numberTextureHandle_);
		plusSecondSprites_[index]->SetTextureInitialSize();
		plusSecondSprites_[index]->SetFlip(false, false);
	}
	plusTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/plus.png");
	plusSprite_.reset(new CreateSprite());
	plusSprite_->Initialize(Vector2{ 64.0f,64.0f }, plusTextureHandle_);
	plusSprite_->SetTextureInitialSize();
	plusSprite_->SetFlip(false, false);

	isAddTime = false;
	displaytime_ = 0;
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
	decimalPosintSprites_[0]->Draw(decimalPosintTransform_[0], {{1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}}, color_);
}

void Timer::AddTimeDraw()
{
	if (isAddTime) {
		if (displaytime_ < 1.0f) {
			int32_t drawNum;
			EulerTransform uv = { {1.0f,1.0f / 10.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
			int32_t num = static_cast<int32_t>(3.00f * std::pow(10, 2));
			EulerTransform plusTransform;
			EulerTransform dptransform;
			for (int index = 0; index < kPlusSecond; index++) {
				drawNum = num / int(std::pow(10, kPlusSecond - index - 1));
				drawNum %= 10;
				uv.translate.num[1] = 0.1f * float(drawNum);
				EulerTransform plusSecondTransform = plusSecondTransform_[index];
				plusSecondTransform.translate = Lerp(plusSecondTransform_[index].translate, transform_[index].translate, displaytime_);
				if (index == 0) {
					plusTransform = plusSecondTransform;
				}
				if (index == 1) {
					dptransform = plusSecondTransform;
				}
				plusSecondSprites_[index]->Draw(plusSecondTransform, uv, color_);
			}
			
			plusTransform.scale = { 1.0f, 1.0f, 1.0f };
			plusTransform.translate.num[0] -= 30.0f;
			plusSprite_->Draw(plusTransform, { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} }, color_);
			dptransform.scale = { 1.0f, 1.0f, 1.0f };
			dptransform.translate.num[0] -= 18.0f;
			dptransform.translate.num[1] -= 15.0f;
			decimalPosintSprites_[1]->Draw(dptransform, { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} }, color_);

			displaytime_ += 0.05f;
		}
		else {
			isAddTime = false;
			displaytime_ = 0.0f;
		}
	}

}

void Timer::SetTransform(EulerTransform transform) {
	for (int index = 0; index < kDigits; index++) {
		transform_[index] = transform;
		transform_[index].scale.num[1] = transform.scale.num[1] * 0.1f;
		float leftTop = (float)kLeft + (index * (space_ + kWidth));
		if (index == kDecimalPoint) {
			decimalPosintTransform_[0] = transform;
			decimalPosintTransform_[0].translate.num[0] += leftTop - 18.0f;
			decimalPosintTransform_[0].translate.num[1] -= 15.0f;
		}
		if (index >= kDecimalPoint) {
			leftTop += 5.0f;
		}
		transform_[index].translate.num[0] += leftTop;
	}

	for (int index = 0; index < kPlusSecond; index++) {
		plusSecondTransform_[index] = transform;
		plusSecondTransform_[index].scale.num[1] = transform.scale.num[1] * 0.1f;
		float leftTop = (float)kLeft + (index * (space_ + kWidth)) + 260.0f;
		if (index == 0) {
			plusTransform_ = transform;
			plusTransform_.translate.num[0] += leftTop - 30.0f;

		}
		if (index == kPlusSecond - kDecimalPoint) {
			decimalPosintTransform_[1] = transform;
			decimalPosintTransform_[1].translate.num[0] += leftTop - 18.0f;
			decimalPosintTransform_[1].translate.num[1] -= 15.0f;
		}
		if (index >= kPlusSecond - kDecimalPoint) {
			leftTop += 5.0f;
		}
		plusSecondTransform_[index].translate.num[0] += leftTop;
	}
}