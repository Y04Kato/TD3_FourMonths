#include "Player.h"

Player::Player() {

}

Player::~Player() {

}

void Player::Initialize() {
	textureManager_ = TextureManager::GetInstance();

	worldTransform_.Initialize();
	worldTransformReticle_.Initialize();

	for (int i = 0; i < 2; i++) {
		worldTransform_.Initialize();
		sphere_[i] = std::make_unique <CreateSphere>();
		sphere_[i]->Initialize();
	}
}

void Player::Updete() {
	//自機とレティクルの距離
	const float kDistancePlayerToReticle = 50.0f;
	//自機からレティクルへのオフセット(Z+向き)
	Vector3 offset = {0, 0, 1.0f};
	//自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.constMap->matWorld);
	//ベクトルの長さを整える
	offset = Normalize(offset);
	offset.num[0] *= kDistancePlayerToReticle;
	offset.num[1] *= kDistancePlayerToReticle;
	offset.num[2] *= kDistancePlayerToReticle;
	//3Dレティクルの座標を設定
	worldTransformReticle_.translation_.num[0] = GetWorldTransform().translation_.num[0] + offset.num[0];
	worldTransformReticle_.translation_.num[1] = GetWorldTransform().translation_.num[1] + offset.num[1];
	worldTransformReticle_.translation_.num[2] = GetWorldTransform().translation_.num[2] + offset.num[2];
	worldTransformReticle_.UpdateMatrix();

	worldTransform_.UpdateMatrix();

	ImGui::Begin("player");
	ImGui::DragFloat3("Pos", worldTransform_.translation_.num, 0.05f);
	ImGui::DragFloat3("Rot", worldTransform_.rotation_.num, 0.05f);
	ImGui::DragFloat3("ReticlePos", worldTransformReticle_.translation_.num, 0.05f);
	ImGui::End();
}

void Player::Draw(const ViewProjection viewProjection) {
	sphere_[0]->Draw(worldTransform_, viewProjection, sphereMaterial_, textureManager_->white);
	sphere_[1]->Draw(worldTransformReticle_, viewProjection, sphereMaterial_, textureManager_->white);
}

void Player::SetWorldTransform(const WorldTransform world) {
	worldTransform_ = world;
}

void Player::SetWorldTransformReticle(const WorldTransform world) {
	worldTransformReticle_ = world;
}

void Player::OnCollision() {
	return;
}