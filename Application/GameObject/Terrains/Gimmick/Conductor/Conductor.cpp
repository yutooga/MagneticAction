#include "Conductor.h"
#include"../../../../Manager/ModelManager/ModelManager.h"

void Conductor::Init()
{
	//モデルの読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("Conductor");
	}

	//IMGUI用の初期化
	m_randomId = rand();

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Conductor", m_model, KdCollider::TypeGround);
}

void Conductor::Update()
{
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle));
	m_mWorld = scaleMat * rotMat * transMat;
}

void Conductor::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("Conductor", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Conductor m_pos", &m_pos.x, 0.1f);
		ImGui::DragFloat("Conductor size", &m_modelSize, 0.01f);
		ImGui::DragFloat("Conductor angle", &m_angle, 0.01f);
	}
	ImGui::PopID();
}
