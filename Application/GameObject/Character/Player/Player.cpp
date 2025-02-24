#include "Player.h"
#include"Application/GameObject/Camera/CameraBase.h"
#include"../../../Scene/SceneManager.h"
#include"../../2DObject/Hp/Hp.h"
#include"../../../main.h"
#include"../../2DObject/GameOver/GameOver.h"
#include"../../Manager/GoalManager/GoalManager.h"
#include"../../Manager/StartManager/StartManager.h"

//====================================================
const float Player::k_deathPlayerHeight = -1000.f;
const float Player::k_shadowRotAngle = 90.f;
const float Player::k_unitVector = 1.f;
const float Player::k_colisionRadius = 1.f;
//====================================================

void Player::Update()
{
	//カウントダウンが終わっていないなら早期リターン
	if (StartManager::instance().GetStartState() == false)return;

	// プレイヤーののこりHPが０ならば早期リターン
	if (m_hp.expired() == false)
	{
		if (m_hp.lock()->GetHpNum() <= 0)
		{
			return;
		}
	}

	// プレイヤーがゴールしたなら早期リターン
	if (GoalManager::instance().GetClearState())return;

	//SE更新
	SeUpdate();

	//進行方向をクリア
	m_moveDir = Math::Vector3::Zero;

	// 移動スピードの初期化
	if (!m_moveDownFlg)
	{
		m_moveSpeed = m_jsonData["MoveSpeed"]["NormalSpeed"];	// 引っ付いた状態でない場合
	}
	else
	{
		m_moveSpeed = m_jsonData["MoveSpeed"]["DownSpeed"];	// 引っ付いた状態のとき
	}

	//キャラ制御（進行ベクトルの情報を更新）
	if (GetAsyncKeyState('D') & 0x8000) { m_moveDir.x = k_unitVector; }
	if (GetAsyncKeyState('A') & 0x8000) { m_moveDir.x = -k_unitVector; }
	if (GetAsyncKeyState('W') & 0x8000) { m_moveDir.z = k_unitVector; }
	if (GetAsyncKeyState('S') & 0x8000) { m_moveDir.z = -k_unitVector; }
	

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		// 反発状態でないとき
		if (!m_jumpKeyFlg && (!m_attractionFlg))
		{
			m_jumpKeyFlg = true;
			float jumpPow = m_jsonData["Jump"]["JumpPow"];
			m_gravity = -jumpPow;
		}
	}

	//ベクトルの向きをｙ軸の回転行列で変換

	if (!(GetAsyncKeyState(VK_LSHIFT) & 0x8000))
	{
		UpdateRotateByMouse();
	}

	m_moveDir = m_moveDir.TransformNormal(m_moveDir, GetRotationYMatrix());

	// 行動制限がかかっているときは動けない
	if (m_moveFlg)
	{
		m_moveDir.z = 0;
	}

	//確定した向き情報を正規化
	m_moveDir.Normalize();
	//座標更新
	//座標+=向き情報　＊　移動速度
	
	m_moveDir *= m_moveSpeed;
	m_pos += m_moveDir;

	//重力更新
	GravityUpdate();

	m_updateFlg = false;


	//プレイヤーの落下死判定
	if (m_pos.y <= k_deathPlayerHeight)
	{
		m_pos = m_returnPos;
		if (m_hp.expired() == false)
		{
			m_hp.lock()->DecreaseHp();
		}
	}

	//デバック処理
	Debug();
}

void Player::PostUpdate()
{
	m_rideOnFlg = false;

	// 斥力のフラグの初期化
	m_attractionFlg = false;

	// 移動スピード低下フラグの初期化
	m_moveDownFlg = false;

	// プレイヤーの影の描画判断
	JudgmentShadow();

	//地面とプレイヤーの当たり判定
	Player_TerrainColision();
	Player_TerrainSphereColision();

	// 磁力エリアにいるかどうか判断する関数
	OnTheArea();

	// キャラクターのワールド行列を創る処理
	Math::Matrix _transMat = Math::Matrix::CreateTranslation(m_pos);

	Math::Matrix _rotMat = GetRotationMatrix();

	m_mWorld =  _rotMat *  _transMat;

	// 影の行列確定
	Math::Matrix _shadowTransMat = Math::Matrix::CreateTranslation(m_shadowPos);
	Math::Matrix _shadowScaleMat = Math::Matrix::CreateScale(m_shadowSize);
	Math::Matrix _shadowRotMat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(k_shadowRotAngle));
	m_shadowMat = _shadowScaleMat * _shadowRotMat * _shadowTransMat;

	//ゲームオーバー処理
	GameOverPlayer();

	// ゲームオーバー状態になった時シーンを切り替える処理
	ChangeScene();
}

