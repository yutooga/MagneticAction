#include "GoldPillar.h"
#include"../../../../Manager/ModelManager/ModelManager.h"

void GoldPillar::Init()
{

	// モデルの読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("GoldPillar");
	}

	// 当たり判定の形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("GoldPillar", m_model, KdCollider::TypeGround);

	// IMGUI用の初期化
	m_randomId = rand();

	m_pos = { 0,20,10 };
}

void GoldPillar::Update()
{
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	m_mWorld = scaleMat * transMat;
}

void GoldPillar::DrawImGui()
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
