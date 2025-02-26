#pragma once

class _2DGameObjBase : public KdGameObject
{
public:
	_2DGameObjBase();
	~_2DGameObjBase()override { Release(); }
protected:

	virtual void Release() { m_tex.Release(); }

	// 画像の実体
	KdTexture m_tex;

	// 表示座標
	Math::Vector3 m_pos;

	// 表示色
	Math::Color m_color;

	// 切り取り範囲
	Math::Rectangle m_rc;

	// 透明度
	float m_alpha = 0.0f;

	// 回転角度
	float m_angle = 0.0f;

	// 拡縮率
	float m_size = 1.0f;

	// jsonファイルから読み込んだデータを格納する変数
	static nlohmann::json m_jsonData;	
};