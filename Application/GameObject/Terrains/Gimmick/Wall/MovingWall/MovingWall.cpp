#include "MovingWall.h"
#include"../../../../../Scene/SceneManager.h"
#include"../../../../../Manager/ModelManager/ModelManager.h"

const float MovingWall::k_adjustAdValue = 5.f;
const float MovingWall::k_repulsionMoveSpeed = 0.7f;
const float MovingWall::k_colisionGuard = 5.f;

const float MovingWall::SmallSize::k_sameForceSmallColisionRadius = 35.f;
const float MovingWall::SmallSize::k_differentForceSmallColisionRadius = 40.f;

const float MovingWall::BigSize::k_sameForceBigColisionRadius = 180.f;
const float MovingWall::BigSize::k_differentForceBigColisionRadius = 200.f;
const float MovingWall::BigSize::k_wallDistanceColisionRadius = 48.9f;

void MovingWall::Init()
{
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

	// まとう磁力の初期化
	m_maguneForce = NoForce;

	//ImGui用のランダムなIdの生成
	m_randomId = rand();

	// 一番最初の座標を保存する
	m_firstPos = m_pos;

	// モデルのサイズの初期化
	m_modelSize = m_gimmickData["MovingWall"].value("ModelSize", 5.f);

	// 動く速さの初期化
	m_moveSpeed = m_gimmickData["MovingWall"].value("MoveSpeed", 0.5f);

	// 回転角度の初期化
	m_angle = m_gimmickData["MovingWall"].value("Angle", 61.3f);
}

void MovingWall::Update()
{
	// フラグの初期化
	m_attractFlg = false;

	if (m_Size)
	{
		m_Size->Update(*this);
	}

	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	Math::Matrix rotationMat = Math::Matrix::CreateRotationY(m_angle);
	m_mWorld = scaleMat * rotationMat * transMat;
}

void MovingWall::PostUpdate()
{
	if (m_Size)
	{
		m_Size->PostUpdate(*this);
	}
}

void MovingWall::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("MovingWall", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("MovingWall m_pos", &m_pos.x, 0.1f);
		ImGui::DragFloat("MovingWall m_size", &m_modelSize, 0.1f);
		ImGui::DragFloat("MovingWall m_angle", &m_angle, 0.01f);
		ImGui::LabelText("length", "length %f", (m_pos - m_previosPos).Length());
	}
	ImGui::PopID();
}

void MovingWall::SetSize(const Size& _size)
{
	switch (_size)
	{
	case Size::Small:
		SetSize(std::make_shared<MovingWall::SmallSize>());
		break;
	case Size::Big:
		SetSize(std::make_shared<MovingWall::BigSize>());
		break;
	default:
		break;
	}
}

void MovingWall::OnHit(Math::Vector3 _pos, UINT _maguneForce, Size _size)
{

	// 移動スピードの初期化
	m_moveSpeed = m_gimmickData["MovingWall"].value("MoveSpeed", 0.5f);

	// 移動方向
	Math::Vector3 moveDir;

	switch (_size)
	{
	case Size::Small:	// 小さいサイズの時
		// 自分と相手が同じ極の磁力をまとっているとき(反発処理)
		if (m_maguneForce == _maguneForce)
		{
			moveDir = m_pos - _pos;
		}

		// 違う極をまとっているとき(斥力処理)
		else if (m_maguneForce != _maguneForce)
		{
			moveDir = m_firstPos - m_pos;
			if (moveDir.Length() < m_moveSpeed)m_moveSpeed = 0;
		}
		break;
	case Size::Big:	// 大きいサイズの時
		m_updateFlg = true;
		// 自分と相手が同じ極の磁力をまとっているとき(反発処理)
		if (m_maguneForce == _maguneForce)
		{
			moveDir = m_firstPos - m_pos;
			if (moveDir.Length() < m_moveSpeed)m_moveSpeed = 0;
		}
		// 違う極をまとっているとき(斥力処理)
		else if (m_maguneForce != _maguneForce)
		{
			moveDir = _pos - m_pos;
			m_moveSpeed = k_repulsionMoveSpeed;
		}
		break;
	default:
		break;
	}

	moveDir.Normalize();
	m_pos += moveDir * m_moveSpeed;
}

