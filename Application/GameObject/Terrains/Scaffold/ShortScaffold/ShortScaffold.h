#pragma once
#include"../../TerrainsBase.h"

class ShortScaffold : public TerrainsBase
{
public:
	ShortScaffold(){}
	~ShortScaffold(){}

	void Init()override;

	void Update()override;

	void DrawImGui()override;

	void DrawBright()override{}

	//モデルの大きさ変更関数
	void SetModelSize(const float _size) { m_modelSize = _size; }

	ObjectType GetObjType()override{ return ObjectType::ShortScaffold; }

private:

	// モデルの大きさ
	float m_modelSize = 0.0f;
};