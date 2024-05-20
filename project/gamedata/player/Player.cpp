#include "Player.h"

Player::Player() {

}

Player::~Player() {

}

void Player::Initialize() {
	textureManager_ = TextureManager::GetInstance();
	input_ = Input::GetInstance();

	worldTransform_.Initialize();
	worldTransformReticle_.Initialize();

	for (int i = 0; i < 2; i++) {
		worldTransform_.Initialize();
		sphere_[i] = std::make_unique <CreateSphere>();
		sphere_[i]->Initialize();
	}

	spriteResourceNum_ = textureManager_->Load("project/gamedata/resources/reticle.png");
	spriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f} };
	SpriteuvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
	sprite_ = std::make_unique <CreateSprite>();
	sprite_->Initialize(Vector2{ 200.0f,200.0f }, spriteResourceNum_);
	sprite_->SetTextureLTSize(Vector2{ 0.0f,0.0f }, Vector2{ 50.0f,50.0f });
	sprite_->SetAnchor(Vector2{ 0.5f,0.5f });
	sprite_->SetTextureInitialSize();
}

void Player::Updete(const ViewProjection viewProjection) {
	Reticle(viewProjection);

	worldTransform_.rotation_.num[1] += input_->GetMousePosition().Velocity.num[0] / 200.0f;
	worldTransform_.rotation_.num[0] += input_->GetMousePosition().Velocity.num[1] / 200.0f;

	SetCursorPos(1280 / 2, 720 / 2);

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

void Player::DrawUI() {
	sprite_->Draw(spriteTransform_, SpriteuvTransform_, sphereMaterial_);
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

void Player::Reticle(const ViewProjection viewProjection) {
	//自機とレティクルの距離
	const float kDistancePlayerToReticle = 50.0f;
	//自機からレティクルへのオフセット(Z+向き)
	Vector3 offset = { 0, 0, 1.0f };
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

	// 3Dレティクルのワールド行列からワールド座標を取得
	Vector3 positionReticle = {
		worldTransformReticle_.matWorld_.m[3][0], worldTransformReticle_.matWorld_.m[3][1],
		worldTransformReticle_.matWorld_.m[3][2] };
	// ビューポート行列
	Matrix4x4 matViewport =
		MakeViewportMatrix(0, 0, WinApp::kClientWidth, WinApp::kClientHeight, 0, 1);
	// ビュー行列とプロジェクション行列、ビューポート行列を合成
	Matrix4x4 matViewProjectionViewport =
		Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));
	// ワールド→スクリーン座標変換(3D→2D)
	positionReticle = TransformN(positionReticle, matViewProjectionViewport);
	// スプライトのレティクルに座標を設定
	spriteTransform_.translate = positionReticle;
}