void Player::Init()
{

	// jsonファイルから情報を読み込んでくる

	std::ifstream ifs("Asset/Data/GameScene/Player/Player.json");
	if (ifs) {
		ifs >> m_jsonData;
	}

	std::ifstream debug("Asset/Data/GameScene/Player/PlayerDebugPos.json");
	if (debug)
	{
		debug >> m_debugData;
	}

	// 影の画像の読み込み
	m_polygon = std::make_shared<KdSquarePolygon>();
	m_polygon->SetMaterial(m_jsonData["Shadow"]["URL"]);

	// 影の大きさの設定
	m_shadowSize = m_jsonData["ShadowColision"].value("ShadowSize", 10.f);

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

	// 復帰座標の初期化
	m_returnPos = {
		m_jsonData["ReturnPos"].value("X",0.f),
		m_jsonData["ReturnPos"].value("Y",18.f),
		m_jsonData["ReturnPos"].value("Z",0.f)
	};

	// 移動スピードの初期化
	m_moveSpeed = m_jsonData["MoveSpeed"]["NormalSpeed"];

	// プレイヤーの初期座標の補正
	m_pos.y = m_jsonData["PlayerFirstPosY"];
	m_pos.y += m_jsonData["AdjustHeight"];

	//当たり判定用の初期化
	{
		DirectX::BoundingSphere sphere;
		//球の中心位置を設定
		sphere.Center = m_pos;
		sphere.Center.y -= m_jsonData["PlayerColision"]["AdjustHeightY"];
		sphere.Center.y -= m_jsonData["AdjustHeight"];
		//球の半径を設定
		sphere.Radius = m_jsonData["PlayerColision"]["Radius"];
		m_pCollider = std::make_unique<KdCollider>();
		m_pCollider->RegisterCollisionShape("PlayerColision", sphere.Center, sphere.Radius, KdCollider::TypeSight);
	}

	//プレイヤーのサイズ
	m_size = { m_jsonData["PlayerSize"],m_jsonData["PlayerSize"],m_jsonData["PlayerSize"] };

	// まとう磁力の初期化
	m_maguneForce = NoForce;

	//カメラのローカル行列
	m_localCamera = m_mWorld;

	// カメラ回転用マウス座標の差分の初期化
	m_FixMousePos = { m_jsonData["Mouse"]["Pos"].value("X",640),
		m_jsonData["Mouse"]["Pos"].value("Y",360) };
}

void Player::OnHit(ObjectType _obj)
{
	switch (_obj)
	{
	case KdGameObject::ObjectType::SavePoint:
		m_returnPos = m_pos;
		break;
	case KdGameObject::ObjectType::DeathFloor:
		if(m_seInterval==0)
		{
			// SE再生
			KdAudioManager::Instance().Play("Asset/Sounds/GameScene/Terrains/Gimmick/DeathFloor/DeathFloor.wav", false);
			m_hp.lock()->DecreaseHp();
			// SE再生のクールタイムの設定
			int coolTime = m_jsonData["SeCoolTime"];
			m_seInterval = coolTime;
		}
		break;
	case KdGameObject::ObjectType::Goal:
		// ゴールのZ座標より小さいならゴールしたとみなす
		if (m_pos.z <= GoalManager::instance().GetGoalPos().z)
		{
			if(!m_clearFlg)
			{
				m_clearFlg = true;
				// クリアテキストの作成
				GoalManager::instance().CreateClearText();
				// プレイヤーがクリアしたことを通知する
				GoalManager::instance().InformClear();
			}
		}
		break;
	default:
		break;
	}
}

void Player::DrawImGui()
{
	ImGui::LabelText("playerPosX", "playerPos % .2f", m_pos.x);
	ImGui::LabelText("playerPosY", "playerPos % .2f", m_pos.y);
	ImGui::LabelText("playerPosZ", "playerPos % .2f", m_pos.z);

	ImGui::DragFloat3("playerPos m_pos", &m_pos.x, 0.1f);
	ImGui::DragFloat("m_shadowSize", &m_shadowSize, 0.1f);
}

void Player::DrawUnLit()
{
	// プレイヤーの下に地面がないのなら影を描画しないd
	if (!m_shadowDrawFlg)return;

	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_polygon,m_shadowMat);
}

