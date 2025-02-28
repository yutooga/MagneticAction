#include "TargetLift.h"
#include"../../../../../Manager/ModelManager/ModelManager.h"
#include"../../../../Manager/UpdateObjManager/UpdateObjManager.h"

void TargetLift::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("MaguneFloor");
	}

	m_size = { m_gimmickData["TargetLift"].value("SizeX",1.f),
	m_gimmickData["TargetLift"].value("SizeY",1.f),m_gimmickData["TargetLift"].value("SizeZ",1.f) };

	// まとっている磁力の初期化
	m_maguneForce = NoForce;

	// ImGui用のランダムなIdの生成
	m_randomId = rand();

	// 動く速さの初期化
	m_moveSpeed = m_gimmickData["TargetLift"].value("MoveSpeed", 2.0f);

	// 当たり判定の形状登録
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("TargetLift", m_model,KdCollider::TypeDamage);
}

void TargetLift::Update()
{
	// 纏っている磁極をマネージャーに伝える
	TellToForce();

	// 角度の更新
	m_angle++;

	// 限界角度を超えたらその分引く
	if (m_angle >= k_rotAngleMax)
	{
		m_angle -= k_rotAngleMax;
	}

	// 座標の更新
	m_pos.z += sin(DirectX::XMConvertToRadians(m_angle)) * m_moveSpeed;


	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_size);
	m_mWorld = scaleMat * transMat;
}

void TargetLift::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("ObstructiveWall", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("m_pos", &m_pos.x, 0.1f);
		ImGui::DragFloat3("m_size", &m_size.x, 0.1f);
		ImGui::DragFloat("m_angle", &m_angle, 0.1f);
	}
	ImGui::PopID();
}

void TargetLift::TellToForce()
{
	// まとっている磁極のセット

	switch (m_keyObject)
	{
	case KdGameObject::KeyObject::Five:
		UpdateObjManager::Instance().SetMaguneForce(m_maguneForce, UpdateObjManager::five);
		break;
	case KdGameObject::KeyObject::Six:
		UpdateObjManager::Instance().SetMaguneForce(m_maguneForce, UpdateObjManager::six);
		break;
	default:
		break;
	}
}
