#pragma once

class StartManager
{
public:
	static StartManager& instance()
	{
		static StartManager instance;
		return instance;
	}

	const bool GetStartState()const { return m_startFlg; }
	void SetStartState(const bool _start) { m_startFlg = _start; }

private:
	StartManager() = default;
	~StartManager() = default;
	std::mutex m_mutex; // データの一貫性を保つ

	bool m_startFlg = false;
};