#include "TerrainsBase.h"

const float TerrainsBase::k_magnetRange = 40.0f;
const float TerrainsBase::k_magnetPower = 1.0f;
nlohmann::json TerrainsBase::m_gimmickData;

TerrainsBase::TerrainsBase()
{
	// jsonファイルの読み込み
	if (m_gimmickData.empty())
	{
		std::ifstream ifs("Asset/Data/GameScene/3DObject/Terrains/Gimmick/GimmickParamData/GimmickParamData.json");
		if (ifs) {
			ifs >> m_gimmickData;
		}
	}

	m_randomId = rand();
}

void TerrainsBase::DrawLit()
{
	if(m_model)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
	}
}

void TerrainsBase::DrawBright()
{
	//磁力をまとっていないなら描画しない
	if ((m_maguneForce & NoForce) != 0) return;

	Math::Color color;
	if ((m_maguneForce & MagunePowerN) != 0)
	{
		//N極の場合赤色に光らせる
		color = { 1,0,0 };
	}
	else if ((m_maguneForce & MagunePowerS) != 0)
	{
		//S極の場合青色に光らせる
		color = { 0,0,1 };
	}
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld, color);
}

void TerrainsBase::CheckSe()
{
	if (m_wpAttractSe.expired() == true)	//　まだ読み込まれていないなら読み込む
	{
		m_wpAttractSe = KdAudioManager::Instance().Play(m_gimmickData["Se"]["Attract"]["URL"], false);
	}
	else if (m_wpAttractSe.expired() == false)	// すでに読み込まれているなら再生が終わっているなら再び再生する
	{
		if (!m_wpAttractSe.lock()->IsPlaying())
		{
			m_wpAttractSe = KdAudioManager::Instance().Play(m_gimmickData["Se"]["Attract"]["URL"], false);
		}
	}
}
