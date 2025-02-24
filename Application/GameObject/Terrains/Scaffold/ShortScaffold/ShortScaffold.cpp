#include "ShortScaffold.h"

void ShortScaffold::Init()
{
	// モデルの読み込み
	if(!m_model)
	{
		m_model = std::make_shared<KdModelWork>();
		m_model->SetModelData("Asset/Models/Terrains/Scaffold/ShortScaffold/ShortScaffold.gltf");
	}

	//ImGui用のランダムなIdの生成
	m_randomId = rand();

	// 当たり判定の形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("ShortScaffold", m_model, KdCollider::TypeGround | KdCollider::TypeDamage);
}

void ShortScaffold::Update()
{
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	Math::Matrix rotMat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(0));
	m_mWorld = scaleMat * rotMat * transMat;
}

void ShortScaffold::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("Scaffold", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("ShortScaffold m_pos", &m_pos.x, 0.01f);
		ImGui::DragFloat("ShortScaffold m_size", &m_modelSize, 0.01f);
	}
	ImGui::PopID();
}
