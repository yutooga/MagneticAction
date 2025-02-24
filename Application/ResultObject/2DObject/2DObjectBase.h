#pragma once

class _2DObjectBase : public KdGameObject
{
public:
	_2DObjectBase();
	~_2DObjectBase()override{}

	void DrawSprite()override;

protected:

	//画像データ
	KdTexture m_tex;
	//テクスチャの表示位置
	Math::Vector3 m_pos;
	//テクスチャの色
	Math::Color m_color;
	//切り取り範囲
	Math::Rectangle m_rc;
	//透明度
	float m_alpha = 1.0f;
	//画像サイズ
	float m_textureSize = 0.f;
	// IMGUI用の変数
	int m_randomId = 0;
	// jsonデータを保存する用の変数
	static nlohmann::json m_jsonData;
};