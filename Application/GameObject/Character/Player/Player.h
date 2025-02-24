#pragma once

class CameraBase;
class HP;
class GameOver;

class Player :public KdGameObject
{
public:
	Player() {}
	~Player()override {}

	void Update()override;

	void PostUpdate()override;

	void Init()override;

	//オブジェクトに当たられたときの処理
	void OnHit(ObjectType _obj)override;

	void DrawImGui()override;

	void DrawUnLit()override;

	void ChangeScene();

	// hpをセットする関数
	void SetHp(const std::shared_ptr<HP>& _hp) { m_hp = _hp; }

	void SetGravity(float _gravity) { m_gravity = _gravity; }

	ObjectType GetObjType()override { return ObjectType::Player; }

	// ゲームオーバー状態取得
	const bool GetGameOver()const { return m_gameoverFlg; }

	const bool GetRideOn() const{ return m_rideOnFlg; }

	const Math::Vector3 GetRotateAngle()const
	{
		return m_degAng;
	}

	//================================================

	static const float k_deathPlayerHeight;	// ここまで落ちたらプレイヤーは死ぬ

	static const float k_shadowRotAngle; // 影の回転角度

	static const float k_unitVector;	// 単位ベクトル

	static const float k_colisionRadius;	// 当たり判定の半径

	//================================================
	
	enum class ColisionCircle
	{
		Top,
		Center,
		Under
	};

	//================================================

private:
	void UpdateRotateByMouse();

	const Math::Matrix GetRotationMatrix()const
	{
		return Math::Matrix::CreateFromYawPitchRoll(
			DirectX::XMConvertToRadians(m_degAng.y),
			DirectX::XMConvertToRadians(m_degAng.x),
			DirectX::XMConvertToRadians(m_degAng.z));
	}

	const Math::Matrix GetRotationYMatrix() const
	{
		return Math::Matrix::CreateRotationY(
			DirectX::XMConvertToRadians(m_degAng.y));
	}

	//================================================================================

	//プレイヤーと地面の処理
	void Player_TerrainColision();

	//プレイヤーと地面の処理
	void Player_TerrainSphereColision();

	// 磁力エリアにいるかどうか判断する関数
	void OnTheArea();

	// 影描画判断
	void JudgmentShadow();
	
	//ゲームオーバー処理
	void GameOverPlayer();

	//デバック処理関数
	void Debug();

	void OnHit(ObjectType _objType, const std::weak_ptr<KdGameObject>& _obj);

	//SE更新
	void SeUpdate();

	//重力更新
	void GravityUpdate();

	// プレイヤーの動く速度を下げる関数
	void DownMoveSpeed(const std::weak_ptr<KdGameObject>& _obj);

	//================================================================================

	//進行方向（ベクトルの向き）
	Math::Vector3 m_moveDir;

	// 回転用角度
	Math::Vector3 m_degAng;

	// ジャンプのキー制御フラグ
	bool m_jumpKeyFlg = false;

	//移動速度（ベクトルの大きさ）
	float m_moveSpeed = 0.6f;

	//重力
	float m_gravity = 0.0f;

	// カメラ回転用マウス座標の差分
	POINT m_FixMousePos = { 640,360 };

	//磁力をまとう床の反発力
	float m_repulsionPow = 0.0f;
	bool m_repulsionFlg = false;
	Math::Vector3 m_repulsionDir;

	//磁力まとう床の引き寄せる力
	bool m_attractionFlg = false;

	//HPの実体
	std::weak_ptr<HP> m_hp;

	//ジャンプ台に乗っているかフラグ
	bool m_getOnJumpFlg = false;

	//プレイヤーの復帰位置
	Math::Vector3 m_returnPos = { 0,18,0};

	//ゲームオーバー関連
	std::weak_ptr<GameOver> m_gameover;
	bool m_gameoverFlg = false;

	//SE関係
	int m_seInterval = 0;
	bool m_seFlg = false;

	// クリアしたかどうか判断するフラグ
	bool m_clearFlg = false;

	// 特定のオブジェクトに乗っているかどうか判断するフラグ
	bool m_onTheAreaFlg = false;

	// 動くときの回転角度
	float m_addAmount = 0.0f;

	// 補正値
	float m_adjustValue = 7.8f;

	// プレイヤーの移動スピード低下フラグ
	bool m_moveDownFlg = false;

	// 影の描画位置
	Math::Vector3 m_shadowPos;

	// 影のサイズ
	float m_shadowSize = 13.0f;

	// 影の行列
	Math::Matrix m_shadowMat;

	// 影の画像
	std::shared_ptr<KdSquarePolygon> m_polygon;

	// 影の描画フラグ
	bool m_shadowDrawFlg = false;

	// プレイヤーの色々なパラメーターをファイルから取得する変数
	nlohmann::json m_jsonData;

	// debug用の座標を読み込んで保存する変数
	nlohmann::json m_debugData;

	bool m_rideOnFlg = false;
};