void Player::ChangeScene()
{
	if (m_gameover.expired() == true)return;
	// ゲームオーバーのテキストが完全に描画しきったらゲームオーバー状態にする
	if (m_gameover.lock()->GetDrawEnd())
	{
		m_gameoverFlg = true;
	}
}

void Player::UpdateRotateByMouse()
{
	// マウスでカメラを回転させる処理
	POINT _nowPos;
	GetCursorPos(&_nowPos);

	POINT _mouseMove{};
	_mouseMove.x = _nowPos.x - m_FixMousePos.x;
	_mouseMove.y = _nowPos.y - m_FixMousePos.y;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	// 実際にカメラを回転させる処理

	float rotation = m_jsonData["Camera"]["RotationAdjustValue"];

	m_degAng.x += _mouseMove.y * rotation;
	m_degAng.y += _mouseMove.x * rotation;

	// 回転制御

	float RotationMax = m_jsonData["Camera"]["RotationAngleMax"];

	m_degAng.x = std::clamp(m_degAng.x, -RotationMax, RotationMax);
}

void Player::Player_TerrainColision()
{
	std::weak_ptr<KdGameObject> m_obj;

	//地面とのレイ判定

	//レイ判定用に変数を作成
	KdCollider::RayInfo ray;

	//レイの発射位置
	ray.m_pos = m_pos;
	//レイの発射方法
	ray.m_dir = Math::Vector3::Down;

	{
		float adjustHeight = m_jsonData["AdjustHeight"];
		float enableStepHigh = m_jsonData["ShadowColision"]["EnableStepHigh"];

		ray.m_pos.y = ray.m_pos.y - adjustHeight + enableStepHigh;
		//レイの長さを設定
		ray.m_range = m_gravity + enableStepHigh;
	}

	//当たり判定をしたいタイプを設定
	ray.m_type = KdCollider::TypeGround;

	//レイに当たったオブジェクト情報を格納
	std::list<KdCollider::CollisionResult> retRayList;
	//当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		bool hitFlg = false;
		hitFlg = obj->Intersects(ray, &retRayList);

		if (hitFlg)
		{
			//当たった時の処理
			OnHit(obj->GetObjType(),obj);
			m_obj = obj;
		}
		else
		{
			m_seFlg = false;
		}
	}

	if (!m_getOnJumpFlg)
	{
		//レイに当たったリストから一番近いオブジェクトを検出
		float maxOverLap = 0;
		Math::Vector3 hitPos;
		bool isHit = false;

		for (auto& ret : retRayList)
		{
			if (maxOverLap < ret.m_overlapDistance)
			{
				maxOverLap = ret.m_overlapDistance;
				hitPos = ret.m_hitPos;
				isHit = true;
			}
		}
		if (isHit)
		{
			if(!m_rideOnFlg)
			{
				m_pos = hitPos;
				m_pos.y += m_jsonData["AdjustHeight"];
			}
			
			m_gravity = 0;
			m_jumpKeyFlg = false;
		}
		else
		{
			m_jumpKeyFlg = true;
		}
	}

	m_getOnJumpFlg = false;

	// 反発処理
	Repulsion();
}

void Player::Player_TerrainSphereColision()
{
	//===============================
	//              球判定
	//===============================

	//球判定用の変数を作成
	KdCollider::SphereInfo sphere;

	//球の中心位置を設定

	float adjustValueY = m_jsonData["ShadowColision"]["AdjustValueY"];

	sphere.m_sphere.Center = m_pos;
	sphere.m_sphere.Center.y += adjustValueY;
	//球の半径を設定
	sphere.m_sphere.Radius = k_colisionRadius;
	//当たり判定をしたいタイプを設定
	sphere.m_type = KdCollider::TypeGround;

	//球に当たったオブジェクト情報を格納
	std::list<KdCollider::CollisionResult> retSphereList;

	//当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		bool hitFlg = false;
		hitFlg = obj->Intersects(sphere, &retSphereList);
		// 当たっていてそれがなおかつ死の床or動く死の床の時
		if ((hitFlg && obj->GetObjType() == ObjectType::DeathFloor) || (hitFlg && obj->GetObjType() == ObjectType::MovingDeathFloor))
		{
			// クールタイムが０の時に再生
			if (m_seInterval != 0)continue;
			
			// SE再生
			KdAudioManager::Instance().Play("Asset/Sounds/GameScene/Terrains/Gimmick/DeathFloor/DeathFloor.wav", false);
			if (m_hp.expired() == false)
			{
				// HP減算処理
				m_hp.lock()->DecreaseHp();
			}
			// SE再生のクールタイムの設定
			m_seInterval = m_jsonData.value("SeCoolTime", 60);
		}
	}

	//球に当たったオブジェクトを検出
	float maxOverLap = 0;
	Math::Vector3 hitDir;
	bool isHit = false;

	for (auto& ret : retSphereList)
	{
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			hitDir = ret.m_hitDir;
			isHit = true;
		}
	}

	if (isHit)
	{
		//方向ベクトルは長さを１にしないと駄目
		//正規化（長さを１にする）
		hitDir.Normalize();

		//押し戻し処理
		m_pos += hitDir * maxOverLap;
	}
	
}

