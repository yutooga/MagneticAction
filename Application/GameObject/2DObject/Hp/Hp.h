#pragma once
#include"../2DGameObjBase.h"

class HP : public _2DGameObjBase
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

	//==============================================================

private:

	// ハートが揺れる処理
	void ShakeHeart();

	//現在のHP数
	int m_nowHp = 0;

	// ダメージを受けたかどうか判断するフラグ
	bool m_damageFlg = false;

	// ハートが揺れる時間
	float m_time = 0.0f;

	// 画像の大きさ
	Math::Vector3 m_scale;
};