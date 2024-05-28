#include "numbers.h"

void Numbers::Initialize() {
	numberTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/Numbers.png");
	for (int index = 0; index < kDigits; index++) {
		Sprites_[index].reset(new CreateSprite());
		Sprites_[index]->Initialize(Vector2{ 64.0f,640.0f }, numberTextureHandle_);
		Sprites_[index]->SetTextureInitialSize();
		Sprites_[index]->SetFlip(false, false);
	}
}

void Numbers::Draw() {
	int32_t drawNum;
	EulerTransform uv = { {1.0f,1.0f / 10.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	for (int index = 0; index < kDigits; index++) {
		drawNum = num_ / int(std::pow(10, kDigits - index - 1));
		drawNum %= 10;
		uv.translate.num[1] = 0.1f * float(drawNum);
		Sprites_[index]->Draw(transform_[index], uv, color_);
	}
}

void Numbers::SetTransform(EulerTransform transform) {
	for (int index = 0; index < kDigits; index++) {
		transform_[index] = transform;
		transform_[index].scale.num[1] = transform.scale.num[1] * 0.1f;
		float leftTop = (float)kLeft + (index * (space_ + kWidth));
		transform_[index].translate.num[0] += leftTop; 
	}
}