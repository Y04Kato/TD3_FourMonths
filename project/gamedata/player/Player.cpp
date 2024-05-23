#include "Player.h"

Player::Player() {

}

Player::~Player() {

}

void Player::Initialize() {
	textureManager_ = TextureManager::GetInstance();
	input_ = Input::GetInstance();

	worldTransform_.Initialize();
	worldTransform2_.Initialize();
	worldTransformReticle_.Initialize();

	for (int i = 0; i < 2; i++) {
		sphere_[i] = std::make_unique <CreateSphere>();
		sphere_[i]->Initialize();
	}

	// レティクル:円 初期化
	spriteResourceNum_ = textureManager_->Load("project/gamedata/resources/reticlef1.png");
	reticleSpriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f} };
	reticleSpriteuvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
	reticleSprite_ = std::make_unique <CreateSprite>();
	reticleSprite_->Initialize(Vector2{ 200.0f,200.0f }, spriteResourceNum_);
	reticleSprite_->SetTextureLTSize(Vector2{ 0.0f,0.0f }, Vector2{ 50.0f,50.0f });
	reticleSprite_->SetAnchor(Vector2{ 0.5f,0.5f });
	reticleSprite_->SetTextureInitialSize();


	// レティクル:周り 初期化
	edgeSpriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f} };
	edgeSpriteuvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
	edgeSprite_ = std::make_unique<CreateSprite>();
	edgeSprite_->Initialize(Vector2{ 200.0f,200.0f }, textureManager_->Load("project/gamedata/resources/reticleCursol.png"));
	edgeSprite_->SetTextureLTSize(Vector2{ 0.0f,0.0f }, Vector2{ 50.0f,50.0f });
	edgeSprite_->SetAnchor(Vector2{ 0.5f,0.5f });
	edgeSprite_->SetTextureInitialSize();

	// Physicsクラス初期化
	physics_ = std::make_unique<Physics>();
	physics_->Initialize(20.0f);
	isActive_ = false;

}

void Player::Updete(const ViewProjection viewProjection) {
	//Reticle(viewProjection);

	worldTransform2_.translation_ = worldTransform_.translation_;
	worldTransform2_.rotation_.num[1] += input_->GetMousePosition().Velocity.num[0] / 200.0f;
	worldTransform2_.rotation_.num[0] += input_->GetMousePosition().Velocity.num[1] / 200.0f;

	if (input_->TriggerKey(DIK_6)) {
		isActive_ ^= true;
	}

	if (isActive_) {
		physics_->SetGravity({ 0.0f, -9.8f, 0.0f });
		Vector3 force = physics_->RubberMovement(worldTransform_.translation_, {0.0f, 10.0f, 10.0f}, 0.02f, 10.0f);
		physics_->AddForce(force);
		Vector3 velocity = physics_->Update();
		worldTransform_.translation_ += Multiply(physics_->deltaTime_, velocity);
		worldTransform_.UpdateMatrix();
	}

	ImGui::Begin("player");
	ImGui::DragFloat3("Pos", worldTransform_.translation_.num, 0.05f);
	ImGui::DragFloat3("Rot", worldTransform_.rotation_.num, 0.05f);
	ImGui::DragFloat3("ReticlePos", worldTransformReticle_.translation_.num, 0.05f);
	ImGui::End();
}

void Player::Draw(const ViewProjection viewProjection) {
	sphere_[0]->Draw(worldTransform_, viewProjection, sphereMaterial_, textureManager_->white);
	//sphere_[1]->Draw(worldTransformReticle_, viewProjection, sphereMaterial_, textureManager_->white);
}

void Player::DrawUI() {
	edgeSprite_->Draw(edgeSpriteTransform_, edgeSpriteuvTransform_, sphereMaterial_);
	reticleSprite_->Draw(reticleSpriteTransform_, reticleSpriteuvTransform_, sphereMaterial_);
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
	offset = TransformNormal(offset, worldTransform2_.constMap->matWorld);
	//ベクトルの長さを整える
	offset = Normalize(offset);
	offset.num[0] *= kDistancePlayerToReticle;
	offset.num[1] *= kDistancePlayerToReticle;
	offset.num[2] *= kDistancePlayerToReticle;
	//3Dレティクルの座標を設定
	worldTransformReticle_.translation_.num[0] = worldTransform2_.translation_.num[0] + offset.num[0];
	worldTransformReticle_.translation_.num[1] = worldTransform2_.translation_.num[1] + offset.num[1];
	worldTransformReticle_.translation_.num[2] = worldTransform2_.translation_.num[2] + offset.num[2];
	worldTransformReticle_.UpdateMatrix();

	worldTransform_.UpdateMatrix();
	worldTransform2_.UpdateMatrix();

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
	reticleSpriteTransform_.translate = positionReticle;
}