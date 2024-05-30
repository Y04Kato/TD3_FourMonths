#include "GamePlayScene2.h"
#include "GameSelectScene.h"
#include "components/utilities/globalVariables/GlobalVariables.h"

void GamePlayScene2::Initialize() {
	CJEngine_ = CitrusJunosEngine::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager_ = TextureManager::GetInstance();

	//Input
	input_ = Input::GetInstance();

	//Audio
	audio_ = Audio::GetInstance();

	// デバッグカメラの初期化
	debugCamera_ = DebugCamera::GetInstance();
	debugCamera_->initialize();

	viewProjection_.Initialize();

	//CollisionManager
	collisionManager_ = CollisionManager::GetInstance();

	//テクスチャ
	spriteResource_ = textureManager_->Load("project/gamedata/resources/UI/bg.png");

	uiResource_[0] = textureManager_->Load("project/gamedata/resources/reticle2.png");
	uiResource_[1] = textureManager_->Load("project/gamedata/resources/UI/StartUI.png");
	uiResource_[2] = textureManager_->Load("project/gamedata/resources/UI/Manu.png");
	uiResource_[3] = textureManager_->Load("project/gamedata/resources/UI/Cursor.png");
	uiResource_[4] = textureManager_->Load("project/gamedata/resources/UI/stage2.png");

	//testSprite
	spriteMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
	spriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f} };

	SpriteuvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	sprite_ = std::make_unique <CreateSprite>();

	sprite_->Initialize(Vector2{ 1280.0f,780.0f }, spriteResource_);
	sprite_->SetAnchor(Vector2{ 0.5f,0.5f });

	//uiSprite
	for (int i = 0; i < 5; i++)
	{
		uiSpriteMaterial_[i] = { 1.0f,1.0f,1.0f,1.0f };
		uiSpriteTransform_[i] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{1280 / 2.0f,720 / 2.0f,0.0f} };

		uiSpriteuvTransform_[i] = {
			{1.0f,1.0f,1.0f},
			{0.0f,0.0f,0.0f},
			{0.0f,0.0f,0.0f},
		};

		uiSprite_[i] = std::make_unique <CreateSprite>();
		isSpriteDraw_[i] = false;
	}

	uiSprite_[0]->Initialize(Vector2{ 1280.0f,720.0f }, uiResource_[0]);
	uiSprite_[0]->SetTextureInitialSize();
	uiSprite_[0]->SetAnchor(Vector2{ 0.5f,0.5f });
	uiSpriteTransform_[0].translate.num[1] = 415.0f;
	uiSpriteTransform_[0].scale = { 0.8f,0.8f,0.8f };

	uiSprite_[1]->Initialize(Vector2{ 1280.0f,720.0f }, uiResource_[1]);
	uiSprite_[1]->SetAnchor(Vector2{ 0.5f,0.5f });

	uiSprite_[2]->Initialize(Vector2{ 1280.0f,720.0f }, uiResource_[2]);
	uiSprite_[2]->SetAnchor(Vector2{ 0.5f,0.5f });

	uiSprite_[3]->Initialize(Vector2{ 1280.0f,720.0f }, uiResource_[3]);
	uiSprite_[3]->SetAnchor(Vector2{ 0.5f,0.5f });

	uiSprite_[4]->Initialize(Vector2{ 1280.0f,720.0f }, uiResource_[4]);
	uiSprite_[4]->SetAnchor(Vector2{ 0.5f,0.5f });

	//Player
	player_ = new Player();
	player_->Initialize();

	structSphere_.radius = 1.0f;

	structSphereTree_.radius = 1.5f;

	//Skydome
	skydome_ = new Skydome();
	skydome_->Initialize();

	//Mountain
	mountain_ = new Mountain();
	mountain_->Initialize();

	//Goal
	goal_ = new Goal();
	goal_->Initialize();

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(&player_->GetWorldTransformPlayer());

	//
	model_.reset(Model::CreateModel("project/gamedata/resources/block", "block.obj"));
	ObjModelData_ = model_->LoadModelFile("project/gamedata/resources/models/tree", "tree.obj");
	ObjTexture_ = textureManager_->Load(ObjModelData_.material.textureFilePath);

	for (int i = 0; i < objCountMax_; i++) {
		objNameHolder_[i] = "test" + std::to_string(i);
	}

	testEmitter_.transform.translate = { 0.0f,0.0f,45.0f };
	testEmitter_.transform.rotate = { 0.0f,0.0f,0.0f };
	testEmitter_.transform.scale = { 1.0f,1.0f,1.0f };
	testEmitter_.count = 10;
	testEmitter_.frequency = 0.05f;
	testEmitter_.frequencyTime = 0.0f;//発生頻度の時刻

	accelerationField_.acceleration = { 0.0f,0.0f,-10.0f };
	accelerationField_.area.min = { -1.0f,-1.0f,-1.0f };
	accelerationField_.area.max = { 1.0f,1.0f,1.0f };

	particle_ = std::make_unique <CreateParticle>();

	particle_->Initialize(1000, testEmitter_, accelerationField_, spriteResource_);
	particle_->SetisVelocity(true);

	//Timer
	numbers_ = std::make_unique<Numbers>();
	numbers_->Initialize();
	numbers_->SetInitialNum(0 / 60);
	numbersTransform_.scale = { 1.0f,1.0f,1.0f };
	numbersTransform_.rotate = { 0.0f,0.0f,0.0f };
	numbersTransform_.translate = { 430.0f,50.0f,0.0f };

	numbers2_ = std::make_unique<Numbers>();
	numbers2_->Initialize();
	numbers2_->SetInitialNum(0);
	numbers2_->SetSpace(-35);
	numbersTransform2_.scale = { 0.7f,0.7f,0.7f };
	numbersTransform2_.rotate = { 0.0f,0.0f,0.0f };
	numbersTransform2_.translate = { 550.0f,395.0f,0.0f };

	GlobalVariables* globalVariables{};
	globalVariables = GlobalVariables::GetInstance();

	GlobalVariables::GetInstance()->CreateGroup(groupName);

	globalVariables->AddItem(groupName, "ObjCount", objCount_);

	startWorldTransform_.Initialize();
	startWorldTransform_.rotation_ = { 1.5f,0.0f,2.0f };

	datas_ = Datas::GetInstance();
	datas_->Initialize();
}

