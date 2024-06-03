#include "datas.h"

Datas* Datas::GetInstance() {
	static Datas instance;

	return &instance;
}

void Datas::Initialize() {
	for (int index = 0; index < 6; index++) {
		starsEarned_[index].item = false;
		starsEarned_[index].time = false;
		starsEarned_[index].hit = false;
	}
}
