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
#include "goal/Goal.h"
#include "numbers/numbers.h"
#include "datas/datas.h"

class GamePlayScene6 :public Iscene {
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

	std::unique_ptr<Model> model_;

	//Player
	Player* player_ = nullptr;
	StructSphere structSphere_;

	//カーソルの表示変数、0で非表示、1で表示
	//使用するShowCursor関数がboolを使えない為intで代用
	int showCursor = 0;

	//カメラモードのチェンジフラグ、falseでFollow、trueでdebug
	bool cameraChange_ = false;

	//ステージエディター擬き、名前をtest0 ~ objCountMax_で生成、それを記録する
	struct Obj {
		Model model;
		WorldTransform world;
		Vector4 material;
		Vector4 Backmaterial;
		std::string name;
		OBB obb_;
		bool isHit;
		bool isHitEye;
	};
	std::list<Obj> objects_;
	ModelData ObjModelData_;
	uint32_t ObjTexture_;
	char objName_[64];//オブジェクトの名前を入力する変数
	static const int objCountMax_ = 100;//オブジェクト最大数
	int objCount_ = 0;//オブジェクトの設置数
	std::string objNameHolder_[objCountMax_];//オブジェクトの名前を保存する変数
	StructSphere structSphereTree_;//木の当たり判定用

	const char* groupName = "GamePlayScene6";

	//Skydome
	Skydome* skydome_ = nullptr;

	//Mountain
	Mountain* mountain_ = nullptr;

	//Goal
	Goal* goal_ = nullptr;

	//TestSprite
	std::unique_ptr <CreateSprite> sprite_;
	EulerTransform spriteTransform_;
	EulerTransform SpriteuvTransform_;
	Vector4 spriteMaterial_;

	uint32_t spriteResource_;

	//UISprite
	uint32_t uiResource_[5];

	std::unique_ptr <CreateSprite> uiSprite_[5];
	EulerTransform uiSpriteTransform_[5];
	EulerTransform uiSpriteuvTransform_[5];
	Vector4 uiSpriteMaterial_[5];

	bool isSpriteDraw_[5];

	Segment segmentRay_;
	Segment segmentEye_;
	bool isHitWire_;
	bool isHitPlayer_;
	int resetTime_;

	//Particle
	std::unique_ptr<CreateParticle> particle_;
	Emitter testEmitter_ = {};
	AccelerationField accelerationField_;
	bool isBillBoard_ = true;
	bool isColor_;
	Vector4 particleColor_ = { 0.0f,0.0f,0.0f,0.0f };

	bool isGameStart_ = true;//ゲーム開始時に1回だけ呼ぶ

	//Restart
	WorldTransform startWorldTransform_;

	//Timer
	std::unique_ptr<Numbers> numbers_;
	float nowTime_ = 0;
	EulerTransform numbersTransform_;

	std::unique_ptr<Numbers> numbers2_;
	int nowCount_ = 0;
	EulerTransform numbersTransform2_;

	//Datas
	Datas* datas_;
};