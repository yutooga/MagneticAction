#pragma once

class Player;

class SignBoard : public KdGameObject
{
public:
	SignBoard(){}
	~SignBoard()override{}

	enum class TextOption
	{
		GetGun,
		ElectroMagneticInduction,
		MagneGun
	};

	void Init()override;

	void Update()override;

	void PostUpdate()override;

	void DrawUnLit()override;

	void DrawImGui()override;

	void SetPos(const Math::Vector3& _pos) { m_pos = _pos; }

	void SetPlayer(const std::shared_ptr<Player>& _player) { m_wpPlayer = _player; }

	void SetTexture(const TextOption& _option, const float _scale, const int _splitX, const int _splitY);

private:

	// 画像更新
	void TextureUpdate();

	// 画像のアニメーション
	void Animation();

	// プレイヤー感知関数
	void JudgementPlayer();

	// 回転角度
	Math::Vector3  m_angle;

	// 画像の実体
	std::shared_ptr<KdSquarePolygon> m_exclamationMark;

	std::shared_ptr<KdSquarePolygon> m_text;

	// 表示画像切り替えフラグ
	bool m_changeFlg = false;

	// アニメーションの最大コマ
	float m_animationCntMax = 0.0f;

	// アニメーション管理カウント
	float m_animationCnt = 0.f;

	// プレイヤーの実体
	std::weak_ptr<Player> m_wpPlayer;

	// jsonファイルから読みとったデータを格納する変数
	nlohmann::json m_signBoardData;
};