void MovingWall::OnHit([[maybe_unused]] Math::Vector3 _pos)
{
	m_attractFlg = true;
}

void MovingWall::PlayerReaction()
{
	//オブジェクトが存在しないまたは磁力をまとっていないなら早期リターン
	if (m_nowState == State::NormalState || m_obj.expired() == true)return;

	Math::Vector3 moveDir = Math::Vector3::Zero;
	Math::Vector3 addPos = m_obj.lock()->GetPos();

	switch (m_nowState)
	{
	case MagneFloorBase::State::Opposition:	// 反発状態の時
	{

		//反発処理
		if (m_oppCnt == 0)
		{
			//反発SE再生
			KdAudioManager::Instance().Play(m_gimmickData["Se"]["Repulsion"]["URL"], false);
		}

		// オブジェクトとプレイヤーの位置から反発方向を決める
		moveDir = m_obj.lock()->GetPos() - m_pos;
		moveDir.Normalize();
		addPos += moveDir * m_oppoPow;
		m_oppoPow += k_oppositionPower;
		m_obj.lock()->SettingPos(addPos);
		m_oppCnt++;

		// 反発状態の維持の限界値を超えたら初期状態に戻す
		if (m_oppCnt > k_OppoStateLimit)
		{
			m_nowState = State::NormalState;
			m_oppCnt = 0;
			m_oppoPow = k_initialOppoPowerValue;
		}
		break;
	}
	case MagneFloorBase::State::Adsorption:	// 吸着状態の時
	{
		//吸着処理

		Math::Vector3 PlayerPos = m_obj.lock()->GetPos();

		m_adPow = k_adsorptionPower;
		moveDir = m_pos - PlayerPos;


		// プレイヤーがオブジェクトのどの側面にくっついているか判別する処理

		// プレイヤーがオブジェクトの左右にいる場合
		if (PlayerPos.z < (m_pos.z + k_adjustAdValue) && PlayerPos.z >(m_pos.z - k_adjustAdValue))
		{
			if (m_pos.x > PlayerPos.x || m_pos.x < PlayerPos.x)
			{
				moveDir.z = 0;
			}
		}
		// プレイヤーがオブジェクトの前後にいる場合
		else if (m_pos.z > PlayerPos.z || m_pos.z < PlayerPos.z) { moveDir.x = 0; }

		if (moveDir.Length() < k_adsorptionPower)m_adPow = moveDir.Length();
		moveDir.Normalize();
		addPos += moveDir * m_adPow;
		m_obj.lock()->SettingPos(addPos);
		break;
	}
	default:
		break;
	}
}

void MovingWall::ColisionCheck(const float _radius, ColisionOption _option, Size _size)
{
	//磁力まとっていないなら当たり判定しない
	if ((m_maguneForce & NoForce) != 0)return;

	//===============================
	//              球判定
	//===============================

	//球判定用の変数を作成
	KdCollider::SphereInfo sphere;

	//球の中心位置を設定
	sphere.m_sphere.Center = m_pos;
	sphere.m_sphere.Center.y += m_gimmickData["MovingWall"]["Colision"].value("AdjsutY", 15.f);

	//球の半径を設定
	sphere.m_sphere.Radius = _radius;

	//当たり判定をしたいタイプを設定
	sphere.m_type = KdCollider::TypeGround;

	//当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		switch (_size)
		{
		case MovingWall::Size::Small:
			if (obj->GetObjType() != ObjectType::MovingWall)continue;	// 指定のオブジェクト以外当たり判定しない
			break;
		case MovingWall::Size::Big:
			if (obj->GetObjType() != ObjectType::BigMovingWall)continue;
			break;
		default:
			break;
		}
		
		if ((obj->GetMaguneForce() & NoForce)!= 0)continue;	//磁力を帯びてない物体とは当たり判定しない

		// 自分自身とは当たり判定しない
		if (abs(m_pos.x-obj->GetPos().x) < k_colisionGuard && abs(m_pos.z - obj->GetPos().z) < k_colisionGuard) {
			continue;
		}


		bool hitFlg = false;
		hitFlg = obj->Intersects(sphere, nullptr);

		// 当たっているときの処理

		if (!hitFlg)continue;	// 当たっていないなら処理を飛ばす

		switch (_option)
		{
		case MovingWall::ColisionOption::DifferentForce:
			//違う極の磁力をまとっているならHIT時の処理をする
			if (m_maguneForce != obj->GetMaguneForce())
			{
				OnHit(obj->GetPos(), obj->GetMaguneForce(), _size);
			}
			break;
		case MovingWall::ColisionOption::SameForce:
			//同じ極の磁力をまとっているならHIT時の処理をする
			if (m_maguneForce == obj->GetMaguneForce())
			{
				OnHit(obj->GetPos(), obj->GetMaguneForce(), _size);
			}
			break;
		case MovingWall::ColisionOption::WallDistance:	// オブジェクトが近づきすぎているとき
			//まとっている磁力の極がちがうならHIT処理をする
			if (m_maguneForce != obj->GetMaguneForce())
			{
				OnHit(obj->GetPos());
			}
			break;
		default:
			break;
		}
	}
}

