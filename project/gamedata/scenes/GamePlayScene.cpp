#include "GamePlayScene.h"
#include "GameSelectScene.h"
#include "components/utilities/globalVariables/GlobalVariables.h"

void GamePlayScene::Initialize() {
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

	uiResource_[0] = textureManager_->Load("project/gamedata/resources/UI/GoalUI.png");
	uiResource_[1] = textureManager_->Load("project/gamedata/resources/UI/ReturnUI.png");

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
		isSpriteDraw_[i] = false;
	}

	uiSprite_[0]->Initialize(Vector2{ 1280.0f,720.0f }, uiResource_[0]);
	uiSprite_[0]->SetAnchor(Vector2{ 0.5f,0.5f });

	uiSprite_[1]->Initialize(Vector2{ 1280.0f,720.0f }, uiResource_[1]);
	uiSprite_[1]->SetAnchor(Vector2{ 0.5f,0.5f });

	//Player
	player_ = new Player();
	player_->Initialize();

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

	testEmitter_.transform.translate = { 0.0f,0.0f,20.0f };
	testEmitter_.transform.rotate = { 0.0f,0.0f,0.0f };
	testEmitter_.transform.scale = { 1.0f,1.0f,1.0f };
	testEmitter_.count = 5;
	testEmitter_.frequency = 0.5f;//0.5秒ごとに発生
	testEmitter_.frequencyTime = 0.0f;//発生頻度の時刻

	accelerationField_.acceleration = { 0.0f,0.0f,-10.0f };
	accelerationField_.area.min = { -1.0f,-1.0f,-1.0f };
	accelerationField_.area.max = { 1.0f,1.0f,1.0f };

	particle_ = std::make_unique <CreateParticle>();

    particle_->Initialize(100, testEmitter_, accelerationField_, spriteResource_);

	GlobalVariables* globalVariables{};
	globalVariables = GlobalVariables::GetInstance();

	GlobalVariables::GetInstance()->CreateGroup(groupName);

	globalVariables->AddItem(groupName, "ObjCount", objCount_);

	//Line
	line_ = std::make_unique <CreateLine>();
	line_->Initialize();
	line_->SetDirectionalLightFlag(false, 0);
	line_->SetLineThickness(0.2f);
}

void GamePlayScene::Update() {
	GlobalVariables* globalVariables{};
	globalVariables = GlobalVariables::GetInstance();
	ApplyGlobalVariables();

	if (isGameStart_ == true) {//ゲーム開始時の処理
		for (int i = 0; i < objCount_; i++) {
			SetObject(EulerTransform{ { 1.0f,1.0f,1.0f }, {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} }, objNameHolder_[i]);
		}
		isGameStart_ = false;
	}

	//Goal
	if (input_->TriggerKey(DIK_G))
	{
		isGoal_ = true;
	}

	if (input_->TriggerKey(DIK_R))
	{
		isGoal_ = false;
	}

	if (isGoal_)
	{
		for (int i = 0; i < 2; i++)
		{
			isSpriteDraw_[i] = true;
		}
	}
	else
	{
		for (int i = 0; i < 2; i++)
		{
			isSpriteDraw_[i] = false;
		}
	}

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

	for (Obj& obj : objects_) {
		obj.world.UpdateMatrix();
	}

	//レイの設定
	segment_.origin = player_->GetWorldTransformPlayer().translation_;
	segment_.diff = player_->GetWorldTransformReticle().translation_ - player_->GetWorldTransformPlayer().translation_;

	for (Obj& obj : objects_) {//レイとオブジェクトの当たり判定
		obj.obb_.center = obj.world.translation_;
		GetOrientations(MakeRotateXYZMatrix(obj.world.rotation_), obj.obb_.orientation);
		obj.obb_.size = obj.world.scale_;
		if (IsCollision(obj.obb_, segment_)) {
			player_->SetWorldTransformObject(obj.world);
			player_->SetIsHit(true);
			isHit_ = true;
		}
		else {

		}
	}

	if (isHit_ == true) {//レイがヒットしている時
		resetTime_++;
	}
	if (resetTime_ >= 30) {
		isHit_ = false;
		player_->SetIsHit(false);
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
		if (showCursor == (int)true) {
			showCursor = (int)false;
		}
		else {
			showCursor = (int)true;
		}
	}
	ShowCursor(showCursor);//カーソル表示設定関数
	if (showCursor == 0) {//カーソル非表示時、カーソルの座標を画面中央に固定
		SetCursorPos(1280 / 2, 720 / 2);
	}

	ImGui::Begin("debug");
	ImGui::Text("CameraChange:Z key");
	ImGui::Text("CorsorDemo:X key");
	ImGui::Text("IsHitRay %d", isHit_);

	ImGui::InputText("BlockName", objName_, sizeof(objName_));
	if (ImGui::Button("SpawnBlock")) {
		SetObject(EulerTransform{ { 1.0f,1.0f,1.0f }, {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} }, objName_);
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
		SetObject(EulerTransform{ { 1.0f,1.0f,1.0f }, {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} }, objName_);
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
			SetObject(EulerTransform{ { 1.0f,1.0f,1.0f }, {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} }, objNameHolder_[i]);
		}
	}

	ImGui::End();

	ImGui::Begin("StageNum");
	ImGui::Text("stgaeNum %d", GameSelectScene::stageNum);
	ImGui::End();

}

