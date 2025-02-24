#pragma once

class ScoreManager
{
public:

	static ScoreManager& instance()
	{
		static ScoreManager instance;
		return instance;
	}

	void ConversionScore();
	void RegisterClearHp(const int _hp) { m_hp = _hp; }

	const int GetScore() const { return m_score; }
	const int GetClearHp()const { return m_hp; }

private:
	ScoreManager(){}
	~ScoreManager(){}

	std::mutex m_mutex; // データの一貫性を保つ

	enum class RANK
	{
		S,
		A,
		B,
		C,
		D
	};

	enum class ClearHp
	{
		SRANK = 4,
		ARANK = 3,
		BRANK = 2,
		CRANK = 1,
	};

	//スコア記録用変数
	int m_score = 0;

	//クリア時のHP数
	int m_hp = 0;
};