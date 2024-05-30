#pragma once
#include "Iscene.h"

class GameDemoScene :public Iscene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	
};

