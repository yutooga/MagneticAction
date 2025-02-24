#pragma once

class MagunePower : public KdGameObject
{
public:
	MagunePower(){}
	~MagunePower()override{}

	void Init()override;

	void Update()override;

	void PostUpdate()override;

	void DrawSprite()override;

	void DrawImGui()override;

	// プレイヤーの実体をセット
	void SetPlayer(const std::shared_ptr<KdGameObject>& _player) { m_wpPlayer = _player; }

	static const float k_rcMax; // 更新カウントの限界
	static const float k_rotAngleMax;	// 回転角度の限界
	static const float k_addEffectCnt;	// エフェクトの更新カウント加算量

private:

	enum class RotationDirection
	{
		Rightturn = 1,
		Leftturn = 1 << 1,
		Noturn = 1 << 2,
		Finish = 1 << 3
	};

	// 画像の切り取り範囲更新関数
	void EffectRcUpdate();

	// テキストの回転更新関数
	void TextRotUpdate();

	// テキストの更新
	void TextUpdate();

	// エフェクトの画像
	KdTexture m_effectTex;

	// 自分がまとっている磁極の画像
	KdTexture m_nPowerTex;
	KdTexture m_sPowerTex;
	KdTexture m_noPowerTex;

	// プレイヤーの実体
	std::weak_ptr<KdGameObject> m_wpPlayer;

	// プレイヤーの現在まとっている磁極
	UINT m_nowPower = NoForce;

	// 画像の座標
	Math::Vector3 m_texturePos = Math::Vector3::Zero;

	// 画像のサイズ
	float m_powerTexSize = -0.25f;
	float m_effectTexSize = 0.6f;

	// エフェクト画像の切り取り範囲
	Math::Rectangle m_effcRc;

	// エフェクト画像の切り取り範囲更新カウント
	float m_rcCnt = 0.0f;

	// エフェクト更新フラグ
	bool m_effectFlg = false;

	// テキスト更新フラグ
	bool m_textFlg = false;

	// テキスト回転更新フラグ
	RotationDirection m_textRotFlg = RotationDirection::Noturn;

	// テキストの回転角度
	int m_angle = 0;

	// 回転速度
	int RotSpeed = 9;

	//行列
	Math::Matrix m_powerMat = Math::Matrix::Identity;
	Math::Matrix m_effectMat = Math::Matrix::Identity;
};