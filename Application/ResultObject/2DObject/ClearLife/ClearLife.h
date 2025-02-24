#pragma once
#include"../2DObjectBase.h"

class ClearLife : public _2DObjectBase
{
public:
	ClearLife(){}
	~ClearLife()override{}

	void Init()override;

	void Update()override;

	void DrawSprite()override;

	void DrawImGui()override;

private:

	// ライフの画像関係

	KdTexture m_lifeTex;

	Math::Rectangle m_lifeRc;

	Math::Vector3 m_lifeSize;

	Math::Vector3 m_lifePos;

	Math::Matrix m_lifeMat;
};