#pragma once

class Pause : public KdGameObject
{
public:
	Pause(){}
	~Pause() { Release(); }

	// 選択状態
	enum class State
	{
		NotSelect = 1,	// 何も選択されていない状態
		Title = 1 << 1,	// タイトルを選択している状態
		Quit = 1 << 2	// やめるを選択している状態
	};

	std::unordered_map<std::string, float> TextureSize = {

		{"DefaultSize",1.0f},
		{"BigSize",1.5f}
	};

	void Init()override;

	void PreUpdate()override;

	void DrawSprite()override;

private:

	void Release();

	//テキスト画像
	KdTexture m_textTex;

	//背景画像
	KdTexture m_backTex;

	//「タイトル画面に戻る」項目の画像
	KdTexture m_titleTextTex;
	Math::Matrix m_titleTextMat;

	//「ゲームを中断」項目の画像
	KdTexture m_quitTextTex;
	Math::Matrix m_quitTextMat;

	//今の状態を管理する変数
	State m_nowState = State::NotSelect;

	// 押しっぱ防止フラグ
	bool m_pushEnterFlg = false;

	bool m_pushPFlg = true;
};