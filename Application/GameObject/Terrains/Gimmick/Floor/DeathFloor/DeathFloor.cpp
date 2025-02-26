#include "DeathFloor.h"
#include"../../../../../Scene/SceneManager.h"
#include"../../../../../Manager/ModelManager/ModelManager.h"

const float DeathFloor::k_oppositionLimit = 30.f;
const float DeathFloor::k_oppositionPow = 0.5f;
const float DeathFloor::k_correctionValue = 0.2f;
const float DeathFloor::k_addAngleValue = 0.5f;
const float DeathFloor::k_sameForceColisionRadius = 80.f;
const float DeathFloor::k_differentForceColisionRadius = 100.f;
const float DeathFloor::k_opppsitionSpeed = 0.3f;
const float DeathFloor::k_adsorptionSpeed = 0.4f;
const float DeathFloor::k_unitVector = 1.f;

void DeathFloor::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("DeathFloor");
	}

	// モデルの大きさの初期化
	m_modelSize = m_gimmickData["DeathFloor"].value("ModelSize", 8.f);

	// 移動スピードの初期化
	m_moveSpeed = m_gimmickData["DeathFloor"].value("MoveSpeed", 0.5f);

	// 長さ初期化
	m_length = m_gimmickData["DeathFloor"].value("Length", 40.f);

	// 振れ幅初期化
	m_amplitude = m_gimmickData["DeathFloor"].value("Amplitude", 3.f);

	// デバックラインの実態生成
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

	//ImGui用のランダムなIdの生成
	m_randomId = rand();

	// まとっている磁力の初期化
	m_maguneForce = NoForce;

	// 最初の座標を保存
	m_firstPos = m_pos;

	// 当たり判定の形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("DeathFloor", m_model, KdCollider::TypeGround | KdCollider::TypeDamage);
}

void DeathFloor::Update()
{

	if (m_moveState)
	{
		m_moveState->Update(*this);
	}

	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	m_mWorld = scaleMat * transMat;
}

void DeathFloor::PostUpdate()
{
	if (m_moveState)
	{
		m_moveState->PostUpdate(*this);
	}
}

void DeathFloor::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("DeathFloor", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("DeathFloor m_posX", &m_pos.x, 0.1f);
		ImGui::DragFloat("DeathFloor m_posY", &m_pos.y, 0.1f);
		ImGui::DragFloat("DeathFloor m_posZ", &m_pos.z, 0.1f);
		ImGui::DragFloat("length", &m_length, 0.1f);
	}
	ImGui::PopID();
}

void DeathFloor::OnHit(KdGameObject::ObjectType _obj)
{
	// S極の磁力が付与された場合
	if (_obj == KdGameObject::ObjectType::MaguneBulletS)
	{
		KdAudioManager::Instance().Play("Asset/Sounds/GameScene/Terrains/Gimmick/MagunePower.wav",false);
     	m_maguneForce = MagunePowerN;
	}

	// N極の磁力が付与された場合
	else if(_obj == KdGameObject::ObjectType::MaguneBulletN)
	{
		KdAudioManager::Instance().Play("Asset/Sounds/GameScene/Terrains/Gimmick/MagunePower.wav",false);
		m_maguneForce = MagunePowerS;
	}
}

