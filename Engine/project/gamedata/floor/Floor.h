#include "components/3d/Model.h"
#include "components/3d/WorldTransform.h"
#include "components/3d/ViewProjection.h"

class Floor
{
public:
	void Initialize();

	void Update();

	void Draw(const ViewProjection viewProjection);

private:
	std::unique_ptr<Model> floorModel_[15];
	WorldTransform worldTransform_[15];
	Vector4 modelMaterial_;
};

