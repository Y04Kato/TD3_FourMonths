#pragma once
#include "components/3d/Model.h"
#include "components/3d/CreateSphere.h"
#include "components/3d/WorldTransform.h"
#include "components/3d/ViewProjection.h"
#include "components/2d/CreateSprite.h"
#include "components/input/Input.h"
#include "TextureManager.h"
#include "components/2d/CreateSprite.h"
#include "components/utilities/collisionManager/CollisionManager.h"
#include "components/utilities/collisionManager/CollisionConfig.h"
#include "components/3d/CreateLine.h"

#include "Physics/Physics.h"

class Player : public Collider {
public:
	Player();
	~Player();

	void Initialize();

	void Updete(const ViewProjection viewProjection);

	void Draw(const ViewProjection viewProjection);

	void DrawUI();

	WorldTransform GetWorldTransform() override { return worldTransform_; }
	const WorldTransform& GetWorldTransformPlayer() { return worldTransform2_; }
	const WorldTransform& GetWorldTransformReticle() { return worldTransformReticle_; }

	void SetWorldTransform(const WorldTransform world);
	void SetWorldTransformReticle(const WorldTransform world);
	void SetWorldTransformObject(const WorldTransform world) { worldTransformObject_ = world; }

	void SetCameraMode(const bool cameraMode) { cameraChange_ = cameraMode; }
	void SetIsHit(const bool isHit) { isHitWire_ = isHit; }

	void OnCollision()override;

private:
	TextureManager* textureManager_;
	Input* input_;

	WorldTransform worldTransform_;//自機用
	WorldTransform worldTransform2_;//視点用
	WorldTransform worldTransformReticle_;//レティクル用
	WorldTransform worldTransformWire_;//ワイヤー演出用
	WorldTransform worldTransformObject_;//狙っているオブジェクト用

	std::unique_ptr<CreateSphere>sphere_[2];
	Vector4 sphereMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

	//UISprite
	uint32_t uiResource_[2];

	std::unique_ptr <CreateSprite> uiSprite_[2];
	EulerTransform uiSpriteTransform_[2];
	EulerTransform uiSpriteuvTransform_[2];
	Vector4 uiSpriteMaterial_[2];

	bool isSpriteDraw_[2];

	std::unique_ptr <CreateSprite> spriteReticle_;
	std::unique_ptr <CreateSprite> debugReticle_;
	EulerTransform spriteTransform_;
	EulerTransform spriteTransform2_;
	EulerTransform SpriteuvTransform_;
	uint32_t spriteResourceNum_;

	//レティクルとワイヤー
	void Reticle(const ViewProjection viewProjection);//レティクルの計算関数
	const float kDistancePlayerToReticle = 40.0f;//自機とレティクルの距離、通常射程
	float DistancePlayerToReticle = kDistancePlayerToReticle;//自機とレティクルの距離
	void SetWire();//ワイヤー成功時関数
	void SetWireMiss();//ワイヤー失敗時関数
	std::unique_ptr<CreateLine> line_;
	Vector4 lineMaterial_;
	float lineThickness_ = 0.2f;//ワイヤーの太さ
	int missTimer_ = 0;//ワイヤーをさせなかった時の演出用
	Vector3 wireVelocity_ = { 0.0f,0.0f,0.0f };//ワイヤーをさせなかったときの演出用
	bool isSetWire_ = false;//ワイヤー成功フラグ
	bool isMissWire_ = false;//ワイヤー失敗フラグ
	bool isHitWire_ = false;//レティクルがオブジェクトを捉えているかのフラグ

	//マウス感度、この値で割る為、高くなるほど感度が低くなる
	Vector2 sensitivity_ = { 400.0f,400.0f };

	//カメラモードのチェンジフラグ、falseでFollow、trueでdebug
	bool cameraChange_ = false;

	Segment segment_;
	OBB obb_;

	// 物理挙動クラス
	std::unique_ptr<Physics> physics_;
	bool isActive_ = false;
};