void DeathFloor::ColisionCheck(const float _radius, const ColisionOption& _option,std::list<std::weak_ptr<KdGameObject>>& _hitList)
{
	// 自身が磁力をまとっていない場合早期リターン
	if ((m_maguneForce & NoForce) != 0)return;

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
	//デバック用
	m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius);

	std::list<std::shared_ptr<KdGameObject>> hitFloor;

	//当たり判定
	for (auto& obj : SceneManager::Instance().GetgimmickList())
	{

		ObjectType targetObjType = obj->GetObjType();
		UINT targetForce = obj->GetMaguneForce();

		// 特定のオブジェクト以外とは当たり判定をしない
		if (targetObjType != ObjectType::DeathFloor && targetObjType != ObjectType::MagneticArea && targetObjType != ObjectType::ChangeFloor)continue;
		//磁力を帯びてない物体とは当たり判定しない
		else if ((targetForce & NoForce) != 0)continue;
		// 自分自身と当たり判定しない
		else if ((m_pos - obj->GetPos()).Length() < 1.0)continue;
		else if (targetObjType == ObjectType::DeathFloor && abs(m_pos.z - obj->GetPos().z) > m_length)continue;

		bool hitFlg = false;
		hitFlg = obj->Intersects(sphere, nullptr);
		if (hitFlg && _option == ColisionOption::DifferentForce)
		{
			if (m_maguneForce != targetForce)
			{
				if (targetObjType != ObjectType::ChangeFloor)
				{
					OnHit(obj->GetPos(), targetForce, targetObjType);
				}
				else
				{
					//違う極の磁力をまとっているならHIT時の処理をする
					_hitList.push_back(obj);
				}
			}
		}
		else if (hitFlg && _option == ColisionOption::SameForce)
		{
			if (m_maguneForce == targetForce)
			{
				if (targetObjType != ObjectType::ChangeFloor)
				{
					OnHit(obj->GetPos(), targetForce, targetObjType);
				}
				else
				{
					//同じ極の磁力をまとっているならHIT時の処理をする
					_hitList.push_back(obj);
				}
			}
		}
	}
}

void DeathFloor::MaguneScope()
{
	//===============================
	//              球判定
	//===============================

	//球判定用の変数を作成
	KdCollider::SphereInfo sphere;

	//球の中心位置を設定
	sphere.m_sphere.Center = m_pos;
	//球の半径を設定
	if(m_nowMoveState == MoveState::nomal)
	{
		sphere.m_sphere.Radius = 20.0f;
	}
	else
	{
		sphere.m_sphere.Radius = 40.0f;
	}
	//当たり判定をしたいタイプを設定
	sphere.m_type = KdCollider::TypeSight;

	//当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		if (obj->GetObjType() != ObjectType::Player)continue;
		bool hitFlg = false;
		hitFlg = obj->Intersects(sphere, nullptr);
		if (hitFlg)
		{
			MagneFloorBase::OnHit(obj);
		}
		else
		{
			// 現在当たっていなくて前のフレームで当たっていた時諸々を初期化する
			if (m_nowState == State::Adsorption)
			{
				m_nowState = State::NormalState;
				obj->SetUpdate(false);
			}
		}
	}
}

void DeathFloor::PlayerReaction()
{
	//オブジェクトが存在しないまたは磁力をまとっていないなら早期リターン
	if (m_nowState == State::NormalState || m_obj.expired() == true)return;

	Math::Vector3 moveDir = Math::Vector3::Zero;
	Math::Vector3 addPos = m_obj.lock()->GetPos();

	//反発処理
	if (m_nowState == State::Opposition)
	{
		static int oppCount = 0;

		if (oppCount == 0)
		{
			//反発SE再生
			KdAudioManager::Instance().Play(m_gimmickData["Se"]["Repulsion"]["URL"], false);
		}

		moveDir = m_obj.lock()->GetPos() - m_pos;

		// 地面にめり込まないようにする
		if (moveDir.y <= -k_unitVector)
		{
			moveDir.y = 0.0f;
		}

		moveDir.Normalize();

		addPos += moveDir * m_oppoPow;
		m_oppoPow += k_oppositionPower;
		m_obj.lock()->SettingPos(addPos);
		oppCount++;

		if (oppCount > k_oppositionLimit)
		{
			m_nowState = State::NormalState;
			oppCount = 0;
			m_oppoPow = k_oppositionPow;
		}
	}
	//吸着処理
	else if (m_nowState == State::Adsorption)
	{
		m_adPow = k_adsorptionPower;
		moveDir = m_pos - m_obj.lock()->GetPos();
		if (moveDir.Length() < k_adsorptionPower)m_adPow = moveDir.Length();
		moveDir.Normalize();
		addPos += moveDir * m_adPow;
		m_obj.lock()->SettingPos(addPos);
	}
}

