#pragma once
#include "Iscene.h"

class GameClearScene :public Iscene {
public:
	void Initialize() override;
	void DrawPostEffect() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
};