void GamePlayScene2::Update() {
	GlobalVariables* globalVariables{};
	globalVariables = GlobalVariables::GetInstance();
	ApplyGlobalVariables();

	datas_->SetStageNum(2);

	if (isGameStart_ == true) {//ゲーム開始時の処理
		uiSpriteTransform_[3].translate.num[0] = 1048.0f;
		uiSpriteTransform_[3].translate.num[1] = 324.0f;
		for (int i = 0; i < objCount_; i++) {
			SetObject(EulerTransform{ { 4.0f,30.0f,4.0f }, {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} }, objNameHolder_[i]);
		}
		isGameStart_ = false;
		input_->HideCursor();
	}

	if (input_->TriggerKey(DIK_TAB) && !datas_->GetIsPause())
	{
		datas_->SetIsPause(true);
	}
	else if (input_->TriggerKey(DIK_TAB) && datas_->GetIsPause())
	{
		datas_->SetIsPause(false);
	}


	if (datas_->GetIsPause())
	{
		//カーソル移動の処理
		if (input_->TriggerKey(DIK_S) && uiSpriteTransform_[3].translate.num[1] == 520.0f && !datas_->GetIsRule())
		{
			uiSpriteTransform_[3].translate.num[1] = 717.0f;
		}

		if (input_->TriggerKey(DIK_S) && uiSpriteTransform_[3].translate.num[1] == 324.0f && !datas_->GetIsRule())
		{
			uiSpriteTransform_[3].translate.num[1] = 520.0f;
		}

		if (input_->TriggerKey(DIK_W) && uiSpriteTransform_[3].translate.num[1] == 520.0f && !datas_->GetIsRule())
		{
			uiSpriteTransform_[3].translate.num[1] = 324.0f;
		}

		if (input_->TriggerKey(DIK_W) && uiSpriteTransform_[3].translate.num[1] == 717.0f && !datas_->GetIsRule())
		{
			uiSpriteTransform_[3].translate.num[1] = 520.0f;
		}

		//選択する処理
		if (input_->TriggerKey(DIK_SPACE) && uiSpriteTransform_[3].translate.num[1] == 324.0f)
		{
			datas_->SetIsPause(false);
			datas_->SetIsReset(true);
			startWorldTransform_.translation_ = { 0.0f,20.0f,0.0f };
			player_->SetWorldTransform(startWorldTransform_);
			nowTime_ = 0;
		}

		if (input_->TriggerKey(DIK_SPACE) && uiSpriteTransform_[3].translate.num[1] == 520.0f)
		{
			datas_->SetIsPause(false);
			datas_->SetIsReset(true);
			sceneNo = SELECT_SCENE;
			startWorldTransform_.translation_ = { 0.0f,20.0f,0.0f };
			player_->SetWorldTransform(startWorldTransform_);
			nowTime_ = 0;
			input_->ViewCursor();
		}
	}
	else
	{
		uiSpriteTransform_[3].translate.num[0] = 1048.0f;
		uiSpriteTransform_[3].translate.num[1] = 324.0f;
		//Goal
		if (player_->GetIsGoal())
		{
			sceneNo = CLEAR_SCENE;
			startWorldTransform_.translation_ = { 0.0f,20.0f,0.0f };
			player_->SetWorldTransform(startWorldTransform_);
			player_->SetIsGoal(false);
			datas_->SetClearTime(nowTime_);
			nowTime_ = 0;
			input_->ViewCursor();
		}

		startWorldTransform_.UpdateMatrix();

		//操作形式が一部変わるのでCameraChange変数をPlayerにも送る
		player_->SetCameraMode(cameraChange_);

		mountain_->Update();

		skydome_->Update();

		goal_->Update();

		particle_->Update();
		particle_->SetEmitter(testEmitter_);
		particle_->SetAccelerationField(accelerationField_);
		particle_->SetisBillBoard(isBillBoard_);

		//色を固定するならこれを使う
		particle_->SetisColor(isColor_);
		particle_->SetColor(particleColor_);

		//Timer
		if (player_->GetIsActive() == true) {
			nowTime_++;
		}

		numbers_->SetNum(nowTime_ / 60);
		numbers_->SetTransform(numbersTransform_);

		nowCount_ = (int)player_->GetWorldTransformPlayer().translation_.num[1] + 3;
		numbers2_->SetNum(nowCount_);
		numbers2_->SetTransform(numbersTransform2_);
		if (nowCount_ <= 10 && nowCount_ > 5) {
			numbers2_->SetColor({ 1.0f,1.0f,0.0f,1.0f });
		}
		else if (nowCount_ <= 5) {
			numbers2_->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		}
		else {
			numbers2_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		}

		if (cameraChange_ == true) {//DebugCamera
			debugCamera_->Update();
			viewProjection_.translation_ = debugCamera_->GetViewProjection()->translation_;
			viewProjection_.rotation_ = debugCamera_->GetViewProjection()->rotation_;
			viewProjection_.UpdateMatrix();
		}
		else {//FollowCamera
			followCamera_->Update();
			viewProjection_.translation_ = followCamera_->GetViewProjection().translation_;
			viewProjection_.matView = followCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();
		}

		player_->Updete(viewProjection_);
	}


	for (Obj& obj : objects_) {
		obj.world.UpdateMatrix();
	}

	//レイの設定
	segmentRay_.origin = player_->GetWorldTransformPlayer().translation_;
	segmentRay_.diff = player_->GetWorldTransformReticle().translation_ - player_->GetWorldTransformPlayer().translation_;

	segmentEye_.origin = viewProjection_.translation_;
	segmentEye_.diff = player_->GetWorldTransformPlayer().translation_ - viewProjection_.translation_;

	structSphere_.center = player_->GetWorldTransform().translation_;

	for (Obj& obj : objects_) {
		obj.obb_.center = obj.world.translation_;
		GetOrientations(MakeRotateXYZMatrix(obj.world.rotation_), obj.obb_.orientation);
		obj.obb_.size = obj.world.scale_;
		if (IsCollision(obj.obb_, segmentRay_)) {//レイ(自機~レティクル)とオブジェクトの当たり判定
			player_->SetWorldTransformObject(obj.world);
			player_->SetIsHitWire(true);
			isHitWire_ = true;
			obj.isHit = true;
		}
		else {
			obj.isHit = false;
		}

		if (IsCollision(obj.obb_, segmentEye_)) {//レイ(視点~自機)とオブジェクトの当たり判定
			obj.isHitEye = true;
		}
		else {
			obj.isHitEye = false;
		}

		//Player用に当たり判定を小さくする
		obj.obb_.size.num[0] = obj.world.scale_.num[0] * 0.5f;
		obj.obb_.size.num[2] = obj.world.scale_.num[2] * 0.5f;
		if (IsCollision(obj.obb_, structSphere_)) {//Playerとオブジェクトの当たり判定
			if (player_->GetIsHitObj() == false) {
				isHitPlayer_ = true;
				player_->SetIsHitObj(isHitPlayer_);
				std::pair<Vector3, Vector3> pair = ComputeCollisionVelocities(1.0f, player_->GetVelocity(), 1.0f, Vector3{ 0.0f,0.0f,0.0f }, 0.8f, Normalize(player_->GetWorldTransform().GetWorldPos() - obj.world.translation_));
				player_->SetVelocity(-pair.first * 20.0f);
			}
		}
		else {
		}
	}

	if (isHitWire_ == true) {//レイがヒットしている時
		resetTime_++;
	}
	if (resetTime_ >= 30) {
		isHitWire_ = false;
		player_->SetIsHitWire(false);
		resetTime_ = 0;
	}

	if (input_->TriggerKey(DIK_Z)) {//Zkeyでカメラモードの変更Follow <=> Debugへ
		if (cameraChange_ == true) {
			cameraChange_ = false;
		}
		else {
			cameraChange_ = true;
		}
	}

	if (input_->TriggerKey(DIK_X)) {//Xkeyでカーソル表示変更
		input_->ToggleCursor();
	}

	for (Obj& obj : objects_) {//レイとオブジェクトの当たり判定の結果
		if (obj.isHit == true) {
			obj.material = { 1.0f,0.5f,0.0f,1.0f };
		}
		else {
			obj.material = obj.Backmaterial;
		}

		if (obj.isHitEye == true) {
			obj.material.num[3] = 0.2f;
		}
		else {
			obj.material.num[3] = 1.0f;
		}
	}

	uiSpriteTransform_->rotate.num[2] += 0.05f;

	if (showCursor == 0) {//カーソル非表示時、カーソルの座標を画面中央に固定
		SetCursorPos(1280 / 2, 720 / 2);
	}

	ImGui::Begin("debug");
	ImGui::Text("CameraChange:Z key");
	ImGui::Text("CorsorDemo:X key");
	ImGui::Text("IsHitRay %d", isHitWire_);

	ImGui::InputText("BlockName", objName_, sizeof(objName_));
	if (ImGui::Button("SpawnBlock")) {
		SetObject(EulerTransform{ { 4.0f,30.0f,4.0f }, {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} }, objName_);
		objCount_++;
		globalVariables->SetValue(groupName, "ObjCount", objCount_);
		for (Obj& obj : objects_) {
			globalVariables->AddItem(groupName, obj.name, (std::string)objName_);
			globalVariables->AddItem(groupName, obj.name + "Translate", obj.world.translation_);
			//globalVariables->AddItem(groupName,obj.name + "Rotate", obj.world.rotation_);
			globalVariables->AddItem(groupName, obj.name + "Scale", obj.world.scale_);
			globalVariables->AddItem(groupName, obj.name + "Material", obj.material);
		}
	}
	if (ImGui::Button("DeleteBlock")) {
		SetObject(EulerTransform{ { 4.0f,30.0f,4.0f }, {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} }, objName_);
		for (auto it = objects_.begin(); it != objects_.end();) {
			if (it->name == objName_) {
				globalVariables->RemoveItem(groupName, (std::string)objName_ + "Translate");
				globalVariables->RemoveItem(groupName, (std::string)objName_ + "Scale");
				globalVariables->RemoveItem(groupName, (std::string)objName_ + "Material");
				objCount_--;
				globalVariables->SetValue(groupName, "ObjCount", objCount_);
				it = objects_.erase(it);
			}
			else {
				++it;
			}
		}
	}
	if (ImGui::Button("StartSetBlock")) {
		for (int i = 0; i < objCount_; i++) {
			SetObject(EulerTransform{ { 4.0f,30.0f,4.0f }, {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} }, objNameHolder_[i]);
		}
	}

	ImGui::End();

	ImGui::Begin("Play");
	//ImGui::Text("stgaeNum %d", GameSelectScene::stageNum);
	ImGui::Checkbox("UseBillBoard", &isBillBoard_);
	ImGui::DragFloat3("EmitterTranslate", testEmitter_.transform.translate.num, 0.1f);
	ImGui::DragFloat3("AccelerationField", accelerationField_.acceleration.num, 0.1f);
	ImGui::DragFloat3("OccurrenceRangeMin", accelerationField_.area.min.num, 0.1f);
	ImGui::DragFloat3("OccurrenceRangeMax", accelerationField_.area.max.num, 0.1f);
	ImGui::DragFloat("frequency", &testEmitter_.frequency, 0.1f);
	ImGui::DragFloat("frequencyTime", &testEmitter_.frequencyTime, 0.1f);
	ImGui::End();

	ImGui::Begin("Cursor");
	ImGui::DragFloat3("CursorWTF", &uiSpriteTransform_[3].translate.num[0], 1.0f);
	ImGui::End();
}

