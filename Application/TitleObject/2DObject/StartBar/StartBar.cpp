#include "StartBar.h"

const float StartBar::k_alphaMax = 1.f;
const float StartBar::k_alphaMin = 0.3f;

void StartBar::Init()
{
	// 画像のロード
	m_tex.Load(m_jsonData["StartBar"]["URL"]);

	// 画像の表示座標
	m_pos = { m_jsonData["StartBar"]["Pos"].value("X",0.f),
		m_jsonData["StartBar"]["Pos"].value("Y",-260.f),m_jsonData["StartBar"]["Pos"].value("Z",0.f)
	};

	// 加算量の初期化
	m_alphaAdd = m_jsonData["StartBar"].value("AlphaAdd", 0.01f);

	//画像の表示範囲
	m_rectAngle = { 0,0,m_jsonData["StartBar"].value("RcX",892),m_jsonData["StartBar"].value("RcY",119) };

	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_jsonData["StartBar"].value("TextureSize", 0.75f));
	m_mWorld = scaleMat * transMat;
}

void StartBar::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
	Math::Color color = { k_alphaMax,k_alphaMax,k_alphaMax,m_alpha };
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0, &m_rectAngle,&color);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}

void StartBar::Update()
{
	// スタートボタンが押されたなら存在を消滅させる
	if (m_startFlg)
	{
		m_alpha = 0.0f;
		m_isExpired = true;
	}

	// 点滅処理
	Flashing();

	// クリックでスタート
	if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) || GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		if(!m_pushFlg)
		{
			m_startFlg = true;
			m_pushFlg = true;
		}
	}
	else
	{
		m_pushFlg = false;
	}
}

void StartBar::Flashing()
{
	// スタートボタンが押されているなら処理をしない
	if (m_startFlg)return;
	
	//スタートボタンの点滅
	m_alpha += m_alphaAdd;

	// 透明度が上限値を超えようとしたなら加算量を変化させる
	if (m_alpha > k_alphaMax)
	{
		m_alpha = k_alphaMax;
		m_alphaAdd *= -k_alphaMax;
	}
	// 透明度が下限値を超えようとしたなら加算量を変化させる
	else if (m_alpha < k_alphaMin)
	{
		m_alpha = k_alphaMin;
		m_alphaAdd *= -k_alphaMax;
	}
}
