#pragma once

class Player;
class MagneGun;

class FallingMagneGun : public KdGameObject
{
public:
	FallingMagneGun(){}
	~FallingMagneGun()override{}

	void Init()override;

	void Update()override;

	void PostUpdate()override;

	void DrawLit()override;

	void DrawImGui()override;

	void SetPlayer(const std::shared_ptr<Player>& _obj) { m_player = _obj; }

	void SetMagneGun(const std::shared_ptr<MagneGun>& _obj) { m_gun = _obj; }

	// ============================定数関係==================
		
	static const float k_upDownSpeed;	// オブジェクトの浮遊スピード

	static const float k_rotSpeed;	// オブジェクトの回転スピード

	static const float k_addAnimeCnt;	// アニメーションの加算値

	static const float k_animationMax;	// アニメーションの限界値

	static const float k_adjustValue;	// 座標の補正値
		
	// ======================================================

private:

	//プレイヤーが銃を取得できるエリアにいるかどうか判定する
	void JudgmentAcquisition();

	//テキストの更新
	void TextureUpdate();

	// 銃取得関数
	void GetMagneGun();

	// モデルの実態
	std::shared_ptr<KdModelWork> m_model;

	//オブジェクトの大きさ
	float m_modelSize = 0.08f;

	//オブジェクトの回転角度
	float m_angle = 0.0f;

	//取得可能表示テキストの実体
	std::shared_ptr<KdSquarePolygon> m_polygon;

	//テキストのアニメーションカウント
	float m_animeCnt = 0.0f;

	//テキストの行列
	Math::Matrix m_textureMat = Math::Matrix::Identity;

	//取得可能状態判断フラグ
	bool m_acquisitionFlg = false;

	//プレイヤーの実体
	std::weak_ptr<Player> m_player;

	//銃の実体
	std::weak_ptr<MagneGun> m_gun;

	// オブジェクトの最初の座標
	Math::Vector3 m_firstPos;

	// エフェクトの名前
	std::string m_effectName;

	// jsonファイルから読みとった値を格納する変数
	nlohmann::json m_magneGunData;
};