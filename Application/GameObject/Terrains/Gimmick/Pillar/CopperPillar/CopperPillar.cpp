#include "CopperPillar.h"
#include"../../../../../Manager/ModelManager/ModelManager.h"

void CopperPillar::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("CopperPillar");
	}

	// 当たり判定の形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("CopperPillar", m_model, KdCollider::TypeGround);

	// IMGUI用の初期化
	m_randomId = rand();
}

void CopperPillar::Update()
{
	//  行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	m_mWorld = scaleMat * transMat;
}

void CopperPillar::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("CopperPillar", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("CopperPillar m_posX", &m_pos.x, 0.1f);
		ImGui::DragFloat("CopperPillar m_posY", &m_pos.y, 0.1f);
		ImGui::DragFloat("CopperPillar m_posZ", &m_pos.z, 0.1f);
		ImGui::DragFloat("CopperPillar m_size", &m_modelSize, 0.1f);
	}
	ImGui::PopID();
}
