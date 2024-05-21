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

#include "player/Player.h"
#include "skydome/Skydome.h"
#include "mountain/Mountain.h"

class GamePlayScene :public Iscene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
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
	Vector4 lineMaterial_;
	float lineThickness_ = 0.2f;

	std::unique_ptr<Model> model_;

	Player* player_ = nullptr;

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

	//Skydome
	Skydome* skydome_ = nullptr;

	//Mountain
	Mountain* mountain_ = nullptr;

	//TestSprite
	std::unique_ptr <CreateSprite> sprite_;
	EulerTransform spriteTransform_;
	EulerTransform SpriteuvTransform_;
	Vector4 spriteMaterial_;

	uint32_t spriteResource_;

	//UISprite
	uint32_t uiResource_[2];

	std::unique_ptr <CreateSprite> uiSprite_[2];
	EulerTransform uiSpriteTransform_[2];
	EulerTransform uiSpriteuvTransform_[2];
	Vector4 uiSpriteMaterial_[2];

	bool isSpriteDraw_[2];

	//Goal
	bool isGoal_ = false;
};
