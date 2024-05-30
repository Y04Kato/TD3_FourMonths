#pragma once

class Datas {
public:
	static Datas* GetInstance();

	void Initialize();

	void SetClearTime(const int clearTime) { clearTime_ = clearTime; }
	const int& GetClearTime() { return clearTime_; }

	void SetStageNum(const int stageNum) { stageNum_ = stageNum; }
	const int& GetStageNum() { return stageNum_; }

	void SetIsPause(const bool isPause) { isPause_ = isPause; }
	const bool& GetIsPause() { return isPause_; }

	void SetIsRule(const bool isRule) { isRule_ = isRule; }
	const bool& GetIsRule() { return isRule_; }

	void SetIsReset(bool isReset) { isReset_ = isReset; }
	bool GetIsReset() { return isReset_; }

private:
	Datas() = default;
	~Datas() = default;
	Datas(const Datas& obj) = default;
	Datas& operator=(const Datas& obj) = default;

	int clearTime_;

	int stageNum_ = 0;

	bool isPause_ = false;

	bool isRule_ = false;

	bool isReset_ = false;
};