void Player::OnTheArea()
{
	// 磁力ゾーンに入っていないまたは、磁力を帯びていないなら早期リターン
	if (!m_onTheAreaFlg)return;
	else if ((m_maguneForce & NoForce) != 0)return;

	////レイ判定用に変数を作成
	std::vector<KdCollider::SphereInfo> spheres;

	// 当たり判定の球の個数
	float circleNum = m_jsonData["ColisionDataForMagneArea"]["CircleNum"];

	for (int i = 0; i < circleNum; i++)
	{
		KdCollider::SphereInfo sphere;
		sphere.m_sphere.Center = m_pos;
		sphere.m_sphere.Radius = m_jsonData["ColisionDataForMagneArea"]["CircleRadius"];
		sphere.m_type = KdCollider::TypeGround;
		spheres.push_back(sphere);
	}

	//レイの発射位置
	spheres[static_cast<int>(ColisionCircle::Center)].m_sphere.Center.y -= m_jsonData["ColisionDataForMagneArea"].value("AdjustHighCenter", 60.f);
	spheres[static_cast<int>(ColisionCircle::Under)].m_sphere.Center.y -= m_jsonData["ColisionDataForMagneArea"].value("AdjustHighUnder", 120.f);

	bool isHit = false;
	float distance = 0.0f;

	//当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		// 特定のオブジェクトではない場合は読み飛ばす
		if (obj->GetObjType() != KdGameObject::ObjectType::MagneticArea)continue;
		// まとっている磁力が一緒または、対象のオブジェクトが磁力をまとっていないなら読み飛ばす
		else if ((m_maguneForce != obj->GetMaguneForce()) || ((obj->GetMaguneForce() & NoForce) != 0))continue;

		bool hitFlg = obj->Intersects(spheres[static_cast<int>(ColisionCircle::Top)], nullptr);
		if (!hitFlg)
		{
			hitFlg = obj->Intersects(spheres[static_cast<int>(ColisionCircle::Center)], nullptr);
		}
		if (!hitFlg)
		{
			hitFlg = obj->Intersects(spheres[static_cast<int>(ColisionCircle::Under)], nullptr);
		}
		if (hitFlg)
		{
			isHit = true;
			m_updateFlg = true;
			Math::Vector3 objPos = obj->GetPos();
			distance = fabsf(m_pos.y - objPos.y);
			break;
		}
		else
		{
			m_updateFlg = false;
		}
	}

	if (isHit)
	{
		float range = m_jsonData["Magnet"]["MagnetRange"];
		float power = m_jsonData["Magnet"]["MagnetPower"];

		// このマグネットがプレイヤーを動かすほど近くにいるか
		if (distance <= range)
		{
			m_pos.y += power;// 問答無用でプレイヤーを動かす
		}
		else
		{
			// 上まで持ち上げられた
			float speed = m_jsonData["ColisionDataForMagneArea"]["FloatingSpeed"];
			m_addAmount += DirectX::XMConvertToRadians(power);
			m_pos.y += sin(m_addAmount) * speed;
		}
	}
	else
	{
		m_addAmount = 0.0f;
		m_onTheAreaFlg = false;
	}
}

void Player::Repulsion()
{
	// 反発の処理
	if (!m_repulsionFlg)return;

	{
		float repulsionAddPow = m_jsonData["Repulsion"]["RepulsionAddPow"];
		m_repulsionPow += repulsionAddPow;
		m_pos += m_repulsionDir * m_repulsionPow;
	}

	// 反発状態の限界を迎えたら初期状態に戻す

	float repulsionLimit = m_jsonData["Repulsion"]["RepulsionLimit"];

	if (m_repulsionPow > repulsionLimit)
	{
		m_repulsionPow = 0.0f;
		m_repulsionFlg = false;
	}
}

