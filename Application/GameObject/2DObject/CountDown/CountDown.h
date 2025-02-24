#pragma once

class CountDown : public KdGameObject
{
public:
	CountDown(){}
	~CountDown(){}

	void Init()override;

	void Update()override;

	void DrawSprite()override;

	// =====================定数関係========================================

	static const int k_changeCoolTime = 48;	// 画像が切り替わるまでのクールタイム
	static const float k_reduceAmount;	// 減算量
	static const int k_textMaxAmount = 3;	// 画像の最大枚数
	static const float k_textNormalSize;	// 画像の通常時のサイズ

	// =====================================================================

private:

	// 画像の実体
	std::vector<std::shared_ptr<KdTexture>> m_number;

	// 画像の切り取り範囲
	std::vector<Math::Rectangle> m_rect;

	// 回転用角度
	float m_angle = 0.0f;

	// 拡縮率
	float m_size = 1.0f;

	// 現在のテクスチャカウント
	int m_nowTextureCnt = 0;

	// 切り替わりのタイミングを計るカウント
	int m_changeCnt = 0;

	// 画像の更新が終わったかどうか判断するフラグ
	bool m_endFlg = false;

	// SE再生判断フラグ
	bool m_seFlg = false;
};