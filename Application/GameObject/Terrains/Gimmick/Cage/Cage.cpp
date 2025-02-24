#include "Cage.h"
#include"../../../../Scene/SceneManager.h"
#include"../../../../Manager/ModelManager/ModelManager.h"

const float Cage::k_correctionValue = 0.2f;
const float Cage::k_moveSpeed = 0.5f;
const float Cage::k_modelSize = 7.0f;
const float Cage::k_sameForceColisionRadius = 80.f;
const float Cage::k_differentForceColisionRadius = 100.f;

void Cage::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = std::make_shared<KdModelWork>();
		m_model->SetModelData("Asset/Models/Terrains/Gimmick/Cate/Cate.gltf");
	}

	// デバック用初期化
	m_randomId = rand();

	// まとっている磁極の初期化
	m_maguneForce = MagunePowerN;

	// 最初の座標を保存しておく
	m_firstPos = m_pos;

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

	// 当たり判定の形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Cage", m_model, KdCollider::TypeGround | KdCollider::TypeDamage);
}

void Cage::Update()
{
	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(k_modelSize);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle));
	m_mWorld = scaleMat * rotMat * transMat;
}

void Cage::PostUpdate()
{
	//磁力まとっていないなら当たり判定しない
	if ((m_maguneForce & NoForce) != 0)return;

	//違う極の磁力をまとった壁同士の当たり判定
	CheckColision(k_differentForceColisionRadius, ColisionOption::DifferentForce);

	//同じ極の磁力をまとった壁同士の当たり判定
	CheckColision(k_sameForceColisionRadius, ColisionOption::SameForce);

	MagneFloorBase::PostUpdate();
}

void Cage::DrawLit()
{
	Math::Color color = { 0.5f,0.5f,0.5f };
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld, color);
}

void Cage::DrawBright()
{
	// 磁力をまとっていないなら早期リターン
	if ((m_maguneForce & NoForce) != 0)return;

	// S極の磁力をまとっているとき
	if ((m_maguneForce & MagunePowerN) != 0)
	{
		Math::Color color = { 1,0,0};
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld, color);
	}

	// N極の磁力をまとっているとき
	else if ((m_maguneForce & MagunePowerS) != 0)
	{
		Math::Color color = {0,1,1 };
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld, color);
	}
}


void Cage::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("Cage", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Cage m_pos", &m_pos.x, 0.1f);
		ImGui::DragFloat("Cage angle", &m_angle, 0.1f);
	}
	ImGui::PopID();
}

void Cage::OnHit(ObjectType _obj)
{
	// 当たった物のまとっている磁力と同じ磁極を付与する

	switch (_obj)
	{
	case ObjectType::MaguneBulletS:
		m_maguneForce = MagunePowerN;
		break;
	case ObjectType::MaguneBulletN:
		m_maguneForce = MagunePowerS;
		break;
	default:
		break;
	}
}

void Cage::CheckColision(const float _radius, ColisionOption _option)
{
	//===============================
	//              球判定
	//===============================

	//球判定用の変数を作成
	static KdCollider::SphereInfo sphere;

	//球の中心位置を設定
	sphere.m_sphere.Center = m_pos;
	//球の半径を設定
	sphere.m_sphere.Radius = _radius;
	//当たり判定をしたいタイプを設定
	sphere.m_type = KdCollider::TypeGround;

	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		//磁力を帯びてない物体または磁力の箱以外とは当たり判定しない
		if (((obj->GetMaguneForce() & NoForce) != 0) || (obj->GetObjType() != ObjectType::MagneBox))continue;

		bool hitFlg = false;
		hitFlg = obj->Intersects(sphere, nullptr);

		if (hitFlg && _option == ColisionOption::SameForce)
		{
			//同じ極の磁力をまとっているならHIT時の処理をする
			if (m_maguneForce == obj->GetMaguneForce())
			{
				OnHit(obj->GetPos(), obj->GetMaguneForce());
			}
		}
		else if (hitFlg && _option == ColisionOption::DifferentForce)
		{
			//違う極の磁力をまとっているならHIT時の処理をする
			if (m_maguneForce != obj->GetMaguneForce())
			{
				OnHit(obj->GetPos(), obj->GetMaguneForce());
			}
		}
	}
}

void Cage::OnHit(Math::Vector3 _pos, UINT _maguneForce)
{
	// 自分と相手が同じ極の磁力をまとっているとき
	if (m_maguneForce == _maguneForce)
	{
		// 移動スピードの初期化
		m_moveSpeed = k_moveSpeed;

		Math::Vector3 moveDir = m_pos - _pos;
		moveDir.Normalize();

		m_pos += moveDir * m_moveSpeed;
	}
	// 違う極をまとっているとき
	else if (m_maguneForce != _maguneForce)
	{
		// 移動スピードの初期化
		m_moveSpeed = k_moveSpeed;

		Math::Vector3 moveDir = m_firstPos - m_pos;
		if (moveDir.Length() < m_moveSpeed + k_correctionValue)m_moveSpeed = 0;
		moveDir.Normalize();

		m_pos += moveDir * m_moveSpeed;
	}
}
