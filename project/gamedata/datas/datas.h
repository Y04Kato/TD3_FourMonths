#pragma once
#include "Vector.h"

class Datas {
public:
	static Datas* GetInstance();

	void Initialize();

	void SetClearTime(const float clearTime) { clearTime_ = clearTime; }
	const float& GetClearTime() { return clearTime_; }

	void SetStageNum(const int stageNum) { stageNum_ = stageNum; }
	const int& GetStageNum() { return stageNum_; }

	void SetIsPause(const bool isPause) { isPause_ = isPause; }
	const bool& GetIsPause() { return isPause_; }

	void SetIsRule(const bool isRule) { isRule_ = isRule; }
	const bool& GetIsRule() { return isRule_; }

	void SetIsReset(bool isReset) { isReset_ = isReset; }
	bool GetIsReset() { return isReset_; }

	void SetUIResrouce(VectorInt4 uiResrouce) { uiResrouce_ = uiResrouce; }
	bool GetUIResrouce() { return isReset_; }

	void SetItem(int getItem) { getItem_ = getItem; }
	int GetItem() { return getItem_; }

	void SetFell(bool fell) { fell_ = fell; }
	bool GetFell() { return fell_; }

private:
	Datas() = default;
	~Datas() = default;
	Datas(const Datas& obj) = default;
	Datas& operator=(const Datas& obj) = default;

	int getItem_;
	float clearTime_;
	bool fell_;

	int stageNum_ = 0;

	bool isPause_ = false;

	bool isRule_ = false;

	bool isReset_ = false;

	VectorInt4 uiResrouce_ = { 0,0,0,0 };

	
};

