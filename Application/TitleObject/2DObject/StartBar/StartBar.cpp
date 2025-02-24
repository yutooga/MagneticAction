#include "StartBar.h"

const float StartBar::k_alphaMax = 1.f;
const float StartBar::k_alphaMin = 0.3f;

void StartBar::Init()
{
	// 画像のロード
	m_tex.Load("Asset/Textures/Scene/TitleScene/StartBar/Start.png");

	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(0, -260, 0);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(0.75f);
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
	// スタートボタンが押されていないなら更新をする
	if (!m_startFlg)
	{
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
	// スタートボタンが押されたなら存在を消滅させる
	else
	{
		m_alpha = 0.0f;
		m_isExpired = true;
	}

	// ENTERキーでスタート
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
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
