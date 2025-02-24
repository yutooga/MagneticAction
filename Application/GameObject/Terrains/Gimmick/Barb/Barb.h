#pragma once
#include"../../TerrainsBase.h"

class Barb : public TerrainsBase
{
public:
	Barb() {}
	~Barb()override {}

	void Init()override;

	void Update()override;

	void DrawImGui()override;

	void DrawBright()override {}

private:


	float m_modelSize = 8.0f;
};