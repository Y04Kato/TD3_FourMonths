#include "Player.h"
#include "GameSelectScene.h"

Player::Player() {

}

Player::~Player() {

}

void Player::Initialize() {
	textureManager_ = TextureManager::GetInstance();
	input_ = Input::GetInstance();

	//Audio
	audio_ = Audio::GetInstance();

	wireData_ = audio_->SoundLoad("project/gamedata/resources/sounds/Wire.mp3");
	wire2Data_ = audio_->SoundLoad("project/gamedata/resources/sounds/Wire2.mp3");
	hitData_ = audio_->SoundLoad("project/gamedata/resources/sounds/Hit.mp3");

	//テクスチャ
	uiResource_[0] = textureManager_->Load("project/gamedata/resources/UI/MoveUI.png");
	uiResource_[1] = textureManager_->Load("project/gamedata/resources/UI/meter.png");

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
	worldTransform_.rotation_ = { 1.5f,0.0f,2.0f };

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
	testEmitter_.transform.rotate = { -1.0f,-1.0f,-1.0f };
	testEmitter_.transform.scale = { 0.3f,0.3f,0.3f };
	testEmitter_.count = 15;
	testEmitter_.frequency = 0.05f;
	testEmitter_.frequencyTime = 0.0f;//発生頻度の時刻

	accelerationField_.acceleration = { 0.0f,0.0f,0.0f };
	accelerationField_.area.min = { -1.0f,-1.0f,-1.0f };
	accelerationField_.area.max = { 1.0f,1.0f,1.0f };

	spriteResource_ = textureManager_->Load("project/gamedata/resources/UI/star.png");

	particle_ = std::make_unique <CreateParticle>();

	particle_->Initialize(100, testEmitter_, accelerationField_, spriteResource_);
	//particle_->SetisVelocity(true);
	particle_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	particle_->SetLifeTime(10.0f);

	datas_ = Datas::GetInstance();

	model_.reset(Model::CreateSkinningModel("project/gamedata/resources/star", "star.obj"));
	modelMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

	DistancePlayerToReticle = kDistancePlayerToReticle;
}

