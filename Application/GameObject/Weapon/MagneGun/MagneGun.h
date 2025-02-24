#pragma once

class FPSCamera;
class Player;

class MagneGun : public KdGameObject
{
public:
	void Init()override;

	void Update()override;

	void DrawLit()override;

	void SetPos(Math::Vector3& _pos) { m_pos = _pos; }

	// 親をセットする関数
	void SetParent(const std::shared_ptr<Player>& _parent) { m_parent = _parent; }

	static const float k_grantScope;	// 銃の磁力を付与できる限界

private:

	void Shot(const std::shared_ptr<const Player>& _parent, const UINT& _maguneForce);

	// モデルの実体
	std::shared_ptr<KdModelData> m_model;

	// 座標
	Math::Vector3 m_pos;

	//親の実体
	std::weak_ptr<Player> m_parent;

	//親から銃口へのローカル行列
	Math::Matrix m_localMuzzleMat;

	// 弾を発射したかどうか判断するフラグ
	bool m_shotFlg = false;

	// jsonファイルから読みとった値を格納する変数
	nlohmann::json m_magneGunData;
};