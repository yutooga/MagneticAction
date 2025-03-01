#include "Line.h"
#include"../../../../Manager/ModelManager/ModelManager.h"

void Line::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("Line");
	}

	// モデルの大きさの初期化
	m_modelSize = m_gimmickData["Line"].value("ModelSize", 37.2f);

	// 回転角度の初期化
	m_rotAngle = m_gimmickData["Line"].value("Angle", 90.f);
}

void Line::Update()
{
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rotAngle));

	m_mWorld = scaleMat * rotMat * transMat;
}

void Line::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("Line", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("m_pos", &m_pos.x, 0.01f);
		ImGui::DragFloat("m_size", &m_modelSize, 0.01f);
		ImGui::DragFloat("angle", &m_rotAngle, 0.01f);
	}
	ImGui::PopID();
}
