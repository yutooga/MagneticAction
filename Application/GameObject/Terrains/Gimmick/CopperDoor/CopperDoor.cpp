#include "CopperDoor.h"
#include"../../../../Manager/ModelManager/ModelManager.h"
#include"../../../Manager/UpdateObjManager/UpdateObjManager.h"

void CopperDoor::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("CopperDoor");
	}

	// IMGUI用の初期化
	m_randomId = rand();

	// 当たり判定の形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("CopperDoor", m_model, KdCollider::TypeGround);

	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rotAngle));
	m_mWorld = scaleMat * rotMat * transMat;
}

void CopperDoor::Update()
{
	// 更新フラグがOFFならば早期リターン
	if (!UpdateObjManager::Instance().GetDoorUpdate())return;

	// 角度の更新
	m_angle++;

	// 角度が限界値を超えたらその分引く
	if (m_angle >= k_rotAngleMax)
	{
		m_angle -= k_rotAngleMax;
	}

	// 座標の更新
	m_pos.y += sin(DirectX::XMConvertToRadians(m_angle)) * m_moveSpeed;

	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rotAngle));
	m_mWorld = scaleMat * rotMat * transMat;
}

void CopperDoor::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("CopperDoor", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("CopperDoor m_pos", &m_pos.x, 0.01f);
		ImGui::DragFloat("CopperDoor m_size", &m_modelSize, 0.01f);
		ImGui::DragFloat("CopperDoor m_moveSpeed ", &m_moveSpeed, 0.01f);
	}
	ImGui::PopID();
}
