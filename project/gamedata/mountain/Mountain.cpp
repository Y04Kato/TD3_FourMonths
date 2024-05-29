#include "Mountain.h"
#include "GameSelectScene.h"

void Mountain::Initialize()
{
	for (int i = 0; i < 10; i++)
	{
		model_[i].reset(Model::CreateModel("project/gamedata/resources/models/mountain", "mountain.obj"));

		worldTransform_[i].Initialize();
	}

	worldTransform_[0].translation_ = { -50.0f,-4.0f,20.0f };
	worldTransform_[0].scale_ = { 4.0f,4.0f,8.5f };

	worldTransform_[1].translation_ = { 50.0f,-4.0f,20.0f };
	worldTransform_[1].rotation_ = { 0.0f,3.13f,0.0f };
	worldTransform_[1].scale_ = { 4.0f,4.0f,8.5f };

	worldTransform_[2].translation_ = { -50.0f,-4.0f,290.0f };
	worldTransform_[2].scale_ = { 4.0f,4.0f,8.5f };

	worldTransform_[3].translation_ = { 50.0f,-4.0f,258.0f };
	worldTransform_[3].rotation_ = { 0.0f,3.13f,0.0f };
	worldTransform_[3].scale_ = { 4.0f,4.0f,8.5f };

	worldTransform_[4].translation_ = { -50.0f,-4.0f,560.0f };
	worldTransform_[4].scale_ = { 4.0f,4.0f,8.5f };

	worldTransform_[5].translation_ = { 50.0f,-4.0f,496.0f };
	worldTransform_[5].rotation_ = { 0.0f,3.13f,0.0f };
	worldTransform_[5].scale_ = { 4.0f,4.0f,8.5f };

	worldTransform_[6].translation_ = { -50.0f,-4.0f,830.0f };
	worldTransform_[6].scale_ = { 4.0f,4.0f,8.5f };

	worldTransform_[7].translation_ = { 50.0f,-4.0f,734.0f };
	worldTransform_[7].rotation_ = { 0.0f,3.13f,0.0f };
	worldTransform_[7].scale_ = { 4.0f,4.0f,8.5f };

	worldTransform_[8].translation_ = { -50.0f,-4.0f,1100.0f };
	worldTransform_[8].scale_ = { 4.0f,4.0f,8.5f };

	worldTransform_[9].translation_ = { 50.0f,-4.0f,972.0f };
	worldTransform_[9].rotation_ = { 0.0f,3.13f,0.0f };
	worldTransform_[9].scale_ = { 4.0f,4.0f,8.5f };

	modelMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
	//model_->SetDirectionalLightFlag(true, 3);
}

void Mountain::Update()
{
	//worldTransform_.rotation_.num[1] += 0.001f;
	//worldTransform_.rotation_.num[2] += 0.001f;

	for (int i = 0; i < 10; i++)
	{
		worldTransform_[i].UpdateMatrix();
	}

	ImGui::Begin("Mountain");
	ImGui::DragFloat3("WTFT", &worldTransform_[2].translation_.num[0], -13.0f, 13.0f);
	ImGui::DragFloat3("WTFR", &worldTransform_[2].rotation_.num[0], -13.0f, 13.0f);
	ImGui::DragFloat3("WTFS", &worldTransform_[2].scale_.num[0], 1.0f, 100.0f);
	ImGui::End();

	ImGui::Begin("Mountain2");
	ImGui::DragFloat3("WTFT", &worldTransform_[3].translation_.num[0], -13.0f, 13.0f);
	ImGui::DragFloat3("WTFR", &worldTransform_[3].rotation_.num[0], -13.0f, 13.0f);
	ImGui::DragFloat3("WTFS", &worldTransform_[3].scale_.num[0], 1.0f, 100.0f);
	ImGui::End();
}

void Mountain::Draw(const ViewProjection viewProjection)
{
	if (GameSelectScene::stageNum == 1)
	{
		for (int i = 0; i < 10; i++)
		{
			if (viewProjection.translation_.num[1] <= 0) {
				model_[i]->Draw(worldTransform_[i], viewProjection, Vector4{ modelMaterial_.num[0],modelMaterial_.num[1] ,modelMaterial_.num[2] ,0.05f });
			}
			else {
				model_[i]->Draw(worldTransform_[i], viewProjection, modelMaterial_);
			}
		}
	}
	else if (GameSelectScene::stageNum == 2)
	{
		for (int i = 0; i < 10; i++)
		{
			if (viewProjection.translation_.num[1] <= 0) {
				model_[i]->Draw(worldTransform_[i], viewProjection, Vector4{ modelMaterial_.num[0],modelMaterial_.num[1] ,modelMaterial_.num[2] ,0.05f });
			}
			else {
				model_[i]->Draw(worldTransform_[i], viewProjection, modelMaterial_);
			}
		}
	}
}