void MovingWall::SetSize(const std::shared_ptr<BaseSize>& Size)
{
	if (m_Size)m_Size->Exit(*this);
	m_Size = Size;
	m_Size->Enter(*this);
}

void MovingWall::BigSize::Enter(MovingWall& owner)
{
	// モデルの読み込み
	if (!owner.m_model)
	{
		owner.m_model = ModelManager::Instance().GetModel("BigMovingWall");
	}

	owner.m_angle = 0.0f;
	owner.m_maguneForce = NoForce;

	// 当たり判定の形状登録
	owner.m_pCollider = std::make_unique<KdCollider>();
	owner.m_pCollider->RegisterCollisionShape("MovingFloor2", owner.m_model, KdCollider::TypeGround | KdCollider::TypeDamage);
}

void MovingWall::BigSize::Update(MovingWall& owner)
{
	// 磁力をまとっていないなら早期リターン
	if ((owner.GetMaguneForce() & NoForce) != 0)return;

	// 距離判定フラグの初期化
	owner.m_attractFlg = false;

	owner.m_updateFlg = false;

	owner.m_previosPos = owner.m_pos;

	//壁同士の距離判定
	owner.ColisionCheck(k_wallDistanceColisionRadius, ColisionOption::WallDistance, Size::Big);

	// 壁同士がすぐ近くまで来ているなら早期リターン
	if (owner.m_attractFlg)return;

	//違う極の磁力をまとった壁同士の当たり判定
	owner.ColisionCheck(k_differentForceBigColisionRadius, ColisionOption::DifferentForce, Size::Big);


	//同じ極の磁力をまとった壁同士の当たり判定
	owner.ColisionCheck(k_sameForceBigColisionRadius, ColisionOption::SameForce, Size::Big);
}

void MovingWall::SmallSize::Enter(MovingWall& owner)
{
	// モデルの読み込み
	if (!owner.m_model)
	{
		owner.m_model = ModelManager::Instance().GetModel("MovingWall");
	}

	// 当たり判定の形状登録
	owner.m_pCollider = std::make_unique<KdCollider>();
	owner.m_pCollider->RegisterCollisionShape("MovingFloor", owner.m_model, KdCollider::TypeGround | KdCollider::TypeDamage);
}

void MovingWall::SmallSize::PostUpdate(MovingWall& owner)
{
	// 違う極の磁力をまとった壁同士の当たり判定(斥力判定)
	owner.ColisionCheck(k_differentForceSmallColisionRadius, ColisionOption::DifferentForce, Size::Small);

	// 同じ極の磁力をまとった壁同士の当たり判定(反発判定)
	owner.ColisionCheck(k_sameForceSmallColisionRadius, ColisionOption::SameForce, Size::Small);

	owner.MagneFloorBase::PostUpdate();
}

