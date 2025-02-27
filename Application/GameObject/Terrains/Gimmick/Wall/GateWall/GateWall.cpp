#include "GateWall.h"
#include"../../../../../Manager/ModelManager/ModelManager.h"

void GateWall::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("GateWall");
	}

	// モデルのサイズの初期化
	m_modelSize = m_gimmickData["GateWall"].value("ModelSize", 16.5f);

	// 当たり判定の形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("GateWall", m_model, KdCollider::TypeGround);

	//カメラのローカル行列
	Math::Matrix _transMat = Math::Matrix::CreateTranslation(m_gimmickData["GateWall"]["Camera"]["Pos"].value("X",0.f), 
		m_gimmickData["GateWall"]["Camera"]["Pos"].value("Y", 5.f), m_gimmickData["GateWall"]["Camera"]["Pos"].value("Z",15.f));
	Math::Matrix _rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_gimmickData["GateWall"]["Camera"].value("Angle", 180.f)));
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
