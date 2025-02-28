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

	static const float k_colisionAdjustValueY;	// 当たり判定のY座標の補正値

private:

	// セーブポイントとプレイヤーとの当たり判定
	void SavePoint_Player();

	// モデルのサイズ
	float m_modelSize = 0.f;

	//プレイヤーがエリア内に入っているか判断フラグ
	bool m_rangeFlg = false;

	//SE再生フラグ
	bool m_seFlg = false;

	// SE再生のクールタイム
	int m_seInterval = 0;
};