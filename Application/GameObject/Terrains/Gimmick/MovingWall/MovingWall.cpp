#include "MovingWall.h"
#include"../../../../Scene/SceneManager.h"

const float MovingWall::k_moveSpeed = 0.5f;
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
		ImGui::DragFloat("MovingWall m_angle", &m_angle, 0.1f);
		ImGui::LabelText("length", "length %f", (m_pos - m_previosPos).Length());
	}
	ImGui::PopID();
}

void MovingWall::SetSize(const Size& _size)
{
	if (_size == Size::Small)
	{
		SetSize(std::make_shared<MovingWall::SmallSize>());
	}
	else
	{
		SetSize(std::make_shared<MovingWall::BigSize>());
	}
}

void MovingWall::OnHit(Math::Vector3 _pos, UINT _maguneForce, Size _size)
{

	// 移動スピードの初期化
	m_moveSpeed = k_moveSpeed;
	Math::Vector3 moveDir;

	// 大きいサイズの時
	if (_size == Size::Big)
	{
		m_updateFlg = true;

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
	}

	// 小さいサイズの時
	else if(_size == Size::Small)
	{
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

	// 反発状態の時
	if (m_nowState == State::Opposition)
	{
		//反発処理

		if (m_oppCnt == 0)
		{
			//反発SE再生
			KdAudioManager::Instance().Play("Asset/Sounds/GameScene/Terrains/Gimmick/Repulsion.wav", false);
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
	}
	
	// 吸着状態の時
	else if (m_nowState == State::Adsorption)
	{
		//吸着処理

		m_adPow = k_adsorptionPower;
		moveDir = m_pos - m_obj.lock()->GetPos();
		
		// プレイヤーがオブジェクトのどの側面にくっついているか判別する処理
		if (m_obj.lock()->GetPos().z < (m_pos.z + k_adjustAdValue) && m_obj.lock()->GetPos().z >(m_pos.z - k_adjustAdValue))
		{
			if (m_pos.x > m_obj.lock()->GetPos().x || m_pos.x < m_obj.lock()->GetPos().x)
			{
				moveDir.z = 0;
			}
		}
		else if (m_pos.z > m_obj.lock()->GetPos().z || m_pos.z < m_obj.lock()->GetPos().z) { moveDir.x = 0; }

		if (moveDir.Length() < k_adsorptionPower)m_adPow = moveDir.Length();
		moveDir.Normalize();
		addPos += moveDir * m_adPow;
		m_obj.lock()->SettingPos(addPos);
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
	sphere.m_sphere.Center.y += 15.0f;

	//球の半径を設定
	sphere.m_sphere.Radius = _radius;

	//当たり判定をしたいタイプを設定
	sphere.m_type = KdCollider::TypeGround;

	//当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		if(_size == Size::Small)
		{
			if (obj->GetObjType() != ObjectType::MovingWall)continue;	// 指定のオブジェクト以外当たり判定しない
		}
		else if (_size == Size::Big)
		{
			if (obj->GetObjType() != ObjectType::BigMovingWall)continue;
		}
		
		if ((obj->GetMaguneForce() & NoForce)!= 0)continue;	//磁力を帯びてない物体とは当たり判定しない

		if (abs(m_pos.x-obj->GetPos().x) < k_colisionGuard && abs(m_pos.z - obj->GetPos().z) < k_colisionGuard) {
			continue;
		}


		bool hitFlg = false;
		hitFlg = obj->Intersects(sphere, nullptr);

		// 当たっているときの処理

		if (hitFlg && _option == ColisionOption::DifferentForce)
		{
			//違う極の磁力をまとっているならHIT時の処理をする
			if (m_maguneForce != obj->GetMaguneForce())
			{
				if(_size == Size::Small)
				{
					OnHit(obj->GetPos(), obj->GetMaguneForce(),Size::Small);
				}
				else if (_size == Size::Big)
				{
					OnHit(obj->GetPos(), obj->GetMaguneForce(),Size::Big);
				}
			}
		}
		else if (hitFlg && _option == ColisionOption::SameForce)
		{
			//同じ極の磁力をまとっているならHIT時の処理をする
			if (m_maguneForce == obj->GetMaguneForce())
			{
				if(_size == Size::Small)
				{
					OnHit(obj->GetPos(), obj->GetMaguneForce(),Size::Small);
				}
				else if (_size == Size::Big)
				{
					OnHit(obj->GetPos(), obj->GetMaguneForce(),Size::Big);
				}
			}
		}

		// オブジェクトが近づきすぎているとき
		else if (hitFlg && _option == ColisionOption::WallDistance)
		{
			//まとっている磁力の極がちがうならHIT処理をする
			if (m_maguneForce != obj->GetMaguneForce())
			{
				OnHit(obj->GetPos());
			}
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
		owner.m_model = std::make_shared<KdModelWork>();
		owner.m_model->SetModelData("Asset/Models/Terrains/Gimmick/BigMovingWall/MovingWall2.gltf");
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

void MovingWall::BigSize::PostUpdate(MovingWall& owner)
{
}

void MovingWall::SmallSize::Enter(MovingWall& owner)
{
	// モデルの読み込み
	if (!owner.m_model)
	{
		owner.m_model = std::make_shared<KdModelWork>();
		owner.m_model->SetModelData("Asset/Models/Terrains/Gimmick/MovingWall/MovingWall.gltf");
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

