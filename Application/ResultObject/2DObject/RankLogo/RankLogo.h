#pragma once
#include"../2DObjectBase.h"

class RankLogo : public _2DObjectBase
{
public:
	RankLogo(){}
	~RankLogo()override{}

	void Init()override;

	void Update()override;

	void DrawImGui()override;
private:

	// ロゴの表示サイズ
	float m_logoSize = 0.0f;

	// ロゴの表示座標
	Math::Vector2 m_logoPos;

};
