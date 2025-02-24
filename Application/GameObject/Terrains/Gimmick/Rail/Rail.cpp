#include "Rail.h"
#include"../../../../Manager/ModelManager/ModelManager.h"

void Rail::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("Rail");
	}

	// IMGUI用の初期化
	m_randomId = rand();

	// 当たり判定の形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Rail", m_model, KdCollider::TypeGround);
}

void Rail::Update()
{
	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle));
	m_mWorld = scaleMat * rotMat * transMat;
}

void Rail::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("Rail", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Rail m_pos", &m_pos.x, 0.01f);
		ImGui::DragFloat("Rail m_size", &m_modelSize, 0.01f);
		ImGui::DragFloat("Rail angle", &m_angle, 0.01f);
	}
	ImGui::PopID();
}
