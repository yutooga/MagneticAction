#pragma once

class Timer :public KdGameObject
{
public:
	Timer() {}
	~Timer() override{}

	void Init()override;

	void Update()override;

	void DrawSprite()override;

	//更新状態切り替え関数
	void ChangeUpdate(const bool _update) { m_updateFlg = _update; }

	static const int k_timeNumberOfDigits = 3;	// タイムの桁数
	static const int k_timeDigitLimit = 9;	// １桁の限界値
	static const int k_oneMinute = 60;	// 1分は60秒
	static const int k_hundredthPlace = 100;	// 100の位
	static const int k_tenPlace = 10;	// 10の位

private:

	// タイム記録用変数
	int m_timer[3] = {};

	//画像データ
	KdTexture m_tex;

	//更新判断フラグ
	bool m_updateFlg = false;

	// 時計の画像
	KdTexture m_clockTex;

	// jsonファイル空の読み込んだ値を格納する変数
	nlohmann::json m_timerData;
};