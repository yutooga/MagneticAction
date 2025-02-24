#pragma once

#include"../../TerrainsBase.h"

class GateWall : public TerrainsBase
{
public:
	GateWall(){}
	~GateWall(){}

	void Init()override;

	void Update()override;

	void DrawImGui()override;

	void DrawBright()override{}

	ObjectType GetObjType()override { return ObjectType::Gate; }

private:

	// モデルのサイズ
	float m_modelSize = 16.5f;

};
