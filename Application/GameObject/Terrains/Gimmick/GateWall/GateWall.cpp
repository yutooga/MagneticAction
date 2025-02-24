#include "GateWall.h"

void GateWall::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = std::make_shared<KdModelWork>();
		m_model->SetModelData("Asset/Models/Terrains/Gimmick/Gate/Gate_Wall_1.gltf");
	}

	// 当たり判定の形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("GateWall", m_model, KdCollider::TypeGround);

	//カメラのローカル行列
	Math::Matrix _transMat = Math::Matrix::CreateTranslation(0, 5.f, 15.f);
	Math::Matrix _rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180));
	m_localCamera = _rotMat * _transMat;
}

void GateWall::Update()
{
	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	m_mWorld = scaleMat * transMat;
}

void GateWall::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("GateWall", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("GateWall m_posX", &m_pos.x, 0.1f);
		ImGui::DragFloat("GateWall m_posY", &m_pos.y, 0.1f);
		ImGui::DragFloat("GateWall m_posZ", &m_pos.z, 0.1f);
		ImGui::DragFloat("GateWall size", &m_modelSize, 0.1f);
	}
	ImGui::PopID();
}
