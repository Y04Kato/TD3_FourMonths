#include "Player.h"
#include "GameSelectScene.h"

Player::Player() {

}

Player::~Player() {

}

void Player::Initialize() {
	textureManager_ = TextureManager::GetInstance();
	input_ = Input::GetInstance();

	//テクスチャ
	uiResource_[0] = textureManager_->Load("project/gamedata/resources/UI/MoveUI.png");
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

	worldTransform_.translation_ = { 0.0f,20.0f,0.0f };

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
	upSize_ = 0.0f;

	// モデルの向き(無回転時)
	forwad_ = { 0.0f, 0.0f, 1.0f };
	right_ = { 1.0f, 0.0f, 0.0f };

	//Particle
	testEmitter_.transform.translate = { 0.0f,0.0f,45.0f };
	testEmitter_.transform.rotate = { 0.0f,0.0f,0.0f };
	testEmitter_.transform.scale = { 1.0f,1.0f,1.0f };
	testEmitter_.count = 15;
	testEmitter_.frequency = 0.05f;
	testEmitter_.frequencyTime = 0.0f;//発生頻度の時刻

	accelerationField_.acceleration = { 0.0f,0.0f,0.0f };
	accelerationField_.area.min = { -1.0f,-1.0f,-1.0f };
	accelerationField_.area.max = { 1.0f,1.0f,1.0f };

	spriteResource_ = textureManager_->Load("project/gamedata/resources/circle.png");

	particle_ = std::make_unique <CreateParticle>();

	particle_->Initialize(100, testEmitter_, accelerationField_, spriteResource_);
	//particle_->SetisVelocity(true);
	particle_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	particle_->SetLifeTime(10.0f);

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

	if (input_->pushMouse(MOUSE_BOTTON0) && isActive_ ) {//左クリックした時
		DistancePlayerToReticle = kDistancePlayerToReticle;
		isHitObj_ = false;
		Reticle(viewProjection);
		if (isHitWire_ == true) {//レティクルがオブジェクト捉えていれば
			DistancePlayerToReticle = worldTransformObject_.translation_.num[2] - worldTransform2_.translation_.num[2];
			upSize_ = 0.0f; // 上昇量を初期化
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
		isDownSpeed_ = true;
	}

	if (isMissWire_ == true) {//ワイヤー失敗時の演出
		missTimer_++;
		worldTransformWire_.translation_ += wireVelocity_;
	}
	if (missTimer_ >= 15) {
		isMissWire_ = false;
		missTimer_ = 0;
	}
	
	if (isHitObj_ == true) {//接触時の演出
		HitTimer_++;
	}
	if (HitTimer_ >= 10) {
		isHitObj_ = false;
		HitTimer_ = 0;
	}

	if (input_->pushMouse(MOUSE_BOTTON0)) {
		isActive_ = true;
	}

	particle_->SetTranslate(worldTransform_.translation_);
	accelerationTimer_++;
	if (accelerationTimer_ <= accelerationTimerMax_ / 2.0f) {
		accelerationField_.acceleration.num[0] = 1000.0f;
	}
	else if (accelerationTimer_ > accelerationTimerMax_ / 2.0f) {
		accelerationField_.acceleration.num[0] = -1000.0f;
	}
	if (accelerationTimer_ >= accelerationTimerMax_) {
		accelerationTimer_ = 0.0f;
	}
	particle_->SetAccelerationField(accelerationField_);
	particle_->Update();

	ImGui::Begin("physics");
	if (isActive_) {
		if (isSetWire_ && !isRoll_) { // ワイヤー中
			physics_->SetGravity(gravityHaveWire_); // ワイヤー中の重力
			// ワイヤー中の物理挙動
			Vector3 force = physics_->RubberMovement(worldTransform_.translation_, worldTransformGrapple_.translation_, stiffness_, dampingCoefficient_);
			physics_->AddForce(force);

			// 進んでいる方向の単位ベクトルを求める(Y軸を除く)
			Vector2 vec = physics_->Vector2Perpendicular({ start_.num[0], start_.num[2] });
			vec = physics_->Vector2Normalize(vec);
			Vector3 dir = { vec.num[0], 0.0f ,vec.num[1] };/*{ vec.num[0], 0.0f, vec.num[1] }*/; // 進んでいる方向の単位ベクトル
			/*Vector2 vec = physics_->Vector2Perpendicular({ physics_->GetVelocity().num[0], physics_->GetVelocity().num[2]});
			vec = physics_->Vector2Normalize(vec);
			Vector3 dir = { vec.num[0], 0.0f, vec.num[1]};*/

			if (input_->PressKey(DIK_A)) { // 進んでいる方向に対して左
				Vector3 force = -sideForceValueHaveWire_ * dir;
				physics_->AddForce(force, 1);
			}
			if (input_->PressKey(DIK_D)) { // 進んでいる方向に対して右
				Vector3 force = sideForceValueHaveWire_ * dir;
				physics_->AddForce(force, 1);
			}
			if (input_->PressKey(DIK_W)) { // 上に徐々に上がる
				Vector3 force = { 0.0f, upSize_ * upSize_, 0.0f };
				//upForce_ += 1.0f; // 上昇量
				if (upSize_ < maxUpSize_) {
					upSize_ += upSizeValue_; // 上昇量
				}
				physics_->AddForce(force, 1);
			}

			// ある程度の進んだら自動でワイヤーが切れる(角度で判定)
			if (physics_->Vector3Angle(start_, Normalize(worldTransform2_.translation_ - worldTransformGrapple_.translation_)) < limitAngle_) {
				isSetWire_ = false;
			}

		}
		else if (isRoll_) {
			if (physics_->GetGravity().num[1] != 0.0f) {
				physics_->SetGravity({ 0.0f, 0.0f, 0.0f });
				physics_->SetVelocity({ 0.0f, 0.0f, 0.0f });
			}

			angle_ += angularVelocity * physics_->deltaTime_;

			if (angle_ > 180.0f * physics_->DegToRad() || angle_ < -180.0f * physics_->DegToRad()) {
				isRoll_ = false;
				isSetWire_ = false;
			}
			else {
				worldTransform_.translation_ = { worldTransformGrapple_.translation_.num[0] + std::cosf(startAngle_ + angle_) * 15.0f,worldTransform_.translation_.num[1], worldTransformGrapple_.translation_.num[2] + std::sinf(startAngle_ + angle_) * 15.0f };
				float a2 = angularVelocity * angularVelocity /** 15.0f*/;
				Vector3 force = { -15.0f * a2 * std::cosf(startAngle_ + angle_), 0.0f, -15.0f * a2 * std::sinf(startAngle_ + angle_) };
				physics_->AddForce(force);
			}
		}
		else { // ワイヤーじゃない時
			physics_->SetGravity(gravityNoWire_); // ワイヤー中じゃない時の重力
			if (input_->PressKey(DIK_A)) {

				Vector3 force = -sideForceValueNoWire_ * right_/*{ -sideForceValueNoWire_ , 0.0f, 0.0f }*/;
				physics_->AddForce(force, 1);
			}
			if (input_->PressKey(DIK_D)) {
				Vector3 force = sideForceValueNoWire_ * right_/*{ sideForceValueNoWire_, 0.0f, 0.0f }*/;
				physics_->AddForce(force, 1);
			}

			if (isDownSpeed_) {
				Vector3 dir = physics_->GetVelocity();
				if (dir.num[1] < 0.0f) {
					dir.num[1] = 0.0f;
				}
				//dir.num[1] = 0.0f;
				if (Length(dir) > minSpeedVolume_) {
					if (downSpeedSize_ < maxDownSpeedSize_) {
						// 減少量がだんだん大きくなる
						downSpeedSize_ += downSpeedValue_;
					}
					Vector3 force = -downSpeedSize_ * Normalize(dir);
					physics_->AddForce(force, 0);
				}
				else {
					// 初期化
					downSpeedSize_ = 0.0f;
				}
			}
		}

		Vector3 velocity = physics_->Update();
		if (!isRoll_) {
			worldTransform_.translation_ += velocity * physics_->deltaTime_;
			//Vector3 impulse = physics_->GetImpulse_();
			//worldTransform_.translation_ += impulse/* * physics_->deltaTime_*/;
			ImGui::DragFloat3("velocity", velocity.num, 0.05f);
			//ImGui::DragFloat3("impulse", impulse.num, 0.05f);
		}

		//physics_->Vector3Direction((velocity + impulse), &forwad_, &right_);
	}
	ImGui::End();

	//画面端
	if (worldTransform_.translation_.num[0] >= 75.0f)
	{
		worldTransform_.translation_.num[0] = 75.0f;
	}

	if (worldTransform_.translation_.num[0] <= -75.0f)
	{
		worldTransform_.translation_.num[0] = -75.0f;
	}

	//床に落ちたとき
	if (worldTransform_.translation_.num[1] <= -3.0f)
	{
		worldTransform_.translation_.num[1] = -3.0f;
		physics_->SetVelocity({ 0.0f, 0.0f, 0.0f });
		Vector3 force = { 0.0f, 25.0f, 0.0f };
		physics_->AddForce(force, 1);
		isFell_ = true;
		
	}

	//リスタート
	if (input_->TriggerKey(DIK_R))
	{
		isRestart_ = true;
		isActive_ = false;
		isSetWire_ = false;
		physics_->SetVelocity({ 0.0f, 0.0f, 0.0f });
	}

	//ゴール
	if (GameSelectScene::stageNum == 1 && worldTransform_.translation_.num[2] >= 1000.0f)
	{
		isGoal_ = true;
		isActive_ = false;
		isSetWire_ = false;
		physics_->SetVelocity({ 0.0f, 0.0f, 0.0f });
	}
	else if (GameSelectScene::stageNum == 2 && worldTransform_.translation_.num[2] >= 500.0f)
	{
		isGoal_ = true;
		isActive_ = false;
		isSetWire_ = false;
		physics_->SetVelocity({ 0.0f, 0.0f, 0.0f });
	}

	ImGui::Begin("player");
	ImGui::DragFloat3("Pos", worldTransform_.translation_.num, 0.05f);
	ImGui::DragFloat3("Rot", worldTransform2_.rotation_.num, 0.05f);
	ImGui::DragFloat3("ReticlePos", worldTransformReticle_.translation_.num, 0.05f);
	ImGui::DragFloat3("velocity", accelerationField_.acceleration.num, 0.05f);
	ImGui::DragFloat2("MouseSensitivity", sensitivity_.num, 0.05f);
	ImGui::DragFloat("LineThickness", &lineThickness_, 0.05f, 0.0f);
	ImGui::DragFloat3("gravityNoWire", gravityNoWire_.num, 0.01f);
	ImGui::DragFloat3("gravityHaveWire", gravityHaveWire_.num, 0.01f);
	ImGui::DragFloat("limitAngle_", &limitAngle_, 1.0f);
	ImGui::DragFloat("sideForceValueNoWire", &sideForceValueNoWire_, 10.0f);
	ImGui::DragFloat("sideForceValueHaveWire", &sideForceValueHaveWire_, 10.0f);
	ImGui::DragFloat("upForceValue", &upSizeValue_, 0.05f);
	ImGui::DragFloat("maxUpForce", &maxUpSize_, 1.0f);
	ImGui::DragFloat("minSpeedVolume", &minSpeedVolume_, 1.0f);
	ImGui::DragFloat("downSpeedScale", &downSpeedValue_, 0.05f);
	ImGui::DragFloat("stiffness", &stiffness_, 0.5f);
	ImGui::DragFloat("dampingCoefficient_", &dampingCoefficient_, 0.05f);
	ImGui::Text("Timer %f", accelerationTimer_);
	ImGui::Text("isHitObj %d", isHitObj_);
	line_->SetLineThickness(lineThickness_);
	ImGui::End();
}

void Player::Draw(const ViewProjection viewProjection) {
	sphere_[0]->Draw(worldTransform_, viewProjection, sphereMaterial_, textureManager_->white);
	if (isSetWire_ == true || isMissWire_ == true) {
		line_->Draw(worldTransform2_, worldTransformWire_, viewProjection, lineMaterial_);
	}
}

void Player::DrawParticle(const ViewProjection viewProjection) {
	particle_->Draw(viewProjection);
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
	worldTransformGrapple_ = worldTransformWire_;
	start_ = Normalize(worldTransformGrapple_.translation_ - worldTransform2_.translation_); // ワイヤーを付けた時の根本から先端への単位ベクトル
}

void Player::SetWireMiss() {
	isMissWire_ = true;
	isSetWire_ = false;
	worldTransformWire_.translation_ = worldTransform2_.translation_;
	wireVelocity_ = worldTransformReticle_.translation_ - worldTransform2_.translation_;
	wireVelocity_ = Normalize(wireVelocity_) * 5.0f;
}