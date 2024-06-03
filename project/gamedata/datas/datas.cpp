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

void Datas::SetStarsEarned(int num, bool item, bool time, bool hit)
{
	if (!starsEarned_[num].item) {
		starsEarned_[num].item = item;
	}
	if (!starsEarned_[num].time) {
		starsEarned_[num].time = time;
	}
	if (!starsEarned_[num].hit) {
		starsEarned_[num].hit = hit;
	}

}
