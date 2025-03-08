#include "MagicRing.h"
#include"../../../../Manager/ModelManager/ModelManager.h"

const float MagicRing::k_changeColorTime = 300.f;
const float MagicRing::k_colorCoolTime = 400.f;
const float MagicRing::k_subtractionAmount = 0.01f;
const float MagicRing::k_addAmount = 0.001f;

void MagicRing::Init()
{
	// モデルの読み込み
	switch (m_objType)
	{
	case KdGameObject::ObjectType::NoForceRing:
		if (!m_model)
		{
			m_model = ModelManager::Instance().GetModel("NoForceMagicRing");
		}
		break;
	default:
		if (!m_model)
		{
			m_model = ModelManager::Instance().GetModel("MagicRing");
		}
		break;
	}
	

	// 当たり判定の形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("MagicRing", m_model, KdCollider::TypeGround | KdCollider::TypeDamage);

	// エフェクトの再生
	//中が空のときは再生しない
	if(m_effectName.size()!=0)
	{
		KdEffekseerManager::GetInstance().Play(m_effectName, m_pos, 
			m_gimmickData["MagicRing"]["Effect"].value("Size", 5.f), m_gimmickData["MagicRing"]["Effect"].value("Speed", 1.f), true);
	}

	// モデルの大きさの初期化
	m_modelSize = m_gimmickData["MagicRing"].value("ModelSize", 5.f);

	// 色の透明度の初期化
	m_colorAlpha = m_gimmickData["MagicRing"].value("Alpha", 1.f);

	// 表示色の初期化
	m_color = { 0,0,0 };

	//ImGui用のランダムなIdの生成
	m_randomId = rand();
}

void MagicRing::Update()
{
	// 魔法陣の纏っている磁力と表示色の更新
	BrightColorUpdate();

	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	m_mWorld = scaleMat * transMat;
}

void MagicRing::SetObjType(ObjectType _type)
{
	// オブジェクトタイプの保存
	m_objType = _type;

	// 磁力の付与
	switch (_type)
	{
	case KdGameObject::ObjectType::MagicRingS:
		m_color = { 0,0,m_colorAlpha };	// N極の場合青色に光らせる
		m_maguneForce = MagunePowerN;
		break;
	case KdGameObject::ObjectType::MagicRingN:
		m_color = { m_colorAlpha,0,0 };	// S極の場合赤色に光らせる
		m_maguneForce = MagunePowerS;
		break;
	case KdGameObject::ObjectType::NoForceRing:
		m_maguneForce = NoForce;
		break;
	default:
		break;
	}
}

void MagicRing::DrawBright()
{
	//磁力をまとっていないなら描画しない
	if ((m_maguneForce & NoForce) != 0)return;

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld, m_color);
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

void MagicRing::BrightColorUpdate()
{

	// 魔法陣が磁力をまとっていないとき早期リターン
	if ((m_maguneForce & NoForce) != 0)return;

	// プレイヤーが魔法陣から磁力を吸い取ったら魔法陣の磁力をなくす
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

	// 魔法陣が再び磁力をまとう処理
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

	// 表示色の更新
	switch (m_maguneForce)
	{
	case KdGameObject::MagunePowerN:
		m_color = { m_colorAlpha,0,0 };	// S極の場合赤色に光らせる
		break;
	case KdGameObject::MagunePowerS:
		m_color = { 0,0,m_colorAlpha };	// N極の場合青色に光らせる
		break;
	default:
		break;
	}
}
