#pragma once

class _2DTitleObjBase : public KdGameObject
{
public:
	_2DTitleObjBase();
	~_2DTitleObjBase()override{}

protected:

	// 画像の実体
	KdTexture m_tex;

	// 画像の表示範囲
	Math::Rectangle m_rectAngle;

	// 画像の表示位置
	Math::Vector3 m_pos;

	// 透明度
	float m_alpha = 0.0f;

	// jsonファイルのデータ
	static nlohmann::json m_jsonData;
};