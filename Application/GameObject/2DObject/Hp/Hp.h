#pragma once

class HP : public KdGameObject
{
public:
	HP(){}
	~HP()override {}

	void Init()override;

	void Update()override;

	void DrawSprite()override;

	// HPを減らす関数
	void DecreaseHp();

	//現在のHP取得関数
	const int GetHpNum()const { return m_nowHp; }

	//=======================定数関係===============================
	
	static const float k_addTimeAmount;	// 時間の加算量
	static const float k_tremorSpeed;	// 揺れの速さ
	static const float k_tremorRange;	// 揺れ幅
	static const float k_tremorLimit;	// 揺れの限界値
	const Math::Vector3 k_firstPos = { -400, 300, 0 };	// 最初の座標
	const Math::Vector3 k_scale = { 6.0f,1.0f,1.0f };	// 最初の座標

	//==============================================================

private:

	// 画像の実体
	KdTexture m_tex;

	//現在のHP数
	int m_nowHp = 6;

	// ダメージを受けたかどうか判断するフラグ
	bool m_damageFlg = false;

	// 画像の座標
	Math::Vector3 m_pos;

	// ハートが揺れる時間
	float m_time = 0.0f;
};