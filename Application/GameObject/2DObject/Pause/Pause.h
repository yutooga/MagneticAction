#pragma once
#include"../2DGameObjBase.h"

class Pause : public _2DGameObjBase
{
public:
	Pause(){}
	~Pause()override{ Release(); }

	// 選択状態
	enum class State
	{
		NotSelect,	// 何も選択されていない状態
		Title,	// タイトルを選択している状態
		Quit 	// やめるを選択している状態
	};

	std::unordered_map<std::string, float> TextureSize = {

		{"DefaultSize",1.0f},
		{"BigSize",1.5f}
	};

	void Init()override;

	void PreUpdate()override;

	void DrawSprite()override;

private:

	void Release()override;

	// 選択している項目を切り替える関数
	void ChangeSelectState();

	//背景画像
	KdTexture m_backTex;

	//「タイトル画面に戻る」項目の画像
	KdTexture m_titleTextTex;
	Math::Matrix m_titleTextMat;

	//「ゲームを中断」項目の画像
	KdTexture m_quitTextTex;
	Math::Matrix m_quitTextMat;

	// 画像の切り取り範囲
	Math::Rectangle m_titleRc;
	Math::Rectangle m_quitRc;

	//今の状態を管理する変数
	State m_nowState = State::NotSelect;

	// 前の状態を管理する変数
	State m_previousState = State::NotSelect;

	// 押しっぱ防止フラグ
	bool m_pushEnterFlg = false;

	bool m_pushPKeyFlg = true;
};