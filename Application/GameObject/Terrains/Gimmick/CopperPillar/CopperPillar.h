#pragma once
#include"../../TerrainsBase.h"

class CopperPillar :public TerrainsBase
{
public:

	void Init()override;

	void Update()override;

	void DrawImGui()override;

	void DrawBright()override {}

	void SetModelSize(const float _size) { m_modelSize = _size; }

private:

	// モデルの表示サイズ
	float m_modelSize = 1.0f;
};