#pragma once

class Datas {
public:
	static Datas* GetInstance();

	void Initialize();

	void SetClearTime(const int clearTime) { clearTime_ = clearTime; }
	const int& GetClearTime() { return clearTime_; }

	void SetStageNum(const int stageNum) { stageNum_ = stageNum; }
	const int& GetStageNum() { return stageNum_; }

private:
	Datas() = default;
	~Datas() = default;
	Datas(const Datas& obj) = default;
	Datas& operator=(const Datas& obj) = default;

	int clearTime_;

	int stageNum_ = 0;
};

