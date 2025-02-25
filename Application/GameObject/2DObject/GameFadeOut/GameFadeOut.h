#pragma once
#include"../2DGameObjBase.h"

class GameFadeOut : public _2DGameObjBase
{
public:
	GameFadeOut(){}
	~GameFadeOut(){}

	void Init()override;

	void Update()override;

	void DrawSprite()override;

	static const float k_reduceAlphaAmount;	// 画像の透明度の減算量

private:
};