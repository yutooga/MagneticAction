#pragma once
#include"../2DObjectBase.h"

class ResultText : public _2DObjectBase
{
public:
	ResultText(){}
	~ResultText()override{}

	void Init()override;

	void Update()override;

	void DrawSprite()override;

	void DrawImGui()override;

	static const float k_inPlace;	// 所定の位置

private:

	// 次のシーンに誘導するテキスト関係
	KdTexture m_clickTex;

	Math::Matrix m_clickMat;

	Math::Vector3 m_clickPos;

	Math::Rectangle m_clickRc;

	float m_clickTextSize = 0.f;
};