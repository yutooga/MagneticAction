#pragma once

class MaguneBullet : public KdGameObject
{
public:

	MaguneBullet(){}
	~MaguneBullet(){}

	void Init()override;

	void Update()override;

	void DrawLit()override;

	void SetParam(Math::Vector3& _pos, Math::Vector3& _dir, int _lifespan, float _speed);

	//======================================================
	
	static const float k_bulletSize;	// 弾の大きさ

	//======================================================


private:
	std::shared_ptr<KdModelWork> m_model;

	//座標
	Math::Vector3 m_pos;

	//移動方向
	Math::Vector3 m_dir;

	//弾の速さ
	float m_speed = 0.0f;

	//弾の生存時間
	int m_lifeSpan = 0;

	// 弾の回転行列
	Math::Matrix m_rotBulletMat;
};