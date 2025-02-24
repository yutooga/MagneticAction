#pragma once

class Magnet : public KdGameObject
{
public:
	Magnet(){}
	~Magnet()override {}

	void Init()override;

	void Update()override;

	void DrawLit()override;

	void GenerateDepthMapFromLight()override;

	static const float k_addAmount;	// 角度の加算量

private:

	// モデルの実体
	std::shared_ptr<KdModelWork> m_model;

	//回転角度
	float m_angle = 0.0f;
};