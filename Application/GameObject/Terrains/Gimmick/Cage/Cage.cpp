#include "Cage.h"
#include"../../../../Scene/SceneManager.h"
#include"../../../../Manager/ModelManager/ModelManager.h"

const float Cage::k_correctionValue = 0.2f;
const float Cage::k_sameForceColisionRadius = 80.f;
const float Cage::k_differentForceColisionRadius = 100.f;

void Cage::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("Cage");
	}

	// 移動速度の初期化
	m_moveSpeed = m_gimmickData["Cage"].value("MoveSpeed", 0.5f);

	// デバック用初期化
	m_randomId = rand();

	// まとっている磁極の初期化
	m_maguneForce = MagunePowerN;

	// 最初の座標を保存しておく
	m_firstPos = m_pos;

	// 表示色の初期化
	m_color = {
		m_gimmickData["Cage"].value("Color", 0.5f),
		m_gimmickData["Cage"].value("Color", 0.5f),
		m_gimmickData["Cage"].value("Color", 0.5f)
	};

	m_brightColor = {
		m_gimmickData["Cage"].value("RgbMax", 1.f),
		m_gimmickData["Cage"].value("RgbMax", 1.f),
		m_gimmickData["Cage"].value("RgbMax", 1.f)
	};

	// モデルのサイズの初期化
	m_modelSize = m_gimmickData["Cage"].value("ModelSize", 7.f);

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

	// 当たり判定の形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Cage", m_model, KdCollider::TypeGround | KdCollider::TypeDamage);
}

void Cage::Update()
{
	// 表示色の更新
	if ((m_maguneForce & MagunePowerN) != 0)	// N極の磁力をまとっているとき
	{
		m_brightColor = { m_gimmickData["Cage"].value("RgbMax", 1.f),0,0 };
	}
	else if ((m_maguneForce & MagunePowerS) != 0)	// S極の磁力をまとっているとき
	{
		m_brightColor = { 0,m_gimmickData["Cage"].value("RgbMax", 1.f),m_gimmickData["Cage"].value("RgbMax", 1.f) };
	}

	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
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
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld, m_color);
}

void Cage::DrawBright()
{
	// 磁力をまとっていないなら早期リターン
	if ((m_maguneForce & NoForce) != 0)return;

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld, m_brightColor);
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

void Cage::CheckColision(const float _radius, const ColisionOption _option)
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

		// 当たっていないなら以下処理しない
		if (!hitFlg)continue;

		switch (_option)
		{
		case ColisionOption::SameForce:
			//同じ極の磁力をまとっているならHIT時の処理をする
			if (m_maguneForce == obj->GetMaguneForce())
			{
				OnHit(obj->GetPos(), obj->GetMaguneForce());
			}
			break;
		case ColisionOption::DifferentForce:
			//違う極の磁力をまとっているならHIT時の処理をする
			if (m_maguneForce != obj->GetMaguneForce())
			{
				OnHit(obj->GetPos(), obj->GetMaguneForce());
			}
			break;
		default:
			break;
		}
	}
}

void Cage::OnHit(Math::Vector3 _pos, UINT _maguneForce)
{
	// 移動スピードの初期化
	m_moveSpeed = m_gimmickData["Cage"].value("MoveSpeed", 0.5f);
	Math::Vector3 moveDir;

	// 自分と相手が同じ極の磁力をまとっているとき
	if (m_maguneForce == _maguneForce)
	{
		moveDir = m_pos - _pos;
	}
	// 違う極をまとっているとき
	else if (m_maguneForce != _maguneForce)
	{
		moveDir = m_firstPos - m_pos;
		if (moveDir.Length() < m_moveSpeed + k_correctionValue)m_moveSpeed = 0;
	}

	moveDir.Normalize();
	m_pos += moveDir * m_moveSpeed;
}
