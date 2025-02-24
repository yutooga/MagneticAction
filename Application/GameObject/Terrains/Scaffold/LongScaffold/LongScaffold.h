#pragma once
#include"../../TerrainsBase.h"

class LongScaffold : public TerrainsBase
{
public:
	LongScaffold() {}
	~LongScaffold() {}

	void Init()override;

	void Update()override;

	void DrawImGui()override;

	void GenerateDepthMapFromLight();

	void DrawBright()override {}

	//　床の幅を決める関数
	void SetWidth(const float _size) { m_adjustWidth = _size; }

	// 回転角度を設定する関数
	void SetRotAngle(const float _angle = k_defaultRotAngle) { m_rotAngle = _angle; }

	// オブジェクトタイプ取得関数
	ObjectType GetObjType()override { return ObjectType::LongScaffold; }

	static const float k_modelSize;	// モデルのサイズ

	static const float k_defaultRotAngle;	// デフォルトの回転角度

private:

	// 補正値
	float m_adjustWidth = 90.0f;

	// 回転角度
	float m_rotAngle = 90.0f;
};