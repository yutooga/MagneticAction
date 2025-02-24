#include "JointLift.h"
#include"../../../../Manager/ModelManager/ModelManager.h"
#include"../../../Manager/UpdateObjManager/UpdateObjManager.h"

void JointLift::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("JointLift");
	}

	// IMGUI用の初期化
	m_randomId = rand();

	// 当たり判定の形状登録
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("JointLift", m_model, KdCollider::TypeGround);
}

void JointLift::Update()
{
	// 角度の更新
	m_angle++;

	// 限界角度を超えたらその分引く
	if (m_angle > k_rotAngleMax)
	{
		m_angle -= k_rotAngleMax;
	}

	// 座標更新
	if(m_dir == MoveDirOption::X)
	{
		m_pos.x += sin(DirectX::XMConvertToRadians(m_angle)) * m_moveSpeed;
	}
	else if (m_dir == MoveDirOption::Z)
	{
		m_pos.z += sin(DirectX::XMConvertToRadians(m_angle)) * m_moveSpeed;
	}


	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	m_mWorld = scaleMat * transMat;
}

void JointLift::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("JointLift", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("m_pos", &m_pos.x, 0.01f);
		ImGui::DragFloat("m_size", &m_modelSize, 0.01f);
		ImGui::DragFloat("m_speed", &m_moveSpeed, 0.01f);
	}
	ImGui::PopID();
}