void Player::Updete(const ViewProjection viewProjection) {
	if (cameraChange_ == true) {//DebugCamera

	}
	else {//FollowCamera
		Reticle(viewProjection);

		worldTransform2_.translation_ = worldTransform_.translation_;
		worldTransform2_.rotation_.num[1] += input_->GetMousePosition().Velocity.num[0] / sensitivity_.num[0];
		worldTransform2_.rotation_.num[0] += input_->GetMousePosition().Velocity.num[1] / sensitivity_.num[0];
		worldTransform2_.rotation_.num[1] = std::clamp(worldTransform2_.rotation_.num[1], cameraMin.num[1], cameraMax.num[1]);
		worldTransform2_.rotation_.num[0] = std::clamp(worldTransform2_.rotation_.num[0], cameraMin.num[0], cameraMax.num[0]);
	}

	worldTransform_.UpdateMatrix();
	worldTransform2_.UpdateMatrix();

	if (input_->pushMouse(MOUSE_BOTTON0) ) {//左クリックした時
		if (isFell_) {
			DistancePlayerToReticle = kDistancePlayerToReticleFell;
		}
		else {
			DistancePlayerToReticle = kDistancePlayerToReticle;
		}
		isHitObj_ = false;
		Reticle(viewProjection);
		if (isHitWire_ == true) {//レティクルがオブジェクト捉えていれば
			audio_->SoundPlayWave(wireData_, 0.1f, false);
			isFell_ = false;
			DistancePlayerToReticle = worldTransformObject_.translation_.num[2] - worldTransform2_.translation_.num[2];
			upSize_ = 0.0f; // 上昇量を初期化
			if (DistancePlayerToReticle <= 0) {
				DistancePlayerToReticle = -DistancePlayerToReticle + 5.0f;
			}
			Reticle(viewProjection);
			SetWire();
			if (!isActive_) {
				isActive_ = true;
			}
			if (speedUpCount_ < maxSpeedUp_) {
				speedUpCount_++;
			}
		}
		else {//レティクルがオブジェクトを捉えられていなければ
			if (isActive_) {
				audio_->SoundPlayWave(wire2Data_, 0.1f, false);
				speedUpCount_ = 0;
				SetWireMiss();
			}
		}
	}

	if (input_->pushMouse(MOUSE_BOTTON1)) {//右クリックした時
		isSetWire_ = false; // ワイヤーを外す
		DistancePlayerToReticle = kDistancePlayerToReticle;
	}

	if (isSetWire_ == true) {//ワイヤー成功時の演出
		DistancePlayerToReticle = kDistancePlayerToReticle;
		Reticle(viewProjection);
		isDownSpeed_ = true;
	}

	if (isMissWire_ == true) {//ワイヤー失敗時の演出
		missTimer_++;
		DistancePlayerToReticle = kDistancePlayerToReticle;
	}
	if (missTimer_ >= 15) {
		isMissWire_ = false;
		missTimer_ = 0;
	}
	
	if (isHitObj_ == true) {//接触時の演出
		HitTimer_++;
	}
	if (HitTimer_ >= 15) {
		isHitObj_ = false;
		HitTimer_ = 0;
	}

	/*if (input_->pushMouse(MOUSE_BOTTON0)) {
		isActive_ = true;
	}*/

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
		if (isRightMove_ == false) {
			worldTransform_.rotation_.num[2] += 0.1f;
		}
		else {
			worldTransform_.rotation_.num[2] -= 0.1f;
		}
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
				isRightMove_ = true;
			}
			else {
				isRightMove_ = false;
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

				if (forwad_.num[0] == 0.0f) {
					if (forwad_.num[2] == 1.0f) {
						startAngle_ = -90.0f * physics_->DegToRad();
					}
					else if (forwad_.num[2] == -1.0f) {
						startAngle_ = 90.0f * physics_->DegToRad();
					}
				}
				if (forwad_.num[2] == 0.0f) {
					if (forwad_.num[0] == 1.0f) {
						startAngle_ = 180.0f * physics_->DegToRad();
					}
					if (forwad_.num[0] == -1.0f) {
						startAngle_ = 0.0f * physics_->DegToRad();
					}
				}
			}

			if (leftRoll_) {
				angle_ += angularVelocity * physics_->deltaTime_;
			}
			else {
				angle_ -= angularVelocity * physics_->deltaTime_;
			}

			if (angle_ > 180.0f * physics_->DegToRad() || angle_ < -180.0f * physics_->DegToRad()) {
				isRoll_ = false;
				isSetWire_ = false;
				if (leftRoll_) {
					Matrix4x4 rotate = MakeRotateYMatrix(-90.0f * physics_->DegToRad());
					forwad_ = TransformNormal(forwad_, rotate);
					right_ = TransformNormal(right_, rotate);
				}
				else {
					Matrix4x4 rotate = MakeRotateYMatrix(90.0f * physics_->DegToRad());
					forwad_ = TransformNormal(forwad_, rotate);
					right_ = TransformNormal(right_, rotate);
				}
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
				isRightMove_ = true;
			}
			else {
				isRightMove_ = false;
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
					Vector3 force = Multiply(-downSpeedSize_, Normalize(dir));
					physics_->AddForce(force, 0);
					ImGui::DragFloat3("downSpeed", force.num, 1.0f);
				}
				else {
					// 初期化
					downSpeedSize_ = 0.0f;
				}
			}
		}
		if (upperLimit_ - worldTransform_.translation_.num[1] < 15.0f && !isRoll_) {
			float gravity = physics_->GetGravity().num[1];
			gravity *= 5.0f;
			physics_->SetGravity({ 0.0f, gravity, 0.0f });
		}
		Vector3 velocity = physics_->Update();
		if (!isRoll_) {
			float k = 1.0f + float(speedUpCount_) / 5.0f;
			velocity.num[0] *= k;
			velocity.num[2] *= k;

			worldTransform_.translation_ += velocity * physics_->deltaTime_;
			//Vector3 impulse = physics_->GetImpulse_();
			//worldTransform_.translation_ += impulse/* * physics_->deltaTime_*/;
			ImGui::DragFloat3("velocity", velocity.num, 0.05f);
			ImGui::DragFloat("k", &k);
			//ImGui::DragFloat3("impulse", impulse.num, 0.05f);
		}

		//physics_->Vector3Direction((velocity + impulse), &forwad_, &right_);
	}
	ImGui::End();

	//画面端
	//左右
	if (worldTransform_.translation_.num[0] >= 75.0f)
	{
		worldTransform_.translation_.num[0] = 75.0f;
		speedUpCount_ = 0;
	}

	if (worldTransform_.translation_.num[0] <= -75.0f)
	{
		worldTransform_.translation_.num[0] = -75.0f;
		speedUpCount_ = 0;
	}

	//後ろ
	if (worldTransform_.translation_.num[2] <= -5.0f)
	{
		worldTransform_.translation_.num[2] = -5.0f;
	}

	//床に落ちたとき
	if (worldTransform_.translation_.num[1] <= -3.0f)
	{
		worldTransform_.translation_.num[1] = -3.0f;
		physics_->SetVelocity({ 0.0f, 0.0f, 0.0f });
		Vector3 force = { 0.0f, 25.0f, 0.0f };
		physics_->AddForce(force, 1);
		isFell_ = true;
		speedUpCount_ = 0;
		DistancePlayerToReticle = kDistancePlayerToReticleFell;
	}

	//リスタート
	if (datas_->GetIsReset())
	{
		isRestart_ = true;
		isActive_ = false;
		isSetWire_ = false;
		isWireSet_ = false;
		isHitWire_ = false;
		isMissWire_ = false;
		isRoll_ = false;
		isFell_ = false;
		DistancePlayerToReticle = kDistancePlayerToReticle;
		physics_->SetVelocity({ 0.0f, 0.0f, 0.0f });
		datas_->SetIsReset(false);
		forwad_ = { 0.0f, 0.0f, 1.0f };
		right_ = { 1.0f, 0.0f, 0.0f };
	}

	//ゴール
	if (datas_->GetStageNum() == 1 && worldTransform_.translation_.num[2] >= 1700.0f)
	{
		isGoal_ = true;
		isActive_ = false;
		isSetWire_ = false;
		isWireSet_ = false;
		physics_->SetVelocity({ 0.0f, 0.0f, 0.0f });
		forwad_ = { 0.0f, 0.0f, 1.0f };
		right_ = { 1.0f, 0.0f, 0.0f };
	}
	else if (datas_->GetStageNum() == 2 && worldTransform_.translation_.num[2] >= 1700.0f)
	{
		isGoal_ = true;
		isActive_ = false;
		isSetWire_ = false;
		isWireSet_ = false;
		physics_->SetVelocity({ 0.0f, 0.0f, 0.0f });
		forwad_ = { 0.0f, 0.0f, 1.0f };
		right_ = { 1.0f, 0.0f, 0.0f };
	}
	else if (datas_->GetStageNum() == 3 && worldTransform_.translation_.num[2] >= 1700.0f)
	{
		isGoal_ = true;
		isActive_ = false;
		isSetWire_ = false;
		isWireSet_ = false;
		physics_->SetVelocity({ 0.0f, 0.0f, 0.0f });
		forwad_ = { 0.0f, 0.0f, 1.0f };
		right_ = { 1.0f, 0.0f, 0.0f };
	}
	else if (datas_->GetStageNum() == 4 && worldTransform_.translation_.num[2] >= 1700.0f)
	{
		isGoal_ = true;
		isActive_ = false;
		isSetWire_ = false;
		isWireSet_ = false;
		physics_->SetVelocity({ 0.0f, 0.0f, 0.0f });
		forwad_ = { 0.0f, 0.0f, 1.0f };
		right_ = { 1.0f, 0.0f, 0.0f };
	}
	else if (datas_->GetStageNum() == 5 && worldTransform_.translation_.num[2] >= 1710.0f)
	{
		isGoal_ = true;
		isActive_ = false;
		isSetWire_ = false;
		isWireSet_ = false;
		physics_->SetVelocity({ 0.0f, 0.0f, 0.0f });
		forwad_ = { 0.0f, 0.0f, 1.0f };
		right_ = { 1.0f, 0.0f, 0.0f };
	}
	else if (datas_->GetStageNum() == 6 && worldTransform_.translation_.num[2] >= 1710.0f)
	{
		isGoal_ = true;
		isActive_ = false;
		isSetWire_ = false;
		isWireSet_ = false;
		physics_->SetVelocity({ 0.0f, 0.0f, 0.0f });
		forwad_ = { 0.0f, 0.0f, 1.0f };
		right_ = { 1.0f, 0.0f, 0.0f };
	}

	ImGui::Begin("player");
	ImGui::DragFloat3("Pos", worldTransform_.translation_.num, 0.05f);
	ImGui::DragFloat3("Rot", worldTransform_.rotation_.num, 0.05f);
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
	ImGui::DragFloat("downSpeedValue", &downSpeedValue_, 0.05f);
	ImGui::DragFloat("maxDownSpeedSize", &maxDownSpeedSize_, 0.1f);
	ImGui::DragFloat("stiffness", &stiffness_, 0.5f);
	ImGui::DragFloat("dampingCoefficient_", &dampingCoefficient_, 0.05f);
	ImGui::Text("Timer %f", accelerationTimer_);
	ImGui::Text("isHitObj %d", isHitObj_);
	line_->SetLineThickness(lineThickness_);
	ImGui::End();
}

