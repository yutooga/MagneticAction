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

	ObjectType GetObjType()override{ return ObjectType::MagneBox; }

	static const float k_changeMoment;	// 磁力の切り替わるタイミング

private:

	// 纏っている磁極を切り替える処理
	void ChangeForce();

	// 表示色の更新
	void BrightColorUpdate();

	// モデルのサイズ
	float m_modelSize = 0.f;

	// 回転角度
	float m_angle = 0.0f;

	// 磁力の切り替えカウント
	float m_changeCnt = 0.f;

	// クールタイムのカウント
	int m_coolTimeCnt = 0;

	// 表示色
	Math::Color m_color;
};