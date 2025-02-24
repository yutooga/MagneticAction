#pragma once
#include"../BaseScene/BaseScene.h"

class ResultScene : public BaseScene
{
public:
	ResultScene() { Init(); }
	~ResultScene()override{}

	void DrawImGui()override;

private:

	void Init()override;
	void Event()override;
};