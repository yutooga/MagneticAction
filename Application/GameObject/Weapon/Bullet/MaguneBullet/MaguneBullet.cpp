#include "MaguneBullet.h"
#include"../../../../Scene/SceneManager.h"

const float MaguneBullet::k_bulletSize = 0.1f;

void MaguneBullet::Init()
{
	// モデルの読み込み
	if(!m_model)
	{
		m_model = std::make_shared<KdModelWork>();
		m_model->SetModelData("Asset/Models/Weapon/Bullet/MaguneBullet/MaguneBullet.gltf");
	}

	// まとっている磁力の初期化
	m_maguneForce = NoForce;
}

void MaguneBullet::Update()
{
	//弾が生存時間を過ぎたら消す
	m_lifeSpan--;
	if (m_lifeSpan < 0)
	{
		m_isExpired = true;
	}

	// 座標の更新
	m_pos += m_dir * m_speed;

	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(k_bulletSize);
	m_mWorld = scaleMat * m_rotBulletMat * transMat;
}

void MaguneBullet::DrawLit()
{
	if (!m_model)return;

	Math::Color color;
	if ((m_maguneForce & MagunePowerN) != 0)
	{
		//S極
		color = { 1,0,0 };
	}
	else
	{
		//N極
		color = { 0,0,1 };
	}
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld,color);
}

void MaguneBullet::SetParam(Math::Vector3& _pos, Math::Vector3& _dir, int _lifespan, float _speed)
{
	//弾の初期座標をコピー
	m_pos = _pos;
	//進行ベクトルをコピー
	m_dir = _dir;
	m_dir.Normalize();
	//生存時間セット
	m_lifeSpan = _lifespan;
	//スピードセット
	m_speed = _speed;

	//照準へのベクトルと弾の進行ベクトルから弾の向きを作成


	//①ベクトルA　・・・　弾の現在の進行ベクトル
	Math::Vector3 _vecA = m_mWorld.Backward();
	_vecA.Normalize();

	//②　ベクトルB　・・・　照準への進行ベクトル
	Math::Vector3 _vecB = m_dir;
	_vecB.Normalize();

	//①と②のベクトルの内積値から角度を算出
	float _dot = _vecA.Dot(_vecB);//内積値を算出
	float _angle = acos(_dot);//内積値から２つのベクトルの角度を算出

	Math::Vector3 _rotAxis = _vecA.Cross(_vecB);

	//内積で算出した角度分、外積で算出したベクトルで回転する行列を作成
	//　＝弾がレティクルの方向を向く
	m_rotBulletMat = Math::Matrix::CreateFromAxisAngle(_rotAxis, _angle);
}