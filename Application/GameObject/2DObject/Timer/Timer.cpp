#include"Timer.h"
#include"../../../Manager/ScoreManager/ScoreManager.h"
#include"../../Manager/GoalManager/GoalManager.h"
#include"../../Manager/StartManager/StartManager.h"

void Timer::Init()
{
	// jsonファイルから情報を読み込んでくる
	std::ifstream ifs("Asset/Data/2DObject/Timer.json");
	if (ifs) {
		ifs >> m_timerData;
	}

	// 画像のロード
	m_tex.Load("Asset/Textures/Scene/GameScene/2DObject/Timer/Timer.png");
	m_clockTex.Load("Asset/Textures/Scene/GameScene/2DObject/Timer/clock.png");

	// タイムの初期化
	for (int i = 0; i < k_timeNumberOfDigits; i++)
	{
		m_timer[i] = 0;
	}

	//更新判断フラグ
	m_updateFlg = false;
}

void Timer::Update()
{
	//カウントダウンが終わっていないなら早期リターン
	if (StartManager::instance().GetStartState() == false)return;

	//更新停止フラグがONの時のみ更新する
	if (!m_updateFlg && !(GoalManager::instance().GetClearState()))
	{
		m_timer[0]++;

		// 一定の数まで来たら桁上げをする
		if ((m_timer[0] / k_oneMinute) > k_timeDigitLimit)
		{
			m_timer[0] = 0;
			m_timer[1]++;
		}

		// 一定の数まで来たら桁上げをする
		if (m_timer[1] > k_timeDigitLimit)
		{
			m_timer[1] = 0;
			m_timer[2]++;
		}
	}
	else
	{
		ScoreManager::instance().ConversionScore();
		for (int i = 0; i < k_timeNumberOfDigits; i++)
		{
			if(i==0)
			{
				// クリアタイムの登録
				//ScoreManager::instance().RegisterClearTime(i, m_timer[i]/ k_oneMinute);
			}
			else
			{
				// クリアタイムの登録
				//ScoreManager::instance().RegisterClearTime(i, m_timer[i]);
			}
		}
	}
}

void Timer::DrawSprite()
{
	
	int timerRc = m_timerData["TimerText"].value("RectAngle", 32);
	int posY = m_timerData["TimerText"].value("PosY", 300);
	int width = m_timerData["TimerText"].value("Width", 64);
	int height = m_timerData["TimerText"].value("Height", 64);

	{
		int posX = m_timerData["TimerText"].value("PosX1", 500);
		Math::Rectangle rc = { timerRc * (m_timer[0] / k_oneMinute),0,timerRc,timerRc };
		KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, posX, posY, width, height, &rc);
	}

	{
		int posX = m_timerData["TimerText"].value("PosX2", 468);
		Math::Rectangle rc2 = { timerRc * m_timer[1],0,timerRc,timerRc };
		KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, posX, posY, width, height, &rc2);
	}

	{
		int posX = m_timerData["TimerText"].value("PosX3", 436);
		Math::Rectangle rc3 = { timerRc * m_timer[2],0,timerRc,timerRc };
		KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, posX, posY, width, height, &rc3);
	}

	{
		int rect = m_timerData["ClockText"].value("RectAngle", 640);
		int clockPosX = m_timerData["ClockText"].value("PosX", 370);
		int clockPosY = m_timerData["ClockText"].value("PosY", 290);
		width = m_timerData["ClockText"].value("Width", 90);
		height = m_timerData["ClockText"].value("Height", 90);
		Math::Rectangle rc4 = { 0,0,rect,rect };
		KdShaderManager::Instance().m_spriteShader.DrawTex(&m_clockTex, clockPosX, clockPosY, width, height, &rc4);
	}
}
