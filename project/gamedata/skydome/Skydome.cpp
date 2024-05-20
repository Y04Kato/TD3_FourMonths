#include "Skydome.h"

void Skydome::Initialize()
{
	model_.reset(Model::CreateModel("project/gamedata/resources/models/skydome", "skydome.obj"));

	worldTransform_.Initialize();
	worldTransform_.scale_ = { 250.0f,250.0f,250.0f };

	modelMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
	model_->SetDirectionalLightFlag(true, 3);
}

void Skydome::Update()
{
	worldTransform_.rotation_.num[1] += 0.001f;
	worldTransform_.rotation_.num[2] += 0.001f;
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw(const ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, modelMaterial_);
}