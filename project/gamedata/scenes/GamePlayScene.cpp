#include "GamePlayScene.h"
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

	//Player
	player_ = new Player();
	player_->Initialize();

	//Linet
	line_ = std::make_unique <CreateLine>();
	line_->Initialize();
	line_->SetDirectionalLightFlag(false, 0);
	lineMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(&player_->GetWorldTransformPlayer());

	//
	model_.reset(Model::CreateModel("project/gamedata/resources/plane", "plane.gltf"));
	ObjModelData_ = model_->LoadModelFile("project/gamedata/resources/block", "block.obj");
	ObjTexture_ = textureManager_->Load(ObjModelData_.material.textureFilePath);

	for (int i = 0; i < objCountMax_; i++) {
		objNameHolder_[i] = "test" + std::to_string(i);
	}

	GlobalVariables* globalVariables{};
	globalVariables = GlobalVariables::GetInstance();

	GlobalVariables::GetInstance()->CreateGroup(groupName);

	globalVariables->AddItem(groupName, "ObjCount", objCount_);
}

void GamePlayScene::Update() {
	GlobalVariables* globalVariables{};
	globalVariables = GlobalVariables::GetInstance();
	ApplyGlobalVariables();

	collisionManager_->ClearColliders();
	collisionManager_->CheckAllCollision();

	debugCamera_->Update();
	followCamera_->Update();
	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
	viewProjection_.TransferMatrix();

	player_->Updete(viewProjection_);
	for (Obj& obj : objects_) {
		obj.world.UpdateMatrix();
	}

	if (input_->TriggerKey(DIK_D)) {
		if(showCursor == (int)true){
			showCursor = (int)false;
		}
		else {
			showCursor = (int)true;
		}
	}
	ShowCursor(showCursor);
	if (showCursor == 0) {
		SetCursorPos(1280 / 2, 720 / 2);
	}

	ImGui::Begin("debug");
	ImGui::Text("CorsorDemo:D key");
	ImGui::DragFloat("LineThickness", &lineThickness_, 0.05f, 0.0f);
	line_->SetLineThickness(lineThickness_);

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
		}
	}
	if (ImGui::Button("DeleteBlock")) {
		SetObject(EulerTransform{ { 1.0f,1.0f,1.0f }, {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} }, objName_);
		for (auto it = objects_.begin(); it != objects_.end();) {
			if (it->name == objName_) {
				globalVariables->RemoveItem(groupName, (std::string)objName_ + "Translate");
				globalVariables->RemoveItem(groupName, (std::string)objName_ + "Scale");
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

}

void GamePlayScene::Draw() {
#pragma region 背景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);

#pragma endregion

#pragma region 3Dオブジェクト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard3D);

	player_->Draw(viewProjection_);

	line_->Draw(player_->GetWorldTransformPlayer(), player_->GetWorldTransformReticle(), viewProjection_, lineMaterial_);

	for (Obj& obj : objects_) {
		obj.model.Draw(obj.world, viewProjection_, obj.material);
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


#pragma endregion

#pragma region 前景スプライト描画
	CJEngine_->renderer_->Draw(PipelineType::Standard2D);
	player_->DrawUI();

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