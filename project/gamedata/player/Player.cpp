#include "Player.h"

Player::Player() {

}

Player::~Player() {

}

void Player::Initialize() {
	textureManager_ = TextureManager::GetInstance();
	input_ = Input::GetInstance();

	//テクスチャ
	uiResource_[0] = textureManager_->Load("project/gamedata/resources/UI/WireUI.png");
	uiResource_[1] = textureManager_->Load("project/gamedata/resources/UI/MoveUI.png");

	for (int i = 0; i < 2; i++)
	{
		uiSpriteMaterial_[i] = { 1.0f,1.0f,1.0f,1.0f };
		uiSpriteTransform_[i] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f} };

		uiSpriteuvTransform_[i] = {
			{1.0f,1.0f,1.0f},
			{0.0f,0.0f,0.0f},
			{0.0f,0.0f,0.0f},
		};

		uiSprite_[i] = std::make_unique <CreateSprite>();
	}

	uiSprite_[0]->Initialize(Vector2{ 1280.0f,720.0f }, uiResource_[0]);
	uiSprite_[0]->SetAnchor(Vector2{ 0.5f,0.5f });

	uiSprite_[1]->Initialize(Vector2{ 1280.0f,720.0f }, uiResource_[1]);
	uiSprite_[1]->SetAnchor(Vector2{ 0.5f,0.5f });

	worldTransform_.Initialize();
	worldTransform2_.Initialize();
	worldTransformReticle_.Initialize();
	worldTransformWire_.Initialize();
	worldTransformGrapple_.Initialize();

	worldTransform_.translation_ = {0.0f,10.0f,0.0f};

	for (int i = 0; i < 2; i++) {
		sphere_[i] = std::make_unique <CreateSphere>();
		sphere_[i]->Initialize();
	}

	spriteResourceNum_ = textureManager_->Load("project/gamedata/resources/reticle.png");
	spriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f} };
	spriteTransform2_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,415.0f,0.0f} };
	SpriteuvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
	spriteReticle_ = std::make_unique <CreateSprite>();
	spriteReticle_->Initialize(Vector2{ 200.0f,200.0f }, spriteResourceNum_);
	spriteReticle_->SetTextureLTSize(Vector2{ 0.0f,0.0f }, Vector2{ 50.0f,50.0f });
	spriteReticle_->SetAnchor(Vector2{ 0.5f,0.5f });
	spriteReticle_->SetTextureInitialSize();

	debugReticle_ = std::make_unique <CreateSprite>();
	debugReticle_->Initialize(Vector2{ 200.0f,200.0f }, spriteResourceNum_);
	debugReticle_->SetTextureLTSize(Vector2{ 0.0f,0.0f }, Vector2{ 50.0f,50.0f });
	debugReticle_->SetAnchor(Vector2{ 0.5f,0.5f });
	debugReticle_->SetTextureInitialSize();

	//Line
	line_ = std::make_unique <CreateLine>();
	line_->Initialize();
	line_->SetDirectionalLightFlag(false, 0);
	lineMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

	// 物理挙動クラス初期化
	physics_ = std::make_unique<Physics>();
	physics_->Initialize(1.0f);

}

