#include "Title.h"

void Title::Init()
{
	// 画像のロード
	m_tex.Load(m_jsonData["Title"]["URL"]);

	// 画像の表示座標
	m_pos = { m_jsonData["Title"]["Pos"].value("X",0.f),
		m_jsonData["Title"]["Pos"].value("Y",260.f),m_jsonData["Title"]["Pos"].value("Z",0.f)
	};

	//画像の表示範囲
	m_rectAngle = { 0,0,m_jsonData["Title"].value("RcX",1028),m_jsonData["Title"].value("RcY",114) };

	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_jsonData["Title"].value("TextureSize", 1.f));
	m_mWorld = scaleMat * transMat;
}

void Title::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex,0,0,&m_rectAngle);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}
