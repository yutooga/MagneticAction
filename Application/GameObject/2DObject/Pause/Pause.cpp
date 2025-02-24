#include "Pause.h"
#include"../../../Scene/SceneManager.h"
#include"../../Manager/StartManager/StartManager.h"

void Pause::Init()
{
	// 画像のロード
	m_textTex.Load("Asset/Textures/Scene/GameScene/2DObject/Pause/Pause.png");
	m_backTex.Load("Asset/Textures/Scene/GameScene/2DObject/GameFadeOut/Brack.png");
	m_titleTextTex.Load("Asset/Textures/Scene/GameScene/2DObject/Pause/BackText.png");
	m_quitTextTex.Load("Asset/Textures/Scene/GameScene/2DObject/Pause/QuitGame.png");

	Math::Matrix transMat = Math::Matrix::CreateTranslation({0,180,0});
	m_mWorld = transMat;
}

void Pause::PreUpdate()
{
	// 状態の更新
	State previousState = m_nowState;

	// タイトルに戻るをセレクトしている状態にする
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_nowState = State::Title;
	}

	// ゲームを終了するをセレクトしている状態にする
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_nowState = State::Quit;
	}

	//選択している状態が切り替わったらSEを再生する
	if (previousState != m_nowState)
	{
		KdAudioManager::Instance().Play("Asset/Sounds/GameScene/2DObject/Pause/move.wav", false);
	}

	// 何も選択されていない状態
	if (m_nowState == State::NotSelect)
	{
		m_titleTextMat = Math::Matrix::CreateScale(TextureSize["DefaultSize"]);
		Math::Matrix  transMat = Math::Matrix::CreateTranslation({ 0,-150,0 });
		m_quitTextMat = Math::Matrix::CreateScale(TextureSize["DefaultSize"]) * transMat;
	}

	// タイトルに戻るをセレクトしている状態
	else if (m_nowState == State::Title)
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			if (!m_pushEnterFlg)
			{
				//SE再生
				KdAudioManager::Instance().Play("Asset/Sounds/GameScene/2DObject/Pause/click.wav", false);
				//タイトルシーンへ移行する
				SceneManager::Instance().SetNextScene
				(
					SceneManager::SceneType::Title
				);
				StartManager::instance().SetStartState(false);
				//エフェクトの初期化
				KdEffekseerManager::GetInstance().Release();
				m_pushEnterFlg = true;
			}
		}
		else
		{
			m_pushEnterFlg = false;
		}

		m_titleTextMat = Math::Matrix::CreateScale(TextureSize["BigSize"]);
		Math::Matrix  transMat = Math::Matrix::CreateTranslation({ 0,-150,0 });
		m_quitTextMat = Math::Matrix::CreateScale(TextureSize["DefaultSize"]) * transMat;
	}

	// ゲームを終了するをセレクトしている状態
	else if (m_nowState == State::Quit)
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			if (!m_pushEnterFlg)
			{
				//SE再生
				KdAudioManager::Instance().Play("Asset/Sounds/GameScene/2DObject/Pause/click.wav", false);
				std::exit(1);
				m_pushEnterFlg = true;
			}
		}
		else
		{
			m_pushEnterFlg = false;
		}

		m_titleTextMat = Math::Matrix::CreateScale(TextureSize["DefaultSize"]);
		Math::Matrix  transMat = Math::Matrix::CreateTranslation({ 0,-150,0 });
		m_quitTextMat = Math::Matrix::CreateScale(TextureSize["BigSize"]) * transMat;
	}

	// Pキーを押した場合ゲームに戻る
	if (GetAsyncKeyState('P') & 0x8000)
	{
		if (!m_pushPFlg)
		{
			//SE再生
			KdAudioManager::Instance().Play("Asset/Sounds/GameScene/2DObject/Pause/click.wav", false);
			m_isExpired = true;
		}
	}
	else
	{
		m_pushPFlg = false;
	}
}

void Pause::DrawSprite()
{
	if (m_quitTextMat == Math::Matrix::Identity)return;
	else if (m_isExpired)return;

	// 背景
	Math::Color color = { 0,0,0,0.5f };
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_backTex, 0, 0, 0, &color);

	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
	Math::Rectangle rc = { 0,0,636,218 };
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_textTex, 0, 0,&rc);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);

	// タイトルロゴ
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_titleTextMat);
	rc = { 0,0,274,119 };
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_titleTextTex, 0, 0,&rc);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);

	// やめるロゴ
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_quitTextMat);
	rc = { 0,0,524,142 };
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_quitTextTex, 0, 0, &rc);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}

void Pause::Release()
{
	// 画像の開放
	m_backTex.Release();
	m_textTex.Release();
	m_titleTextTex.Release();
	m_quitTextTex.Release();
}
