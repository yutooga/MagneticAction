#include "ScoreManager.h"

void ScoreManager::ConversionScore()
{
	// 残りHPに応じてランクを振り分ける

	if (m_hp > static_cast<int>(ClearHp::SRANK))
	{
		m_score = static_cast<int>(RANK::S);
	}
	else if (m_hp > static_cast<int>(ClearHp::ARANK))
	{
		m_score = static_cast<int>(RANK::A);
	}
	else if (m_hp > static_cast<int>(ClearHp::BRANK))
	{
		m_score = static_cast<int>(RANK::B);
	}
	else if (m_hp > static_cast<int>(ClearHp::CRANK))
	{
		m_score = static_cast<int>(RANK::C);
	}
	else
	{
		m_score = static_cast<int>(RANK::D);
	}
}