void GamePlayScene::Draw() {
#pragma region 背景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);

	sprite_->Draw(spriteTransform_, SpriteuvTransform_, spriteMaterial_);

#pragma endregion

#pragma region 3Dオブジェクト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard3D);

	player_->Draw(viewProjection_);

	skydome_->Draw(viewProjection_);

	//line_->Draw(player_->GetWorldTransformPlayer(),player_->GetWorldTransformReticle(), viewProjection_, Vector4{ 1.0f,1.0f,1.0f,1.0f });

	for (Obj& obj : objects_) {
		obj.model.Draw(obj.world, viewProjection_, obj.material);
	}

	mountain_->Draw(viewProjection_);

	goal_->Draw(viewProjection_);

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

	particle_->Draw(viewProjection_);

#pragma endregion

#pragma region 前景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);

	if (!isGoal_)
	{
		player_->DrawUI();
	}
	else
	{
		for (int i = 0; i < 2; i++)
		{
			if (isSpriteDraw_[i])
			{
				//Sprite描画
				uiSprite_[i]->Draw(uiSpriteTransform_[i], uiSpriteuvTransform_[i], uiSpriteMaterial_[i]);
			}
		}
	}

#pragma endregion
}

void GamePlayScene::Finalize() {
	delete player_;

	objects_.clear();
}

void GamePlayScene::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "GamePlayScene";

	objCount_ = globalVariables->GetIntValue(groupName, "ObjCount");

	for (Obj& obj : objects_) {
		obj.world.translation_ = globalVariables->GetVector3Value(groupName, obj.name + "Translate");
		//obj.world.rotation_ = globalVariables->GetVector3Value(groupName,  obj.name + "Rotate");
		obj.world.scale_ = globalVariables->GetVector3Value(groupName, obj.name + "Scale");
		obj.material = globalVariables->GetVector4Value(groupName, obj.name + "Material");
	}
}

void GamePlayScene::SetObject(EulerTransform trans, const std::string& name) {
	Obj obj;
	obj.model.Initialize(ObjModelData_, ObjTexture_);
	obj.model.SetDirectionalLightFlag(true, 3);

	obj.world.Initialize();
	obj.world.translation_ = trans.translate;
	obj.world.rotation_ = trans.rotate;
	obj.world.scale_ = trans.scale;

	obj.material = { 1.0f,1.0f,1.0f,1.0f };

	obj.name = name;
	objects_.push_back(obj);
}