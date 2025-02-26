#include "Pause.h"
#include"../../../Scene/SceneManager.h"
#include"../../Manager/StartManager/StartManager.h"

void Pause::Init()
{
	// 画像のロード
	m_tex.Load(m_jsonData["Pause"]["Texture"]["URL"]);
	m_backTex.Load(m_jsonData["Pause"]["Back"]["URL"]);
	m_titleTextTex.Load(m_jsonData["Pause"]["Title"]["URL"]);
	m_quitTextTex.Load(m_jsonData["Pause"]["Quit"]["URL"]);

	// 表示色の初期化
	m_color = { 0,0,0,m_jsonData["Pause"]["Back"].value("Alpha",0.5f)};

	// 表示位置の初期化
	m_pos = {
		m_jsonData["Pause"]["Texture"]["Pos"].value("X",0.f),
		m_jsonData["Pause"]["Texture"]["Pos"].value("Y",180.f),
		0
	};

	// 切り取り範囲の初期化
	m_rc = {
		0,
		0,
		m_jsonData["Pause"]["Texture"]["Rc"].value("X",636),
		m_jsonData["Pause"]["Texture"]["Rc"].value("Y",218)
	};

	m_quitRc = {
		0,
		0,
		m_jsonData["Pause"]["Quit"]["Rc"].value("X",524),
		m_jsonData["Pause"]["Quit"]["Rc"].value("Y",142)
	};

	m_titleRc = {
		0,
		0,
		m_jsonData["Pause"]["Title"]["Rc"].value("X",274),
		m_jsonData["Pause"]["Title"]["Rc"].value("Y",119)
	};

	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = transMat;
}

void Pause::PreUpdate()
{
	// 状態の更新
	m_previousState = m_nowState;

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
	if (m_previousState != m_nowState)
	{
		KdAudioManager::Instance().Play(m_jsonData["Pause"]["Se"]["Move"]["URL"], false);
	}

	// 選択している項目を切り替える関数
	ChangeSelectState();

	// Pキーを押した場合ゲームに戻る
	if (GetAsyncKeyState('P') & 0x8000)
	{
		if (!m_pushPKeyFlg)
		{
			//SE再生
			KdAudioManager::Instance().Play(m_jsonData["Pause"]["Se"]["Click"]["URL"], false);
			m_isExpired = true;
		}
	}
	else
	{
		m_pushPKeyFlg = false;
	}
}

void Pause::DrawSprite()
{

	// 画像が読み込まれていない場合は描画しない
	// 実態が消える直前は描画しない
	if (m_quitTextMat == Math::Matrix::Identity)return;
	else if (m_isExpired)return;

	// 背景
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_backTex, 0, 0, 0, &m_color);

	// ポーズロゴ
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0,&m_rc);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);

	// タイトルロゴ
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_titleTextMat);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_titleTextTex, 0, 0,&m_titleRc);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);

	// やめるロゴ
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_quitTextMat);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_quitTextTex, 0, 0, &m_quitRc);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}

void Pause::Release()
{
	// 画像の開放
	m_backTex.Release();
	m_titleTextTex.Release();
	m_quitTextTex.Release();

	_2DGameObjBase::Release();
}

void Pause::ChangeSelectState()
{
	switch (m_nowState)
	{
	case Pause::State::NotSelect:	// 何も選択されていない状態
	{
		m_titleTextMat = Math::Matrix::CreateScale(TextureSize["DefaultSize"]);
		Math::Matrix  transMat = Math::Matrix::CreateTranslation({
			m_jsonData["Pause"]["Quit"]["Pos"].value("X",0.f),
			m_jsonData["Pause"]["Quit"]["Pos"].value("Y",-150.f),0 });;
		m_quitTextMat = Math::Matrix::CreateScale(TextureSize["DefaultSize"]) * transMat;
		break;
	}
	case Pause::State::Title:	// タイトルに戻るをセレクトしている状態
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			if (!m_pushEnterFlg)
			{
				//SE再生
				KdAudioManager::Instance().Play(m_jsonData["Pause"]["Se"]["Click"]["URL"], false);
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
		Math::Matrix  transMat = Math::Matrix::CreateTranslation({
			m_jsonData["Pause"]["Quit"]["Pos"].value("X",0.f),
			m_jsonData["Pause"]["Quit"]["Pos"].value("Y",-150.f),0 });
		m_quitTextMat = Math::Matrix::CreateScale(TextureSize["DefaultSize"]) * transMat;
		break;
	}
	case Pause::State::Quit:	// ゲームを終了するをセレクトしている状態
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			if (!m_pushEnterFlg)
			{
				//SE再生
				KdAudioManager::Instance().Play(m_jsonData["Pause"]["Se"]["Click"]["URL"], false);
				std::exit(1);
				m_pushEnterFlg = true;
			}
		}
		else
		{
			m_pushEnterFlg = false;
		}

		m_titleTextMat = Math::Matrix::CreateScale(TextureSize["DefaultSize"]);
		Math::Matrix  transMat = Math::Matrix::CreateTranslation({
			m_jsonData["Pause"]["Quit"]["Pos"].value("X",0.f),
			m_jsonData["Pause"]["Quit"]["Pos"].value("Y",-150.f),0 });
		m_quitTextMat = Math::Matrix::CreateScale(TextureSize["BigSize"]) * transMat;
		break;
	}
	default:
		break;
	}
}
