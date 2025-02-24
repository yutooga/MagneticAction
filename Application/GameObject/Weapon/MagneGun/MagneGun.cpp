#include "MagneGun.h"
#include"../../Character/Player/Player.h"
#include"../Bullet/MaguneBullet/MaguneBullet.h"
#include"../../../Scene/SceneManager.h"

const float MagneGun::k_grantScope = 400.f;

void MagneGun::Init()
{
	// jsonファイルから情報を読み込んでくる
	std::ifstream ifs("Asset/Data/GameScene/3DObject/Weapon/MagneGun/MagneGunData.json");
	if (ifs) {
		ifs >> m_magneGunData;
	}

	// モデルの読み込み
	if(!m_model)
	{
		m_model = std::make_shared<KdModelData>();
		m_model->Load(m_magneGunData["URL"]);
	}

	// 座標の初期化
	m_pos = { m_magneGunData["FirstPos"].value("X",0.35f),m_magneGunData["FirstPos"].value("Y",-0.25f),m_magneGunData["FirstPos"].value("Z",0.2f) };

	// 回転角度の初期化
	m_rotAngle = m_magneGunData.value("DefaultAngle", 90.f);

	// モデルのサイズの初期化
	m_modelSize = m_magneGunData.value("Scale", 0.005f);

	//親から銃口へのローカル行列
	//銃本体から銃口への相対位置と親から武器本体への相対位置を
	Math::Vector3 localMuzzlePos = { m_magneGunData["LocalMuzzlePos"].value("X",0.0f),
		m_magneGunData["LocalMuzzlePos"].value("Y",0.105f),m_magneGunData["LocalMuzzlePos"].value("Z",0.5f) };
	m_localMuzzleMat = Math::Matrix::CreateTranslation(localMuzzlePos);
	m_localMuzzleMat *= Math::Matrix::CreateTranslation(m_pos);

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void MagneGun::Update()
{
	//更新フラグがfalseなら早期リターン
	if (!m_updateFlg)return;

	// プレイヤーの実態がないなら早期リターン
	std::shared_ptr<Player> _spParent = m_parent.lock();
	if (!_spParent)return;

	Math::Matrix parentMat = _spParent->GetMatrix();	// 親の行列を持ってくる

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		if(!m_shotFlg)
		{
			m_shotFlg = true;
			//S極の磁力を帯びた弾発射
			Shot(_spParent, MagunePowerS);
		}
	}
	else if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (!m_shotFlg)
		{
			m_shotFlg = true;
			//S極の磁力を帯びた弾発射
			Shot(_spParent, MagunePowerN);
		}
	}
	else
	{
		m_shotFlg = false;
	}

	
	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rotAngle));
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	m_mWorld = scaleMat * rotMat * transMat * parentMat;
}

void MagneGun::DrawLit()
{
	//更新フラグがfalseなら早期リターン
	if (!m_updateFlg)return;

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model,m_mWorld);
}

void MagneGun::Shot(const std::shared_ptr<const Player>& _parent,const UINT& _maguneForce)
{
	// 銃口の座上を取得
	Math::Vector3 _muzzlePos = (m_localMuzzleMat * _parent->GetMatrix()).Translation();

	//弾の発射方向
	Math::Vector3 _bulletDir;

	//レイ判定用に変数を作成
	KdCollider::RayInfo ray;

	//レイの発射位置

	ray.m_pos = _parent->GetPos();
	//レイの発射方法
	ray.m_dir = _parent->GetMatrix().Backward();
	_bulletDir = ray.m_dir;
	//レイの長さを設定
	ray.m_range = m_magneGunData.value("RayRange", 10000.0f);
	//当たり判定をしたいタイプを設定
	ray.m_type = KdCollider::Type::TypeGround | KdCollider::TypeDamage;

	//レイに当たったオブジェクト情報を格納
	std::list<KdCollider::CollisionResult> retRayList;
	std::list<std::weak_ptr<KdGameObject>> hitObjList;
	
	//当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		// 魔法陣は処理を飛ばす
		if (obj->GetObjType() == ObjectType::MagicRingS || obj->GetObjType() == ObjectType::MagicRingN || obj->GetObjType() == ObjectType::NoForceRing)continue;

		// 衝突判定フラグ
		bool hitFlg = false;
		hitFlg = obj->Intersects(ray, &retRayList);
		// 当たっているなら当たったものリストにぶっこむ
		if (hitFlg) { hitObjList.push_back(obj); }
	}

	
	float maxOverLap = 0;
	Math::Vector3 hitPos = Math::Vector3::Zero;
	bool isHit = false;
	auto itr = hitObjList.begin();
	std::weak_ptr<KdGameObject> hitObj;

	// レイに当たったリストから一番近いオブジェクトを検出
	for (auto& ret : retRayList)
	{
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			hitPos = ret.m_hitPos;
			hitObj = *itr;
			isHit = true;
		}
		itr++;
	}
	
	if(isHit)
	{
		//レイの着弾点を利用して弾を発射するベクトルを算出
		_bulletDir = hitPos - _muzzlePos;

		if ((hitPos - ray.m_pos).Length() < k_grantScope)
		{
			if (hitObj.expired() == false)
			{
				hitObj.lock()->SetMaguneForce(_maguneForce);
			}

			
			Math::Vector3 angle = _parent->GetRotateAngle();
			float size = m_magneGunData["Effect"].value("Size", 3.0f);
			float speed = m_magneGunData["Effect"].value("Speed", 1.0f);


			if ((_maguneForce & MagunePowerN) != 0)
			{
				KdEffekseerManager::GetInstance().Play("Simple_GeneratingPosition1.efkproj", hitPos, size, speed, false, angle);
			}
			else if ((_maguneForce & MagunePowerS) != 0)
			{
				KdEffekseerManager::GetInstance().Play("Simple_GeneratingPosition2.efkproj", hitPos, size, speed, false, angle);
			}
		}
	}
	// ベクトルの正規化
	_bulletDir.Normalize();

	// 銃弾の実体生成
	{
		std::shared_ptr<MaguneBullet> bullet = std::make_shared<MaguneBullet>();
		bullet->Init();

		int lifeSpan = m_magneGunData["Bullet"].value("LifeSpan", 1000);
		float speed = m_magneGunData["Bullet"].value("Speed", 13.5f);
		bullet->SetParam(_muzzlePos, _bulletDir, lifeSpan, speed);
		// 銃の弾に磁力を付与する
		bullet->SetMaguneForce(_maguneForce);
		SceneManager::Instance().AddObject(bullet);
	}


	// SE再生
	KdAudioManager::Instance().Play("Asset/Sounds/GameScene/Weapon/MaguneGun/MaguneGun.wav", false);
}