void Player::JudgmentShadow()
{
	std::weak_ptr<KdGameObject> m_obj;

	//地面とのレイ判定

	//レイ判定用に変数を作成
	KdCollider::RayInfo ray;

	//レイの発射位置
	ray.m_pos = m_pos;
	//レイの発射方法
	ray.m_dir = Math::Vector3::Down;

	float enableStepHigh = m_jsonData["ShadowColision"]["EnableStepHigh"];
	ray.m_pos.y = ray.m_pos.y - m_jsonData["AdjustHeight"] + enableStepHigh;
	//レイの長さを設定
	ray.m_range = m_jsonData["ShadowColision"]["Range"];
	//当たり判定をしたいタイプを設定
	ray.m_type = KdCollider::TypeGround;

	//レイに当たったオブジェクト情報を格納
	std::list<KdCollider::CollisionResult> retRayList;
	//当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects(ray, &retRayList);
	}

	float maxOverLap = 0;
	bool isHit = false;

	Math::Vector3 hitPos;

	for (auto& ret : retRayList)
	{
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			hitPos = ret.m_hitPos;
			isHit = true;
		}
	}

	// もし当たっているなら描画フラグをONにする
	if (isHit)
	{
		// 影の描画位置の決定
		m_shadowPos = hitPos;
		// 描画位置の補正
		m_shadowPos.y += m_jsonData["ShadowColision"]["DrawAdjustValue"];
		float distance = m_pos.y - hitPos.y;
		m_shadowSize = m_jsonData["ShadowColision"]["ShadowSize"] - distance / 5;
		m_shadowDrawFlg = true;
	}
	else
	{
		m_shadowDrawFlg = false;
	}
}

void Player::GameOverPlayer()
{
	if (m_gameover.expired() == false)return;

	if (m_hp.expired() != false)return;

	//HPが０になったらゲームオーバー画面にする
	if (m_hp.lock()->GetHpNum() <= 0)
	{
		//=====================================
		// ゲームオーバー初期化
		//=====================================
		std::shared_ptr<GameOver> gameover = std::make_shared<GameOver>();
		gameover->Init();
		m_gameover = gameover;
		SceneManager::Instance().AddObject(gameover);

		// 音関係を全て止める
		KdAudioManager::Instance().StopAllSound();
		// ゲームオーバーのSEを流す
		KdAudioManager::Instance().Play("Asset/Sounds/GameScene/2DObject/GameOver/GameOver.wav");
	}
}

void Player::SeUpdate()
{
	//早期リターン
	if (m_seInterval == 0)return;

	// SE再生のクールタイム更新処理
	m_seInterval--;
	if (m_seInterval <= 0)
	{
		m_seInterval = 0;
	}
}

void Player::GravityUpdate()
{
	//更新判断フラグがONなら早期リターン
	if (m_updateFlg||m_reactionFlg)
	{
		m_gravity = 0;
		return;
	}

	//重力をキャラに反映
	m_pos.y -= m_gravity;
	float GravityPow = m_jsonData["Jump"]["GravityPow"];
	m_gravity += GravityPow;
}

void Player::DownMoveSpeed(const std::weak_ptr<KdGameObject>& _obj)
{
	if (_obj.expired() == true)return;
	// 自分または対象のオブジェクトが磁力をまとっていないなら早期リターン
	else if (((m_maguneForce & NoForce) != 0) || ((_obj.lock()->GetMaguneForce() & NoForce) != 0))return;

	if (m_maguneForce != _obj.lock()->GetMaguneForce())
	{
		// 移動力低下状態にする
		m_moveDownFlg = true;
	}
}

