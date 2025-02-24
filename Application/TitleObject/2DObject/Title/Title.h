#pragma once

class Title : public KdGameObject
{
public:
	Title(){}
	~Title()override{}

	void Init()override;

	void DrawSprite()override;

private:

	// 画像の実体
	KdTexture m_tex;

	Math::Rectangle m_rectAngle = { 0,0,1028,114 };
};