#include "Goal.h"
#include"../../../Manager/GoalManager/GoalManager.h"
#include"../../../../Manager/ModelManager/ModelManager.h"

const float Goal::k_modelSize = 19.4f;

void Goal::Init()
{
	// モデルの読み込み
	if(!m_model)
	{
		m_model = ModelManager::Instance().GetModel("Goal");
	}

	//ImGui用のランダムなIdの生成
	m_randomId = rand();

	// 当たり判定の形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Goal", m_model, KdCollider::TypeGround | KdCollider::TypeDamage);
}

void Goal::Update()
{
	if (!m_setFlg)
	{
		m_setFlg = true;
		// ゴールの位置を記憶させる
		GoalManager::instance().SetGoalPos(m_pos);
	}

	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(k_modelSize);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(90));
	m_mWorld = scaleMat * rotMat * transMat;
}

void Goal::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("Goal", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Goal m_posX", &m_pos.x, 0.1f);
		ImGui::DragFloat("Goal m_posY", &m_pos.y, 0.1f);
		ImGui::DragFloat("Goal m_posZ", &m_pos.z, 0.1f);
	}
	ImGui::PopID();
}
