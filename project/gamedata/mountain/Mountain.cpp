#include "Mountain.h"

void Mountain::Initialize()
{
	for (int i = 0; i < 2; i++)
	{
		model_[i].reset(Model::CreateModel("project/gamedata/resources/models/mountain", "mountain.obj"));

		worldTransform_[i].Initialize();
	}

	worldTransform_[0].translation_ = { -18.0f,-4.0f,20.0f };
	worldTransform_[0].scale_ = { 1.0f,4.0f,8.0f };

	worldTransform_[1].translation_ = { 18.0f,-4.0f,20.0f };
	worldTransform_[1].rotation_ = {0.0f,3.13f,0.0f};
	worldTransform_[1].scale_ = { 1.0f,4.0f,8.0f };

	modelMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
	//model_->SetDirectionalLightFlag(true, 3);
}

void Mountain::Update()
{
	//worldTransform_.rotation_.num[1] += 0.001f;
	//worldTransform_.rotation_.num[2] += 0.001f;

	for (int i = 0; i < 2; i++)
	{
		worldTransform_[i].UpdateMatrix();
	}

	ImGui::Begin("Mountain");
	ImGui::DragFloat3("WTFT", &worldTransform_[0].translation_.num[0], -13.0f, 13.0f);
	ImGui::DragFloat3("WTFR", &worldTransform_[0].rotation_.num[0], -13.0f, 13.0f);
	ImGui::DragFloat3("WTFS", &worldTransform_[0].scale_.num[0], 1.0f, 100.0f);
	ImGui::End();

	ImGui::Begin("Mountain2");
	ImGui::DragFloat3("WTFT", &worldTransform_[1].translation_.num[0], -13.0f, 13.0f);
	ImGui::DragFloat3("WTFR", &worldTransform_[1].rotation_.num[0], -13.0f, 13.0f);
	ImGui::DragFloat3("WTFS", &worldTransform_[1].scale_.num[0], 1.0f, 100.0f);
	ImGui::End();
}

void Mountain::Draw(const ViewProjection viewProjection)
{
	model_[0]->Draw(worldTransform_[0], viewProjection, modelMaterial_);

	model_[1]->Draw(worldTransform_[1], viewProjection, modelMaterial_);
}
