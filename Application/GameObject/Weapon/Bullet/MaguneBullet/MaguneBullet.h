#pragma once

class MaguneBullet : public KdGameObject
{
public:

	MaguneBullet(){}
	~MaguneBullet(){}

	void Init()override;

	void Update()override;

	void DrawLit()override;

	void SetParam(const Math::Vector3& _pos, const Math::Vector3& _dir, const int _lifespan, const float _speed);

	static const float k_rgbMax;	// 色の最大値

private:
	// モデルの実体
	std::shared_ptr<KdModelWork> m_model;

	//移動方向
	Math::Vector3 m_dir;

	//弾の速さ
	float m_speed = 0.0f;

	//弾の生存時間
	int m_lifeSpan = 0;

	// モデルのサイズ
	float m_modelSize = 0.0f;

	// 弾の回転行列
	Math::Matrix m_rotBulletMat;

	// jsonファイルから読みとった値を格納する変数
	nlohmann::json m_magneBulletData;
};