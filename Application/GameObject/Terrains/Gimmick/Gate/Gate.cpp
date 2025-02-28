#include "Gate.h"
#include"../../../Manager/GateManager/GateManager.h"
#include"../../../../Manager/ModelManager/ModelManager.h"

void Gate::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("Gate");
	}

	// モデルの大きさ
	m_modelSize = m_gimmickData["Gate"].value("ModelSize", 16.5f);

	//ImGui用のランダムなIdの生成
	m_randomId = rand();

	// 当たり判定の形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Gate", m_model, KdCollider::TypeGround);
}

void Gate::Update()
{

	//扉が開放状態になったら開放アニメーションをする
	if (GateManager::Instance().GetNowState() == GateManager::State::Open)
	{
		GateOpen();
	}

	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	m_mWorld = scaleMat * transMat;
}

void Gate::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("Crystal", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Crystal m_posX", &m_pos.x, 0.1f);
		ImGui::DragFloat("Crystal m_posY", &m_pos.y, 0.1f);
		ImGui::DragFloat("Crystal m_posZ", &m_pos.z, 0.1f);
		ImGui::DragFloat("Crystal size", &m_modelSize, 0.01f);
	}
	ImGui::PopID();
}

void Gate::GateOpen()
{
	// 限界値に達するまで動作をする
	if (m_count < k_moveMax)
	{
		m_pos.y++;
		m_count++;
	}
	// 限界値に達したら動作を終了する
	else
	{
		GateManager::Instance().SetState(GateManager::State::NoticeAnimation);
	}

	// SEの再生フラグがONならSEを再生する
	if (!m_seFlg)
	{
		m_seFlg = true;
		KdAudioManager::Instance().Play(m_gimmickData["Se"]["Gate"]["URL"]);
	}
}
