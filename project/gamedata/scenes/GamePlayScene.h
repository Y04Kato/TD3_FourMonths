#pragma once
#include "Iscene.h"
#include "components/audio/Audio.h"
#include "components/input/Input.h"
#include "components/3d/WorldTransform.h"
#include "components/3d/ViewProjection.h"
#include "TextureManager.h"
#include "components/2d/CreateTriangle.h"
#include "components/2d/CreateSprite.h"
#include "components/2d/CreateParticle.h"
#include "components/3d/CreateLine.h"
#include "components/3d/CreateSphere.h"
#include "components/3d/Model.h"
#include "components/debugcamera/DebugCamera.h"
#include "components/utilities/collisionManager/CollisionManager.h"
#include "components/utilities/collisionManager/CollisionConfig.h"

#include "components/utilities/globalVariables/GlobalVariables.h"
#include "components/utilities/followCamera/FollowCamera.h"

class GamePlayScene :public Iscene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void DrawPostEffect() override;
	void Finalize() override;

	void ApplyGlobalVariables();

	void SetObject(EulerTransform trans, const std::string& name);

private:
	CitrusJunosEngine* CJEngine_;
	DirectXCommon* dxCommon_;
	ViewProjection viewProjection_;
	TextureManager* textureManager_;
	Audio* audio_;
	Input* input_;
	DebugCamera* debugCamera_;
	CollisionManager* collisionManager_;

	std::unique_ptr<FollowCamera> followCamera_;

	std::unique_ptr<CreateLine> line_;
	WorldTransform worldTransformLine_[2];
	Vector4 lineMaterial_;
	float lineThickness_ = 0.2f;

	std::unique_ptr<Model> model_;

	std::unique_ptr <CreateSphere> linePoint_[2];
	Vector4 linePointMaterial_;

	//ステージエディター擬き、名前をtest0~始め、それを記録する
	struct Obj {
		Model model;
		WorldTransform world;
		Vector4 material;
		std::string name;
	};
	std::list<Obj> objects_;
	ModelData ObjModelData_;
	uint32_t ObjTexture_;
	char objName_[64];
	static const int objCountMax_ = 100;
	int objCount_ = 0;
	std::string objNameHolder_[objCountMax_];

	const char* groupName = "GamePlayScene";
};