void Player::Debug()
{
	if (GetAsyncKeyState('1') & 0x8000)
	{
		m_pos = { m_debugData["FirstGimmick"]["x"],m_debugData["FirstGimmick"]["y"],m_debugData["FirstGimmick"]["z"] };
	}

	if (GetAsyncKeyState('2') & 0x8000)
	{
		m_pos = { m_debugData["Tutorial"]["x"],m_debugData["Tutorial"]["y"],m_debugData["Tutorial"]["z"] };
	}

	if (GetAsyncKeyState('3') & 0x8000)
	{
		m_pos = { m_debugData["MoveLift"]["x"],m_debugData["MoveLift"]["y"],m_debugData["MoveLift"]["z"] };
	}

	if (GetAsyncKeyState('4') & 0x8000)
	{
		m_pos = { m_debugData["GetGun"]["x"],m_debugData["GetGun"]["y"],m_debugData["GetGun"]["z"] };
	}

	if (GetAsyncKeyState('5') & 0x8000)
	{
		m_pos = { m_debugData["ElectroMagneticInduction"]["x"],m_debugData["ElectroMagneticInduction"]["y"],m_debugData["ElectroMagneticInduction"]["z"] };
	}

	if (GetAsyncKeyState('6') & 0x8000)
	{
		m_pos = { m_debugData["MagneArea"]["x"],m_debugData["MagneArea"]["y"],m_debugData["MagneArea"]["z"] };
	}

	if (GetAsyncKeyState('7') & 0x8000)
	{
		m_pos = { m_debugData["ThornObjectAndChangeFloor"]["x"],m_debugData["ThornObjectAndChangeFloor"]["y"],m_debugData["ThornObjectAndChangeFloor"]["z"] };
	}

	if (GetAsyncKeyState('8') & 0x8000)
	{
		m_pos = { m_debugData["LastGimmick"]["x"],m_debugData["LastGimmick"]["y"],m_debugData["LastGimmick"]["z"] };
	}
}

// プレイヤー独自の当たられた時の処理
void Player::OnHit(ObjectType _objType, const std::weak_ptr<KdGameObject>& _obj)
{
	// 実体がないなら処理をしない
	if (_obj.expired() == true)return;

	switch (_objType)
	{
	//魔法陣（N極の磁力を付与）
	case KdGameObject::ObjectType::MagicRingN:
	{
		if ((m_maguneForce != MagunePowerS) && (!_obj.lock()->GetReaction()))
		{
			//SE再生
			KdAudioManager::Instance().Play("Asset/Sounds/GameScene/Terrains/Gimmick/MagicRing/Magic.wav");
			m_maguneForce = MagunePowerS;
			_obj.lock()->SetUpdate(true);
			_obj.lock()->SetReaction(true);
		}
		break;
	}
	//魔法陣（S極の磁力を付与）
	case KdGameObject::ObjectType::MagicRingS:
	{
		if ((m_maguneForce != MagunePowerN) && (!_obj.lock()->GetReaction()))
		{
			//SE再生
			KdAudioManager::Instance().Play("Asset/Sounds/GameScene/Terrains/Gimmick/MagicRing/Magic.wav");
			m_maguneForce = MagunePowerN;
			_obj.lock()->SetUpdate(true);
			_obj.lock()->SetReaction(true);
		}
		break;
	}
	//磁力のオーラを消す魔法陣
	case KdGameObject::ObjectType::NoForceRing:
	{
		m_maguneForce = NoForce;
		break;
	}
	// 磁力の切り替わる床
	case KdGameObject::ObjectType::ChangeFloor:
		// 磁力をまとう床の斥力処理
		if (_obj.lock()->GetMaguneForce() != m_maguneForce && (m_maguneForce & NoForce) == 0 )
		{
			m_attractionFlg = true;
			// 引っ付いている状態の時は動く速さを下げる
			DownMoveSpeed(_obj);
		}
		break;
	case KdGameObject::ObjectType::Goal:
		if (m_pos.z >= GoalManager::instance().GetGoalPos().z)
		{
			if (m_clearFlg)break;
			m_clearFlg = true;
			GoalManager::instance().CreateClearText();
			GoalManager::instance().InformClear();
		}
		break;
	case KdGameObject::ObjectType::MagneticArea:
	{
		m_onTheAreaFlg = true;
		break;
	}
	case KdGameObject::ObjectType::MaguneticFloor:
	{
		// 引っ付いている状態の時は動く速さを下げる
		DownMoveSpeed(_obj);
		break;
	}
	case KdGameObject::ObjectType::MoveMagunet:
	{
		// 引っ付いている状態の時は動く速さを下げる
		DownMoveSpeed(_obj);
		break;
	}
	case KdGameObject::ObjectType::BigMovingWall:
	{
		
		// プレイヤーをリフトに追従させる
		if (_obj.lock()->GetUpdate())
		{
			m_pos += _obj.lock()->GetDistancePos();
		}
		if (_obj.lock()->GetDistancePos().Length() != 0)
		{
			m_rideOnFlg = true;
		}
		break;
	}
	case KdGameObject::ObjectType::MoveMagneFloorNomalVer:
	{
		m_pos += _obj.lock()->GetDistancePos();
		m_rideOnFlg = true;
		break;
	}
	default:
		break;
	}
}