void GamePlayScene2::Draw() {
#pragma region 背景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);

	sprite_->Draw(spriteTransform_, SpriteuvTransform_, spriteMaterial_);

#pragma endregion

#pragma region 3Dオブジェクト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard3D);

	player_->Draw(viewProjection_);

	skydome_->Draw(viewProjection_);

	goal_->Draw(viewProjection_);

	for (Obj& obj : objects_) {
		obj.model.Draw(obj.world, viewProjection_, obj.material);
	}

	mountain_->Draw(viewProjection_);

	for (Obj& obj : objects_) {
#ifdef _DEBUG
		model_->Draw(obj.world, viewProjection_, Vector4{ 1.0f,1.0f,1.0f,0.1f });
#endif // _DEBUG
	}

#pragma endregion

#pragma region Skinningモデル描画
	CJEngine_->renderer_->Draw(PipelineType::Skinning);


#pragma endregion

#pragma region VATモデル描画
	CJEngine_->renderer_->Draw(PipelineType::VertexAnimationTexture);


#pragma endregion

#pragma region パーティクル描画
	CJEngine_->renderer_->Draw(PipelineType::Particle);
	player_->DrawParticle(viewProjection_);
	//particle_->Draw(viewProjection_);

#pragma endregion

#pragma region 前景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);

	uiSprite_[4]->Draw(uiSpriteTransform_[4], uiSpriteuvTransform_[4], uiSpriteMaterial_[4]);
	
	if (datas_->GetIsPause())
	{
		uiSprite_[2]->Draw(uiSpriteTransform_[2], uiSpriteuvTransform_[2], uiSpriteMaterial_[2]);

		uiSprite_[3]->Draw(uiSpriteTransform_[3], uiSpriteuvTransform_[3], uiSpriteMaterial_[3]);

		if (datas_->GetIsRule())
		{
			numbers_->Draw();
		}
	}
	else
	{
		numbers_->Draw();
		numbers2_->Draw();
		player_->DrawUI();
		if (isHitWire_ == true) {
			uiSprite_[0]->Draw(uiSpriteTransform_[0], uiSpriteuvTransform_[0], uiSpriteMaterial_[0]);
		}
		if (player_->GetIsActive() == false) {
			uiSprite_[1]->Draw(uiSpriteTransform_[1], uiSpriteuvTransform_[1], uiSpriteMaterial_[1]);
		}

	}

#pragma endregion
}