void Player::Draw(const ViewProjection viewProjection) {
	//sphere_[0]->Draw(worldTransform_, viewProjection, sphereMaterial_, textureManager_->white);
	model_->Draw(worldTransform_, viewProjection, modelMaterial_);
	if (isSetWire_ == true) {
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

void Player::SetWorldTransformCamera(const WorldTransform world) {
	worldTransform2_ = world;
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
		MakeViewportMatrix(0, 0, (float)WinApp::GetInstance()->GetClientWidth(), (float) WinApp::GetInstance()->GetClientHeight(), 0, 1);
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
	isWireParticle_ = true;
	isMissWire_ = false;
	isWireSet_ = true;

	worldTransformWire_.translation_ = Vector3{ worldTransformObject_.translation_.num[0],worldTransformReticle_.translation_.num[1] ,worldTransformObject_.translation_.num[2] };
	worldTransformGrapple_ = worldTransformWire_;
	start_ = Normalize(worldTransformGrapple_.translation_ - worldTransform2_.translation_); // ワイヤーを付けた時の根本から先端への単位ベクトル
}

void Player::SetWireMiss() {
	isMissWire_ = true;
	isSetWire_ = false;
	isWireSet_ = false;

	worldTransformWire_.translation_ = worldTransform2_.translation_;
	wireVelocity_ = worldTransformReticle_.translation_ - worldTransform2_.translation_;
	wireVelocity_ = Normalize(wireVelocity_) * 5.0f;
}

void Player::Shake(int shakePower, int dividePower) {
	worldTransform_.translation_.num[0] += (rand() % shakePower - shakePower / 2 + rand() / (float)RAND_MAX) / dividePower;
	worldTransform_.translation_.num[1] += (rand() % shakePower - shakePower / 2 + rand() / (float)RAND_MAX) / dividePower;
	worldTransform_.translation_.num[2] += (rand() % shakePower - shakePower / 2 + rand() / (float)RAND_MAX) / dividePower;
}