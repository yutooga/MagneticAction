#pragma once
#include"../2DObjectBase.h"

class Rank :public _2DObjectBase
{
public:
	Rank(){}
	~Rank()override{}

	void Init()override;

	void Update()override;

	void DrawImGui()override;

	//================================================================
	static const float k_rotAngleMax;	// 回転する角度の限界

	static const float k_alphaMax;	// 透明度の限界

	static const float k_addAngleAmount;	// 角度の加算量

	static const float k_addAlphaAmount;	// 透明度の加算量

	static const float k_rgbMax;	// rgb値の限界値
	//================================================================

private:

	enum class RANK
	{
		S,
		A,
		B,
		C,
		D
	};

	// 透明度の加算処理
	void AddAlpha();

	// 画像の回転処理
	void RotationTexture();

	// 画像の回転角度
	float m_angle = 0.0f;
};