#include "Goal.h"
#include "GameSelectScene.h"

void Goal::Initialize()
{
	
	model_[0].reset(Model::CreateModel("project/gamedata/resources/models/goal", "Goal.obj"));
	model_[1].reset(Model::CreateModel("project/gamedata/resources/models/goalStar", "GoalStar.obj"));

	for (int i = 0; i < 2; i++)
	{
		worldTransform_[i].Initialize();
	}

	modelMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
	//model_->SetDirectionalLightFlag(true, 3);
}

void Goal::Update()
{
	//worldTransform_.rotation_.num[1] += 0.001f;
	//worldTransform_.rotation_.num[2] += 0.001f;

	if (Iscene::sceneNo == GAME_SCENE)
	{
		for (int i = 0; i < 2; i++)
		{
			worldTransform_[i].translation_ = { -9.5f,49.3f,250.0f };
			worldTransform_[i].rotation_ = { 1.54f,0.0f,1.55f };
			worldTransform_[i].scale_ = { 100.0f,100.0f,100.0f };
		}
	}
	else if (Iscene::sceneNo == GAME_SCENE2)
	{
		for (int i = 0; i < 2; i++)
		{
			worldTransform_[i].translation_ = { -9.5f,49.3f,500.0f };
			worldTransform_[i].rotation_ = { 1.54f,0.0f,1.55f };
			worldTransform_[i].scale_ = { 100.0f,100.0f,100.0f };
		}
	}

	for (int i = 0; i < 2; i++)
	{
		worldTransform_[i].UpdateMatrix();
	}

	ImGui::Begin("Goal");
	ImGui::DragFloat3("WTFT", &worldTransform_[0].translation_.num[0],0.1f, -1300.0f, 1300.0f);
	ImGui::DragFloat3("WTFR", &worldTransform_[0].rotation_.num[0], 0.1f, -13.0f, 13.0f);
	ImGui::DragFloat3("WTFS", &worldTransform_[0].scale_.num[0], 0.1f, 1.0f, 50.0f);
	ImGui::End();

	ImGui::Begin("GoalStar");
	ImGui::DragFloat3("WTFT", &worldTransform_[1].translation_.num[0], 0.1f, -1300.0f, 1300.0f);
	ImGui::DragFloat3("WTFR", &worldTransform_[1].rotation_.num[0], 0.1f, -13.0f, 13.0f);
	ImGui::DragFloat3("WTFS", &worldTransform_[1].scale_.num[0], 0.1f, 1.0f, 50.0f);
	ImGui::End();
}

void Goal::Draw(const ViewProjection viewProjection)
{
	for (int i = 0; i < 2; i++)
	{
		model_[i]->Draw(worldTransform_[i], viewProjection, modelMaterial_);
	}
}

