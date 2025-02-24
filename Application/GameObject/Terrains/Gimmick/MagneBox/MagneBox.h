#pragma once
#include"../../../Terrains/TerrainsBase.h"

class MagneBox : public TerrainsBase
{
public:
	MagneBox(){}
	~MagneBox()override{}

	void Init()override;

	void Update()override;

	void DrawBright()override;

	void DrawImGui()override;

	ObjectType GetObjType() { return ObjectType::MagneBox; }

	static const float k_changeMoment;	// 磁力の切り替わるタイミング

	static const int k_coolTime = 500;	// クールタイムの長さ

private:

	// モデルのサイズ
	float m_modelSize = 13.4f;

	// 回転角度
	float m_angle = 180.0f;

	// 磁力の切り替えカウント
	float m_changeCnt = 0.f;

	// クールタイムのカウント
	int m_coolTimeCnt = 0;
};