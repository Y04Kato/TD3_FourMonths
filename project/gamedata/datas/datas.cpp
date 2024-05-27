#include "datas.h"

Datas* Datas::GetInstance() {
	static Datas instance;

	return &instance;
}

void Datas::Initialize() {
	clearTime_ = 0;
}