#pragma once
#include"../../TerrainsBase.h"

class SavePoint : public TerrainsBase
{
public:
	SavePoint(){}
	~SavePoint()override {}

	void Init()override;

	void Update()override;

	void DrawBright()override;

	void DrawImGui()override;

	static const float k_seCoolTime;	// SE再生のクールタイム

	static const float k_colisionRadius;	// 当たり判定の球の半径

	static const float k_colisionAdjustValueY;	// 当たり判定のY座標の補正値

private:

	// セーブポイントとプレイヤーとの当たり判定
	void SavePoint_Player();

	// SE更新関数
	void CountDownSE();

	// モデルのサイズ
	float m_modelSize = 28.3f;

	//プレイヤーがエリア内に入っているか判断フラグ
	bool m_rangeFlg = false;

	//SE再生フラグ
	bool m_seFlg = false;

	// SE再生のクールタイム
	int m_seInterval = 0;
};