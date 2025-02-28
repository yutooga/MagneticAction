#include "Coil.h"
#include"../../../../Manager/ModelManager/ModelManager.h"

void Coil::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("Coil");
	}

	// モデルの大きさの初期化
	m_modelSize = m_gimmickData["Coil"].value("ModelSize", 26.f);

	// IMGUI用の値の初期化
	m_randomId = rand();

	// 当たり判定の形状登録
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Coil", m_model, KdCollider::TypeGround);
}

void Coil::Update()
{
	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rotAngle));
	m_mWorld = scaleMat * rotMat * transMat;
}

void Coil::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("Coil", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Coil m_pos", &m_pos.x, 0.1f);
		ImGui::DragFloat("Coil size", &m_modelSize, 0.01f);
	}
	ImGui::PopID();
}
