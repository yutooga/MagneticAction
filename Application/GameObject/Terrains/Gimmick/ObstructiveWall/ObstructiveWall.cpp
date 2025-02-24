#include "ObstructiveWall.h"
#include"../../../../Manager/ModelManager/ModelManager.h"
#include"../../../../Scene/SceneManager.h"

void ObstructiveWall::Init()
{
	// モデル読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("MaguneFloor");
	}

	m_size = { m_gimmickData["ObstructiveWall"].value("SizeX",1.f),
	m_gimmickData["ObstructiveWall"].value("SizeY",1.f),m_gimmickData["ObstructiveWall"].value("SizeZ",1.f) };

	// 一番最初の座標を保存しておく
	m_firstPos = m_pos;

	// まとっている磁力の初期化
	m_beforeMagneForce = m_maguneForce;

	// 磁極の切り替わるタイミング
	m_changeMoment = m_gimmickData["ObstructiveWall"].value("ChangeCntMax", 60.f);

	// IMGUI用の初期化
	m_randomId = rand();

	m_ObjType = ObjectType::ObstructiveWall;

	// デバックラインの実体の作成
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

	// 当たり判定の形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("ObstructiveWall", m_model, KdCollider::TypeDamage);
}

void ObstructiveWall::Update()
{
	//磁極の更新
	ChangeMagneForce();

	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_size);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rotAngle));
	m_mWorld = scaleMat * rotMat * transMat;
}

void ObstructiveWall::PostUpdate()
{
	//磁力まとっていないなら当たり判定しない
	if ((m_maguneForce & NoForce) != 0)return;

	{
		// 同じ磁極をまとった阻害壁同士の当たり判定
		float sameColision = m_gimmickData["ObstructiveWall"].value("SameColision", 50.f);
		ColisionCheck(sameColision, ColisionOption::SameForce);
	}

	{
		// 違う磁極をまとった阻害壁同士の当たり判定
		float differentColision = m_gimmickData["ObstructiveWall"].value("DifferentColision", 60.f);
		ColisionCheck(differentColision, ColisionOption::DifferentForce);
	}
}

void ObstructiveWall::DrawBright()
{
	//磁力をまとっていないなら描画しない
	if ((m_maguneForce & NoForce) != 0)return;

	Math::Color color;
	if ((m_maguneForce & MagunePowerN) != 0)
	{
		//N極の場合青色に光らせる
		color = { m_changeForceCnt / m_changeMoment,0, 1.f - (m_changeForceCnt / m_changeMoment) };
	}
	else if ((m_maguneForce & MagunePowerS) != 0)
	{
		//S極の場合赤色に光らせる
		color = { 1.f - (m_changeForceCnt / m_changeMoment),0,m_changeForceCnt / m_changeMoment };
	}
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld, color);
}

void ObstructiveWall::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("ObstructiveWall", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("m_pos", &m_pos.x, 0.1f);
		ImGui::DragFloat3("m_size", &m_size.x, 0.1f);
		ImGui::DragFloat("m_angle", &m_rotAngle, 0.1f);
	}
	ImGui::PopID();
}

void ObstructiveWall::ChangeMagneForce()
{
	// 前のフレームから磁極が変化していないなら処理をしない
	if (m_beforeMagneForce == m_maguneForce)
	{
		m_changeForceCnt = m_gimmickData["ObstructiveWall"].value("ChangeCntMax", 60.f);
		return;
	}

	// カウントの更新
	m_changeForceCnt--;
	
	// カウントが０を下回ったら磁極を更新する
	if (m_changeForceCnt > 0)return;

	// カウントを元に戻す
	m_changeForceCnt = m_gimmickData["ObstructiveWall"].value("ChangeCntMax", 60.f);

	// 纏っていた磁極がS極だった場合
	if ((m_maguneForce & MagunePowerS) != 0)
	{
		m_maguneForce = MagunePowerN;	// 反対の磁極を付与する
	}
	// 纏っていた磁極がN極だった場合
	else if ((m_maguneForce & MagunePowerN) != 0)
	{
		m_maguneForce = MagunePowerS;
	}

	// 纏っている磁極を保存しておく
	m_beforeMagneForce = m_maguneForce;
}

void ObstructiveWall::ColisionCheck(const float _radius, ColisionOption _option)
{
	//===============================
	//              球判定
	//===============================

	//球判定用の変数を作成
	KdCollider::SphereInfo sphere;

	//球の中心位置を設定
	sphere.m_sphere.Center = m_pos;

	//球の半径を設定
	sphere.m_sphere.Radius = _radius;

	//当たり判定をしたいタイプを設定
	sphere.m_type = KdCollider::TypeDamage;

	float m_colisionGard = m_gimmickData["ObstructiveWall"].value("ColisionGuard", 5.f);

	//当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		if (obj->GetObjType() != ObjectType::ObstructiveWall)continue;	// 特定のオブジェクト以外当たり判定をしない
		else if ((m_pos - obj->GetPos()).Length() < m_colisionGard)continue;	// 自分同士とは当たり判定をしないS

		if ((obj->GetMaguneForce() & NoForce) != 0)continue;	//磁力を帯びてない物体とは当たり判定しない

		bool hitFlg = false;
		hitFlg = obj->Intersects(sphere, nullptr);

		// 当たっているときの処理

		if (hitFlg && _option == ColisionOption::DifferentForce)
		{
			//違う極の磁力をまとっているならHIT時の処理をする
			if (m_maguneForce != obj->GetMaguneForce())
			{
				OnHit(obj->GetMaguneForce(), obj->GetPos());
			}
		}
		else if (hitFlg && _option == ColisionOption::SameForce)
		{
			//同じ極の磁力をまとっているならHIT時の処理をする
			if (m_maguneForce == obj->GetMaguneForce())
			{
				OnHit(obj->GetMaguneForce(), obj->GetPos());
			}
		}
	}
}

void ObstructiveWall::OnHit(const UINT& _magneForce, const Math::Vector3& _targetPos)
{
	// 移動スピードの初期化
	m_moveSpeed = 1.0f;
	Math::Vector3 moveDir;

	// 自分と相手が同じ極の磁力をまとっているとき(反発処理)
	if (m_maguneForce == _magneForce)
	{
		moveDir = m_pos - _targetPos;
	}

	// 違う極をまとっているとき(斥力処理)
	else if (m_maguneForce != _magneForce)
	{
		moveDir = m_firstPos - m_pos;
		if (moveDir.Length() < m_moveSpeed)m_moveSpeed = 0;
	}

	moveDir.Normalize();
	m_pos += moveDir * m_moveSpeed;
}