void GamePlayScene2::Finalize() {
	delete player_;

	objects_.clear();
}

void GamePlayScene2::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	objCount_ = globalVariables->GetIntValue(groupName, "ObjCount");

	for (Obj& obj : objects_) {
		obj.world.translation_ = globalVariables->GetVector3Value(groupName, obj.name + "Translate");
		//obj.world.rotation_ = globalVariables->GetVector3Value(groupName,  obj.name + "Rotate");
		obj.world.scale_ = globalVariables->GetVector3Value(groupName, obj.name + "Scale");
		obj.Backmaterial = globalVariables->GetVector4Value(groupName, obj.name + "Material");
	}
}

void GamePlayScene2::SetObject(EulerTransform trans, const std::string& name) {
	Obj obj;
	obj.model.Initialize(ObjModelData_, ObjTexture_);
	obj.model.SetDirectionalLightFlag(true, 3);

	obj.world.Initialize();
	obj.world.translation_ = trans.translate;
	obj.world.rotation_ = trans.rotate;
	obj.world.scale_ = trans.scale;

	obj.isHit = false;
	obj.isHitEye = false;

	obj.material = { 1.0f,1.0f,1.0f,1.0f };
	obj.Backmaterial = { 1.0f,1.0f,1.0f,1.0f };

	obj.name = name;
	objects_.push_back(obj);
}