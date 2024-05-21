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

	WorldTransform worldTransform_;
	WorldTransform worldTransform2_;
	WorldTransform worldTransformReticle_;
	WorldTransform worldTransformObject_;

	std::unique_ptr <CreateSphere> sphere_[2];
	Vector4 sphereMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

	//UISprite
	uint32_t uiResource_[2];

	std::unique_ptr <CreateSprite> uiSprite_[2];
	EulerTransform uiSpriteTransform_[2];
	EulerTransform uiSpriteuvTransform_[2];
	Vector4 uiSpriteMaterial_[2];

	bool isSpriteDraw_[2];

	std::unique_ptr <CreateSprite> sprite_;
	EulerTransform spriteTransform_;
	EulerTransform spriteTransform2_;
	EulerTransform SpriteuvTransform_;
	uint32_t spriteResourceNum_;

	//レティクルとワイヤー
	void Reticle(const ViewProjection viewProjection);
	void SetWire();
	void SetWireMiss();
	std::unique_ptr<CreateLine> line_;
	Vector4 lineMaterial_;
	float lineThickness_ = 0.2f;//ワイヤーの太さ
	int missTimer_ = 0;//ワイヤーをさせなかったときの演出用
	Vector3 wireVelocity_ = { 0.0f,0.0f,0.0f };//ワイヤーをさせなかったときの演出用
	bool isMissWire_ = false;
	bool isSetWire_ = false;
	bool isHitWire_ = false;

	Vector2 sensitivity_ = { 400.0f,400.0f };

	bool cameraChange_ = false;
};