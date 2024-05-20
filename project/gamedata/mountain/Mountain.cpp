#include "Mountain.h"

void Mountain::Initialize()
{
	model_.reset(Model::CreateModel("project/gamedata/resources/models/mountain", "mountain.obj"));

	worldTransform_.Initialize();
	//worldTransform_.scale_ = { 250.0f,250.0f,250.0f };

	modelMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
	model_->SetDirectionalLightFlag(true, 3);
}

void Mountain::Update()
{
	//worldTransform_.rotation_.num[1] += 0.001f;
	//worldTransform_.rotation_.num[2] += 0.001f;
	worldTransform_.UpdateMatrix();
}

void Mountain::Draw(const ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, modelMaterial_);
}
