#pragma once
#include "../2DTitleObjBase.h"

class Title : public _2DTitleObjBase
{
public:
	Title(){}
	~Title()override{}

	void Init()override;

	void DrawSprite()override;

private:

};