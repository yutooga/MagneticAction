#pragma once
#include "../3DObjBase.h"

class TitleBack : public _3DObjectBase
{
public:
	TitleBack(){}
	~TitleBack()override{}

	void Init()override;

	void DrawUnLit()override;

private:
};