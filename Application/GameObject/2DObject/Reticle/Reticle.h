#pragma once

class Reticle : public KdGameObject
{
public:
	Reticle(){}
	~Reticle()override{}

	void Init()override;

	void DrawSprite()override;

private:

	// 画像の実体
	std::shared_ptr<KdTexture> m_tex;

	// 画像の切り取り範囲
	Math::Rectangle m_rec = { 0,0,64,64 };
};