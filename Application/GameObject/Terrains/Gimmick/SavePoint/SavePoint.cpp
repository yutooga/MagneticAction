#include "SavePoint.h"
#include"../../../../Scene/SceneManager.h"
#include"../../../../Manager/ModelManager/ModelManager.h"

const float SavePoint::k_colisionAdjustValueY = 4.5f;

void SavePoint::Init()
{
	// モデルの読み込み
	if(!m_model)
	{
		m_model = ModelManager::Instance().GetModel("SavePoint");
	}

	// モデルの大きさの初期化
	m_modelSize = m_gimmickData["SavePoint"].value("ModelSize", 28.3f);

	//ImGui用のランダムなIdの生成
	m_randomId = rand();

	// デバックラインの初期化
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void SavePoint::Update()
{
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	m_mWorld = scaleMat * transMat;

	// プレイヤーの触れたかどうか判定する関数
	SavePoint_Player();
}

void SavePoint::DrawBright()
{
	if (!m_rangeFlg)return;
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);	
}

void SavePoint::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("SavePoint", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("SavePoint m_posX", &m_pos.x, 0.01f);
		ImGui::DragFloat("SavePoint m_posY", &m_pos.y, 0.01f);
		ImGui::DragFloat("SavePoint m_posZ", &m_pos.z, 0.01f);
		ImGui::DragFloat("SavePoint size", &m_modelSize, 0.01f);
	}
	ImGui::PopID();
}

void SavePoint::SavePoint_Player()
{
	//===============================
	//              球判定
	//===============================

	//球判定用の変数を作成
	KdCollider::SphereInfo sphere;

	//球の中心位置を設定
	sphere.m_sphere.Center = m_pos;
	sphere.m_sphere.Center.y += k_colisionAdjustValueY;

	//球の半径を設定
	sphere.m_sphere.Radius = m_gimmickData["SavePoint"]["Colision"].value("Radius", 3.f);

	//当たり判定をしたいタイプを設定
	sphere.m_type = KdCollider::TypeSight;

	//m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius);

	//当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		// プレイヤー以外は判定しない
		if (obj->GetObjType() != ObjectType::Player)continue;

		bool hitFlg = false;
		hitFlg = obj->Intersects(sphere,nullptr);

		//もし当たっていてなおかつプレイヤーならフラグをONにする
		if (hitFlg)
		{
			m_rangeFlg = true;
			obj->OnHit(ObjectType::SavePoint);

			// SE再生
			if (!m_seFlg)
			{
				m_seFlg = true;
				m_seInterval = m_gimmickData["SavePoint"].value("CoolTime", 60.f);
				KdAudioManager::Instance().Play(m_gimmickData["Se"]["SavePoint"]["URL"], false);
			}
			break;
		}
		else
		{
			m_seFlg = false;
			break;
		}
	}
}

