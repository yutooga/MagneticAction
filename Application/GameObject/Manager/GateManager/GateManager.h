#pragma once

class GateManager
{
public:
	static GateManager& Instance()
	{
		static GateManager instance;
		return instance;
	}

	enum class State
	{
		OpenAnimation,
		Open,
		BackPlayerAnimation,
		NoticeAnimation,
		NotStart,
		AllEnd
	};

	//現在の状況取得
	const State GetNowState() const { return m_nowState; }
	// 状態変更関数
	void SetState(const State _nextState) { m_nowState = _nextState; }

	// 電磁誘導の状態を取得する関数
	const bool GetElectromagneticInduction()const { return m_electromagneticInductionFlg; }

	// 電磁誘導の状態を変更する関数
	void SetElectromagneticInduction(const bool _state) { m_electromagneticInductionFlg = _state; }

	//扉の開閉状態取得
	const bool GetOpenState() const{ return m_openFlg; }
	//扉の開閉状態設定関数
	void SetOpenState(const bool _state) { m_openFlg = _state; }

private:

	GateManager() = default;
	~GateManager() = default;
	std::mutex m_mutex; // データの一貫性を保つ

	//扉開閉フラグ
	bool m_openFlg = false;

	// 電磁誘導しているかどうか判断するフラグ
	bool m_electromagneticInductionFlg = false;

	//現在の状態
	State m_nowState = State::NotStart;
};