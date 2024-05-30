#include "Mountain.h"

void Mountain::Initialize()
{
	for (int i = 0; i < 15; i++)
	{
		mountainModel_[i].reset(Model::CreateModel("project/gamedata/resources/models/mountain", "mountain.obj"));

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

	worldTransform_[10].translation_ = { -50.0f,-4.0f,1370.0f };
	worldTransform_[10].scale_ = { 4.0f,4.0f,8.5f };

	worldTransform_[11].translation_ = { 50.0f,-4.0f,1210.0f };
	worldTransform_[11].rotation_ = { 0.0f,3.13f,0.0f };
	worldTransform_[11].scale_ = { 4.0f,4.0f,8.5f };

	worldTransform_[12].translation_ = { -50.0f,-4.0f,1640.0f };
	worldTransform_[12].scale_ = { 4.0f,4.0f,8.5f };

	worldTransform_[13].translation_ = { 50.0f,-4.0f,1448.0f };
	worldTransform_[13].rotation_ = { 0.0f,3.13f,0.0f };
	worldTransform_[13].scale_ = { 4.0f,4.0f,8.5f };

	worldTransform_[14].translation_ = { 50.0f,-4.0f,1637.0f };
	worldTransform_[14].rotation_ = { 0.0f,3.13f,0.0f };
	worldTransform_[14].scale_ = { 4.0f,4.0f,8.5f };

	modelMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
	//model_->SetDirectionalLightFlag(true, 3);
}

void Mountain::Update()
{
	for (int i = 0; i < 15; i++)
	{
		worldTransform_[i].UpdateMatrix();
	}
}

void Mountain::Draw(const ViewProjection viewProjection)
{
	
	for (int i = 0; i < 15; i++)
	{
		mountainModel_[i]->Draw(worldTransform_[i], viewProjection, modelMaterial_);
	}
}
