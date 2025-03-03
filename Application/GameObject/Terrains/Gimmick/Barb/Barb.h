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

	void SetRotationAngle(const float _rotZ) { m_rotAngle = _rotZ; }

private:

	// モデルのサイズ
	float m_modelSize = 0.0f;

	// モデルの回転角度
	float m_rotAngle = 0.f;
};