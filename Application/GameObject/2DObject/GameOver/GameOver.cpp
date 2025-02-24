#include "GameOver.h"

const float GameOver::k_addAlphaAmount = 0.01f;
const float GameOver::k_alphaMaxAmount = 1.0f;
const float GameOver::k_blackAlphaMaxAmount = 0.8f;

void GameOver::Init()
{
	// 画像のロード
	m_textTex.Load("Asset/Textures/Scene/GameScene/2DObject/GameOver/GameOver.png");
	m_blackTex.Load("Asset/Textures/Scene/GameScene/2DObject/GameOver/Brack.png");
	m_enterTex.Load("Asset/Textures/Scene/GameScene/2DObject/GameOver/Enter.png");

	// 座標の初期化
	m_pos = { 0,200,0 };
	m_enterPos = { 400,-230,0 };
}

void GameOver::Update()
{
	// 透明度の加算処理
	m_alpha += k_addAlphaAmount;

	// 透明度が限界値を超えたら限界値をセットする&描画終了状態にする
	if (m_alpha > k_alphaMaxAmount)
	{
		m_alpha = k_alphaMaxAmount;
		m_drawEndFlg = true;
	}

	// 背景の黒の画像の透明度の加算処理
	m_blackAlpha += k_addAlphaAmount;

	// 透明度が限界値を超えたら限界値をセットする&描画終了状態にする
	if (m_blackAlpha > k_blackAlphaMaxAmount)
	{
		m_blackAlpha = k_blackAlphaMaxAmount;
	}

	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_textSize);
	m_mWorld = scaleMat * transMat;

	m_brackMat = Math::Matrix::CreateTranslation({ 0,0,0 });
	m_enterMat = Math::Matrix::CreateTranslation(m_enterPos);
}

void GameOver::DrawSprite()
{
	// 背景の黒の画像
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_brackMat);
	Math::Rectangle blackRc = { 0,0,1280,720 };
	Math::Color blackColor = { 1,1,1,m_blackAlpha };
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_blackTex, 0, 0, &blackRc, &blackColor);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);

	// ゲームオーバーのロゴ
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
	Math::Rectangle rc = { 0,0,366,88 };
	Math::Color color = { 1,1,1,m_alpha };
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_textTex, 0, 0, &rc,&color);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);

	// エンターキーのロゴ
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_enterMat);
	Math::Rectangle enterRc = { 0,0,410,88 };
	Math::Color enterColor = { 1,1,1,m_alpha };
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_enterTex, 0, 0, &enterRc, &enterColor);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}
