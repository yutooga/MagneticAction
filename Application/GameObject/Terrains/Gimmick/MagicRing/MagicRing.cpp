#include "MagicRing.h"
#include"../../../../Manager/ModelManager/ModelManager.h"

const float MagicRing::k_changeColorTime = 300.f;
const float MagicRing::k_colorCoolTime = 400.f;
const float MagicRing::k_subtractionAmount = 0.01f;
const float MagicRing::k_addAmount = 0.001f;

void MagicRing::Init()
{
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("MagicRing");
	}

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("MagicRing", m_model, KdCollider::TypeGround | KdCollider::TypeDamage);

	// エフェクトの再生
	//中が空のときは再生しない
	if(m_effectName.size()!=0)
	{
		KdEffekseerManager::GetInstance().Play(m_effectName, m_pos, 5.0f, 1.0f, true);
	}

	//ImGui用のランダムなIdの生成
	m_randomId = rand();

	m_maguneForce = NoForce;
}

void MagicRing::Update()
{
	if (m_updateFlg)
	{
		m_colorAlpha -= k_subtractionAmount;
		if (m_colorAlpha < 0)
		{
			// エフェクトをストップさせる
			KdEffekseerManager::GetInstance().StopEffect(m_effectName, m_pos);
			m_effectReappearanceCnt = static_cast<int>(k_colorCoolTime);
			m_colorAlpha = 0.0f;
			m_updateFlg = false;
		}
	}

	if (m_effectReappearanceCnt > 0 && m_effectReappearanceCnt > k_changeColorTime)
	{
		m_effectReappearanceCnt--;
	}
	else if (m_effectReappearanceCnt > 0)
	{
		m_colorAlpha += k_addAmount;

		// 色の限界値を超えないようにする
		if (m_colorAlpha >= k_alphaMax)
		{
			m_colorAlpha = k_alphaMax;
		}

		m_effectReappearanceCnt--;
		if (m_effectReappearanceCnt <= 0)
		{
			m_effectReappearanceCnt = 0;
			KdEffekseerManager::GetInstance().Play(m_effectName, m_pos, 5.0f, 1.0f, true);
			m_reactionFlg = false;
		}
	}

	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	m_mWorld = scaleMat * transMat;
}

void MagicRing::SetObjType(ObjectType _type)
{
	m_objType = _type;
	if (_type == ObjectType::MagicRingS)
	{
		m_maguneForce = MagunePowerN;
	}
	else if (_type == ObjectType::MagicRingN)
	{
		m_maguneForce = MagunePowerS;
	}
	else if (_type == ObjectType::NoForceRing)
	{
		m_maguneForce = NoForce;
	}
}

void MagicRing::DrawBright()
{
	//磁力をまとっていないなら描画しない
	if ((m_maguneForce & NoForce) != 0)return;

	Math::Color color;
	if ((m_maguneForce & MagunePowerN) != 0)
	{
		//S極の場合赤色に光らせる
		color = { m_colorAlpha,0,0 };
	}
	else if ((m_maguneForce & MagunePowerS) != 0)
	{
		//N極の場合青色に光らせる
		color = { 0,0,m_colorAlpha };
	}

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld, color);
}

void MagicRing::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if(ImGui::CollapsingHeader("MagicRing",ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("MagicRing m_pos", &m_pos.x, 0.1f);
	}
	ImGui::PopID();
}
