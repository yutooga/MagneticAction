#include "Reticle.h"

void Reticle::Init()
{
	// 画像のロード
	m_tex.Load(m_jsonData["Reticle"]["URL"]);

	// 画像の切り取り範囲の初期化
	m_rc = { 0,0,
		m_jsonData["Reticle"]["Rc"].value("X", 64),
		m_jsonData["Reticle"]["Rc"].value("Y", 64) };
}

void Reticle::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0, &m_rc);
}
