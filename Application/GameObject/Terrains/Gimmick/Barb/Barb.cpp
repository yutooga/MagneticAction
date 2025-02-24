#include "Barb.h"
#include"../../../../Manager/ModelManager/ModelManager.h"

void Barb::Init()
{
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("Barb");
	}

	// 当たり判定の形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Barb", m_model, KdCollider::TypeGround);
}

void Barb::Update()
{
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	m_mWorld = scaleMat * transMat;
}

void Barb::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("Barb", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Barb m_pos", &m_pos.x, 0.1f);
		ImGui::DragFloat("Barb size", &m_modelSize, 0.1f);
	}
	ImGui::PopID();
}
