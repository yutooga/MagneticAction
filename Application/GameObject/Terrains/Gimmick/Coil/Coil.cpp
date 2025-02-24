#include "Coil.h"
#include"../../../../Scene/SceneManager.h"
#include"../../../Manager/GateManager/GateManager.h"
#include"../../../../Manager/ModelManager/ModelManager.h"

const float Coil::k_ColisionRadius = 60.f;
const float Coil::k_ColisionAdjustValueY = 0.1f;

void Coil::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("Coil");
	}

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

void Coil::PostUpdate()
{
	//エリア内でプレイヤーが特定の行動をしているか判断する関数
	JudgmentMove();
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

void Coil::JudgmentMove()
{
	//===============================
	//              球判定
	//===============================

	//球判定用の変数を作成
	KdCollider::SphereInfo sphere;

	//球の中心位置を設定
	sphere.m_sphere.Center = m_pos;
	sphere.m_sphere.Center.y += k_ColisionAdjustValueY;
	//球の半径を設定
	sphere.m_sphere.Radius = k_ColisionRadius;
	//当たり判定をしたいタイプを設定
	sphere.m_type = KdCollider::TypeGround | KdCollider::TypeSight;

	//当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		if (obj->GetObjType() != ObjectType::MoveMagunet)continue;
		bool hitFlg = false;
		hitFlg = obj->Intersects(sphere, nullptr);

		if (hitFlg && obj->GetKeyObj()==KeyObject::Five)
		{
			//プレイヤーが電磁誘導を行っているならカメラのアニメーションを開始する
			if (obj->GetReaction() == true)
			{
				
			}	
		}
		else if(!hitFlg && obj->GetKeyObj() == KeyObject::Five)
		{
			
		}
	}
}
