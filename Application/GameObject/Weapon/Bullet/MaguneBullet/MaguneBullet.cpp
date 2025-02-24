#include "MaguneBullet.h"
#include"../../../../Scene/SceneManager.h"

const float MaguneBullet::k_rgbMax = 1.0f;

void MaguneBullet::Init()
{
	// jsonファイルから情報を読み込んでくる
	if (m_magneBulletData.empty())
	{
		std::ifstream ifs("Asset/Data/GameScene/3DObject/Weapon/MagneBullet/MagneBulletData.json");
		if (ifs) {
			ifs >> m_magneBulletData;
		}
	}

	// モデルの読み込み
	if(!m_model)
	{
		m_model = std::make_shared<KdModelWork>();
		m_model->SetModelData(m_magneBulletData["URL"]);
	}

	// モデルのサイズの初期化
	m_modelSize = m_magneBulletData.value("BulletSize", 0.1f);

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
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	m_mWorld = scaleMat * m_rotBulletMat * transMat;
}

void MaguneBullet::DrawLit()
{
	if (!m_model)return;

	Math::Color color;
	if ((m_maguneForce & MagunePowerN) != 0)
	{
		// N極
		color = { k_rgbMax,0,0 };
	}
	else
	{
		// S極
		color = { 0,0,k_rgbMax };
	}
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld,color);
}

void MaguneBullet::SetParam(const Math::Vector3& _pos, const Math::Vector3& _dir, const int _lifespan, const float _speed)
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


	// 弾の現在の進行ベクトル
	Math::Vector3 _vecA = m_mWorld.Backward();
	_vecA.Normalize();

	// 照準への進行ベクトル
	Math::Vector3 _vecB = m_dir;
	_vecB.Normalize();

	// ベクトルの内積値から角度を算出
	float _dot = _vecA.Dot(_vecB);
	float _angle = acos(_dot);

	Math::Vector3 _rotAxis = _vecA.Cross(_vecB);

	//内積で算出した角度分、外積で算出したベクトルで回転する行列を作成
	m_rotBulletMat = Math::Matrix::CreateFromAxisAngle(_rotAxis, _angle);
}