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

	void SetModelSize(const float _size) { m_modelSize = _size; }

private:

	// モデルのサイズ
	float m_modelSize = 0.0f;
};