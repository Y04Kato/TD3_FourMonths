#pragma once
#include "components/3d/Model.h"
#include "components/3d/CreateSphere.h"
#include "components/3d/WorldTransform.h"
#include "components/3d/ViewProjection.h"
#include "components/2d/CreateSprite.h"
#include "components/input/Input.h"
#include "TextureManager.h"
#include "components/utilities/collisionManager/CollisionManager.h"
#include "components/utilities/collisionManager/CollisionConfig.h"

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

	void SetCameraMode(const bool cameraMode) { cameraChange_ = cameraMode; }

	void OnCollision()override;

private:
	TextureManager* textureManager_;
	Input* input_;

	WorldTransform worldTransform_;
	WorldTransform worldTransform2_;
	WorldTransform worldTransformReticle_;

	std::unique_ptr <CreateSphere> sphere_[2];
	Vector4 sphereMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

	std::unique_ptr <CreateSprite> sprite_;
	EulerTransform spriteTransform_;
	EulerTransform SpriteuvTransform_;
	uint32_t spriteResourceNum_;

	void Reticle(const ViewProjection viewProjection);

	Vector2 sensitivity_ = { 400.0f,400.0f };

	bool cameraChange_ = false;
};