void DeathFloor::OnHit(Math::Vector3 _pos, UINT _maguneForce, const ObjectType& _type)
{
	// 移動方向
	Math::Vector3 moveDir;

	//自分と相手が同じ極の磁力をまとっているとき
	if (m_maguneForce == _maguneForce)
	{

		moveDir = m_pos - _pos;

		// HITしたオブジェクトが磁力の床の場合Y方向以外０にする
		if (_type == ObjectType::ChangeFloor)
		{
			moveDir.x = 0;
			moveDir.z = 0;
		}

		// 速さの初期化
		m_moveSpeed = k_opppsitionSpeed;
	}
	//違う極をまとっているとき
	else if (m_maguneForce != _maguneForce)
	{
		// 速さの初期化
		m_moveSpeed = k_adsorptionSpeed;

		moveDir = m_firstPos - m_pos;
		if (moveDir.Length() < m_moveSpeed + k_correctionValue)m_moveSpeed = 0;
	}

	moveDir.Normalize();
	m_pos += moveDir * m_moveSpeed;
}

void DeathFloor::OnHit(Math::Vector3& _pos)
{
	m_attractFlg = true;
}

void DeathFloor::ChangeMoveState(MoveState state)
{
	switch (state)
	{
	case DeathFloor::MoveState::nomal:
		SetMoveState(std::make_shared<DeathFloor::NormalMove>());
		break;
	case DeathFloor::MoveState::change:
		SetMoveState(std::make_shared<DeathFloor::ChangeMove>());
		break;
	default:
		break;
	}

	m_nowMoveState = state;
}

void DeathFloor::SetMoveState(std::shared_ptr<MoveStateBase> State)
{
	if (m_moveState)m_moveState->Exit(*this);
	m_moveState = State;
	m_moveState->Enter(*this);
}

void DeathFloor::NormalMove::Update(DeathFloor& owner)
{
	// フラグの初期化
	owner.m_attractFlg = false;
}

void DeathFloor::NormalMove::PostUpdate(DeathFloor& owner)
{

	owner.MagneFloorBase::PostUpdate();

	// 当たっている磁力の変わる床を一時的に記憶しておく
	std::list<std::weak_ptr<KdGameObject>> hitChangeFloors;

	//違う極の磁力をまとった壁同士の当たり判定
	owner.ColisionCheck(k_differentForceColisionRadius, ColisionOption::DifferentForce, hitChangeFloors);
	//同じ極の磁力をまとった壁同士の当たり判定
	owner.ColisionCheck(k_sameForceColisionRadius, ColisionOption::SameForce, hitChangeFloors);

	float range = m_gimmickData["DeathFloor"].value("Range", 1000000.f);
	Math::Vector3 targetPos;
	UINT force;
	ObjectType type;
	bool hitFlg = false;


	// 一番近くにある床だけ当たり判定をする
	for (auto& obj : hitChangeFloors)
	{
		if (range > (owner.m_pos - obj.lock()->GetPos()).Length() && obj.expired() == false)
		{
			hitFlg = true;
			targetPos = obj.lock()->GetPos();
			force = obj.lock()->GetMaguneForce();
			type = obj.lock()->GetObjType();
			range = (owner.m_pos - obj.lock()->GetPos()).Length();
		}
	}

	if (hitFlg)
	{
		owner.OnHit(targetPos, force,type);
	}
}

void DeathFloor::ChangeMove::Enter(DeathFloor& owner)
{
	// オブジェクトのX座標を保管する
	owner.m_firstPosX = owner.m_pos.x;
}

void DeathFloor::ChangeMove::Update(DeathFloor& owner)
{
	owner.m_previousPosition = owner.m_pos;

	// 角度の更新
	owner.m_moveAngle += k_addAngleValue;

	// 角度が最大値を超えないようにする
	if (owner.m_moveAngle >= k_rotAngleMax)
	{
		owner.m_moveAngle -= k_rotAngleMax;
	}
	owner.m_pos.x += sin(DirectX::XMConvertToRadians(owner.m_moveAngle)) * owner.m_amplitude;
}

void DeathFloor::ChangeMove::PostUpdate(DeathFloor& owner)
{
	owner.MagneFloorBase::PostUpdate();
}
