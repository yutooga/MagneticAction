#include "WoodenLift.h"
#include"../../../../Manager/ModelManager/ModelManager.h"
#include"../../../../Scene/SceneManager.h"
#include"../../../Manager/UpdateObjManager/UpdateObjManager.h"

const float WoodenLift::k_colisionRadius = 19.1f;
const float WoodenLift::k_distance = 17.5f;
const float WoodenLift::k_colisionAdjustValueY = 0.1f;

void WoodenLift::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("WoodenLift");
	}

	// IMGUI用の初期化
	m_randomId = rand();

	// 当たり判定の形状登録
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("WoodeLift", m_model, KdCollider::TypeGround);


	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	m_mWorld = scaleMat * transMat;
}

void WoodenLift::Update()
{
	// 更新フラグがOFFの時は早期リターン
	if (!UpdateObjManager::Instance().GetLiftUpdate())return;

	// 前の座標を保存
	m_previosPos = m_pos;

	// 角度の更新
	m_angle++;

	// 限界角度を超えたらその分引く
	if (m_angle > k_rotAngleMax)
	{
		m_angle -= k_rotAngleMax;
	}

	// 座標更新
	m_pos.z += sin(DirectX::XMConvertToRadians(m_angle))*m_moveSpeed;


	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	m_mWorld = scaleMat * transMat;
}

void WoodenLift::PostUpdate()
{
	// リフトの上にプレイヤーが乗っているかどうか判断する関数
	JudgementPlayer();
}

void WoodenLift::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("WoodenLift", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("WoodenLift m_pos", &m_pos.x, 0.01f);
		ImGui::DragFloat("WoodenLift m_size", &m_modelSize, 0.01f);
		ImGui::DragFloat("WoodenLift m_speed", &m_moveSpeed, 0.01f);
	}
	ImGui::PopID();
}

void WoodenLift::JudgementPlayer()
{
	// 更新フラグがOFFの時は早期リターン
	if (!UpdateObjManager::Instance().GetLiftUpdate())return;

	//===============================
	//              球判定
	//===============================

	//球判定用の変数を作成
	KdCollider::SphereInfo sphere;

	//球の中心位置を設定
	sphere.m_sphere.Center = m_pos;
	sphere.m_sphere.Center.y += k_colisionAdjustValueY;
	//球の半径を設定
	sphere.m_sphere.Radius = k_colisionRadius;
	//当たり判定をしたいタイプを設定
	sphere.m_type = KdCollider::TypeSight;

	//当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		// プレイヤーでない時は読み飛ばす
		if (obj->GetObjType() != ObjectType::Player)continue;

		bool hitFlg = false;
		hitFlg = obj->Intersects(sphere, nullptr);

		if (hitFlg)
		{
			if ((obj->GetPos().y - m_pos.y) >= k_distance)
			{
				Math::Vector3 playerPos = obj->GetPos();
				Math::Vector3 distance = m_pos - m_previosPos;
				playerPos += distance;
				obj->SettingPos(playerPos);
			}
			break;
		}
		else {
			break;
		}
	}
}
