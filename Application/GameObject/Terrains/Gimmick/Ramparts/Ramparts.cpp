#include "Ramparts.h"

const float Ramparts::k_rotAngle = 90.f;

void Ramparts::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = std::make_shared<KdModelWork>();
		m_model->SetModelData("Asset/Models/Terrains/Gimmick/GateWall/Wall_2.gltf");
	}

	// IMGUI用の初期化
	m_randomId = rand();

	// 当たり判定の形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Ramparts", m_model, KdCollider::TypeGround | KdCollider::TypeDamage);
}

void Ramparts::Update()
{
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(k_rotAngle));
	m_mWorld = scaleMat * rotMat * transMat;
}

void Ramparts::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("Ramparts", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Ramparts m_posX", &m_pos.x, 0.1f);
		ImGui::DragFloat("Ramparts m_posY", &m_pos.y, 0.1f);
		ImGui::DragFloat("Ramparts m_posZ", &m_pos.z, 0.1f);
		ImGui::DragFloat("Ramparts size", &m_modelSize, 0.1f);
	}
	ImGui::PopID();
}
