#include "Title.h"

void Title::Init()
{
	// 画像の実体
	m_tex.Load("Asset/Textures/Scene/TitleScene/Title/Title.png");

	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(0, 260, 0);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(1.0f);
	m_mWorld = scaleMat * transMat;
}

void Title::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex,0,0,&m_rectAngle);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}
