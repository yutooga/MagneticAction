#pragma once
#include"../../TerrainsBase.h"

class MagnaticArea : public TerrainsBase
{
public:
	MagnaticArea(){}
	~MagnaticArea()override{}

	void Init()override;

	void Update()override;

	void DrawImGui()override;

	// オブジェクトタイプ取得関数
	KdGameObject::ObjectType GetObjType()override { return m_ObjType; }

	// モデルのサイズ設定関数
	void SetModelSize(const float _size = 1.0f) { m_modelSize = _size; }

	// 回転角度設定関数
	void SetAngle(const float _angle = 0.0f) { m_angle = _angle; }

private:

	// モデルのサイズ
	float m_modelSize = 0.0f;

	// 回転角度
	float m_angle = 0.0f;
};