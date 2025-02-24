#include "Reticle.h"

void Reticle::Init()
{
	// 画像のロード
	m_tex = std::make_shared<KdTexture>();
	m_tex->Load("Asset/Textures/Scene/GameScene/2DObject/Reticle/Reticle.png");
}

void Reticle::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_tex, 0, 0, &m_rec);
}
