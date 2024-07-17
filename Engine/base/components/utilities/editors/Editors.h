#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "ImGuiManager.h"
#include "Model.h"

#include "components/utilities/globalVariables/GlobalVariables.h"

//ステージエディター擬き、名前をobj0 ~ objCountMax_で生成、それを記録する
struct Obj {
	Model model;
	WorldTransform world;
	Vector4 material;
	Vector4 Backmaterial;
	std::string name;
	OBB obb_;
	bool isHit;
	bool isHitEye;
	int treeMode;
};

class Editors {
public:
	static Editors* GetInstance();

	void Initialize();

	void SetModels(ModelData ObjModelData, uint32_t ObjTexture);

	void Update();

	void Draw(ViewProjection view);

	void Finalize();

	void ApplyGlobalVariables();
	void SetGlobalVariables();

	void SetObject(EulerTransform trans, const std::string& name);

	void InitializeGroupName(char* groupName);
	void SetGroupName(char* groupName) { decisionGroupName_ = groupName; }

	int GetObjCount() { return objCount_; }

	std::list<Obj> GetObj() { return objects_; }

	//Key1でTRANSLATE、key2でROTATE、key3でSCALE
	WorldTransform Guizmo(ViewProjection& view, WorldTransform world);

	//Key1でTRANSLATE、key2でROTATE、key3でSCALE
	EulerTransform Guizmo(ViewProjection& view, EulerTransform world);

private:
	Editors() = default;
	~Editors() = default;
	Editors(const Editors& obj) = default;
	Editors& operator=(const Editors& obj) = default;

	//ステージエディター擬き、名前をobj0~始め、それを記録する
	std::list<Obj> objects_;
	ModelData ObjModelData_;
	uint32_t ObjTexture_;
	static const int objCountMax_ = 100;
	int objCount_ = 0;
	std::string objNameHolder_[objCountMax_];

	char objName_[64];
	char groupName_[64];
	char* decisionGroupName_;
};