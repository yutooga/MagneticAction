#include "MagneticArea.h"

void MagnaticArea::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = std::make_shared<KdModelWork>();
		m_model->SetModelData("Asset/Models/Terrains/Gimmick/MagneticArea/MagneticArea.gltf");
	}

	// 当たり判定の形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("MagnaticArea", m_model, KdCollider::TypeGround);

	// IMGUI用の初期化
	m_randomId = rand();

	// まとっている磁力の初期化
	m_maguneForce = MagunePowerS;

	// オブジェクトタイプの初期化
	m_ObjType = ObjectType::MagneticArea;
}

void MagnaticArea::Update()
{
	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle));
	m_mWorld = scaleMat * rotMat * transMat;
}

void MagnaticArea::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("MagnaticArea", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("MagnaticArea m_pos", &m_pos.x, 0.1f);
		ImGui::DragFloat("MagnaticArea m_size", &m_modelSize, 0.1f);
		ImGui::DragFloat("MagnaticArea m_angle", &m_angle, 0.1f);
	}
	ImGui::PopID();
}
