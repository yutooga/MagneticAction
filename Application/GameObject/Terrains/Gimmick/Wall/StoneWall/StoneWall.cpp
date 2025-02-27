#include "StoneWall.h"
#include"../../../../../Manager/ModelManager/ModelManager.h"

void StoneWall::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("StoneWall");
	}

	// 当たり判定の形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("StoneWall", m_model, KdCollider::TypeGround);

	// IMGUI用の初期化
	m_randomId = rand();
}

void StoneWall::Update()
{
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	Math::Matrix rotMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
	m_mWorld = scaleMat * rotMat * transMat;
}

void StoneWall::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("StoneWall", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("StoneWall m_pos", &m_pos.x, 0.1f);
		ImGui::DragFloat("StoneWall m_size", &m_modelSize, 0.1f);
		ImGui::DragFloat("StoneWall m_angle", &m_angle, 0.1f);
	}
	ImGui::PopID();
}