void Player::Updete(const ViewProjection viewProjection) {
	if (cameraChange_ == true) {//DebugCamera

	}
	else {//FollowCamera
		Reticle(viewProjection);

		worldTransform2_.translation_ = worldTransform_.translation_;
		worldTransform2_.rotation_.num[1] += input_->GetMousePosition().Velocity.num[0] / sensitivity_.num[0];
		worldTransform2_.rotation_.num[0] += input_->GetMousePosition().Velocity.num[1] / sensitivity_.num[0];
	}

	worldTransform_.UpdateMatrix();
	worldTransform2_.UpdateMatrix();

	if (input_->pushMouse(MOUSE_BOTTON0)) {//左クリックした時
		DistancePlayerToReticle = kDistancePlayerToReticle;
		Reticle(viewProjection);
		if (isHitWire_ == true) {//レティクルがオブジェクト捉えていれば
			DistancePlayerToReticle = worldTransformObject_.translation_.num[2] - worldTransform2_.translation_.num[2];
			worldTransformGrapple_ = worldTransformWire_;
			if (DistancePlayerToReticle <= 0) {
				DistancePlayerToReticle = -DistancePlayerToReticle + 5.0f;
			}
			Reticle(viewProjection);
			SetWire();
		}
		else {//レティクルがオブジェクトを捉えられていなければ
			SetWireMiss();
		}
	}

	if (input_->pushMouse(MOUSE_BOTTON1)) {//右クリックした時
		isSetWire_ = false; // ワイヤーを外す
	}

	if (isSetWire_ == true) {//ワイヤー成功時の演出
		DistancePlayerToReticle = kDistancePlayerToReticle;
		Reticle(viewProjection);
	}

	if (isMissWire_ == true) {//ワイヤー失敗時の演出
		missTimer_++;
		worldTransformWire_.translation_ += wireVelocity_;
	}
	if (missTimer_ >= 15) {
		isMissWire_ = false;
		missTimer_ = 0;
	}

	if (input_->TriggerKey(DIK_6)) {
		isActive_ ^= true;
	}

	if (isActive_) {
		physics_->SetGravity({ 0.0f, -20.8f, 0.0f });
		if (input_->PressKey(DIK_A)) {
			Vector3 force = { -50.0f, 0.0f, 0.0f };
			physics_->AddForce(force, 1);
		}
		if (input_->PressKey(DIK_D)) {
			Vector3 force = { 50.0f, 0.0f, 0.0f };
			physics_->AddForce(force, 1);
		}
		if (isSetWire_) {
			// ワイヤー中の物理挙動
			Vector3 force = physics_->RubberMovement(worldTransform_.translation_, worldTransformGrapple_.translation_, 1.0f, 0.0f);
			physics_->AddForce(force);
		}
		Vector3 velocity = physics_->Update();
		worldTransform_.translation_ += velocity * physics_->deltaTime_;

	}

	if (worldTransform_.translation_.num[1] <= -3.0f)
	{
		isDead_ = true;
		isActive_ = false;
		worldTransform_.translation_ = { 0.0f,10.0f,0.0f };
	}

	ImGui::Begin("player");
	ImGui::DragFloat3("Pos", worldTransform_.translation_.num, 0.05f);
	ImGui::DragFloat3("Rot", worldTransform2_.rotation_.num, 0.05f);
	ImGui::DragFloat3("ReticlePos", worldTransformReticle_.translation_.num, 0.05f);
	ImGui::DragFloat3("velocity", wireVelocity_.num, 0.05f);
	ImGui::DragFloat2("MouseSensitivity", sensitivity_.num, 0.05f);
	ImGui::DragFloat("LineThickness", &lineThickness_, 0.05f, 0.0f);
	line_->SetLineThickness(lineThickness_);
	ImGui::End();
}

void Player::Draw(const ViewProjection viewProjection) {
	sphere_[0]->Draw(worldTransform_, viewProjection, sphereMaterial_, textureManager_->white);
	if (isSetWire_ == true || isMissWire_ == true) {
		line_->Draw(worldTransform2_, worldTransformWire_, viewProjection, lineMaterial_);
	}
}

void Player::DrawUI() {
	for (int i = 0; i < 2; i++) {
		uiSprite_[i]->Draw(uiSpriteTransform_[i], uiSpriteuvTransform_[i], uiSpriteMaterial_[i]);
	}

	spriteReticle_->Draw(spriteTransform2_, SpriteuvTransform_, sphereMaterial_);
	//debugReticle_->Draw(spriteTransform_, SpriteuvTransform_, sphereMaterial_);
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
	//自機からレティクルへのオフセット(Z+向き)
	Vector3 offset = { 0, 0, 1.0f };
	//自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform2_.constMap->matWorld);
	//ベクトルの長さを整える
	offset = Normalize(offset);
	offset.num[0] *= DistancePlayerToReticle;
	offset.num[1] *= DistancePlayerToReticle;
	offset.num[2] *= DistancePlayerToReticle;
	//3Dレティクルの座標を設定
	worldTransformReticle_.translation_.num[0] = worldTransform2_.translation_.num[0] + offset.num[0];
	worldTransformReticle_.translation_.num[1] = worldTransform2_.translation_.num[1] + offset.num[1];
	worldTransformReticle_.translation_.num[2] = worldTransform2_.translation_.num[2] + offset.num[2];
	worldTransformReticle_.UpdateMatrix();

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

void Player::SetWire() {
	isSetWire_ = true;
	isMissWire_ = false;

	worldTransformWire_.translation_ = Vector3{ worldTransformObject_.translation_.num[0],worldTransformReticle_.translation_.num[1] ,worldTransformObject_.translation_.num[2] };
}

void Player::SetWireMiss() {
	isMissWire_ = true;
	isSetWire_ = false;
	worldTransformWire_.translation_ = worldTransform2_.translation_;
	wireVelocity_ = worldTransformReticle_.translation_ - worldTransform2_.translation_;
	wireVelocity_ = Normalize(wireVelocity_) * 5.0f;
}