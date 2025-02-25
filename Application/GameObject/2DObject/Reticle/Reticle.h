#pragma once
#include"../2DGameObjBase.h"

class Reticle : public _2DGameObjBase
{
public:
	Reticle(){}
	~Reticle()override{}

	void Init()override;

	void DrawSprite()override;

private:
};