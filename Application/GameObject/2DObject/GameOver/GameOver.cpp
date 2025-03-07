#include "GameOver.h"

const float GameOver::k_addAlphaAmount = 0.01f;
const float GameOver::k_alphaMax = 1.0f;
const float GameOver::k_blackAlphaMax = 0.8f;

void GameOver::Init()
{
	// 画像のロード
	m_textTex.Load(m_jsonData["GameOver"]["Text"]["URL"]);
	m_blackTex.Load(m_jsonData["GameOver"]["Black"]["URL"]);
	m_clickTex.Load(m_jsonData["GameOver"]["Click"]["URL"]);

	// 座標の初期化
	m_pos = { m_jsonData["GameOver"]["Text"]["Pos"].value("X",0.0f),
		m_jsonData["GameOver"]["Text"]["Pos"].value("Y",200.0f),0 };

	m_enterPos = { m_jsonData["GameOver"]["Click"]["Pos"].value("X",400.0f),
		m_jsonData["GameOver"]["Click"]["Pos"].value("Y",-230.0f),0 };

	// 表示サイズの初期化
	m_size = m_jsonData["GameOver"]["Text"].value("Text",2.0f);

	// 画像の切り取り範囲の初期化
	m_blackRc = { 0,0,
		m_jsonData["GameOver"]["Black"]["Rc"].value("X",1280),
		m_jsonData["GameOver"]["Black"]["Rc"].value("Y",720) };

	m_rc = { 0,0,
		m_jsonData["GameOver"]["Text"]["Rc"].value("X",366),
		m_jsonData["GameOver"]["Text"]["Rc"].value("Y",88) };

	enterRc = { 0,0,
		m_jsonData["GameOver"]["Click"]["Rc"].value("X",403),
		m_jsonData["GameOver"]["Click"]["Rc"].value("Y",64) };

	// 表示色の初期化
	m_blackColor = { m_jsonData["GameOver"].value("RgbMax",1.f),
		m_jsonData["GameOver"].value("RgbMax",1.f),
		m_jsonData["GameOver"].value("RgbMax",1.f),m_blackAlpha };

	m_color = { m_jsonData["GameOver"].value("RgbMax",1.f),
		m_jsonData["GameOver"].value("RgbMax",1.f),
		m_jsonData["GameOver"].value("RgbMax",1.f),m_alpha };

	m_clickColor = m_color;
}

void GameOver::Update()
{
	// 透明度の加算処理
	m_alpha += k_addAlphaAmount;

	// 透明度が限界値を超えたら限界値をセットする&描画終了状態にする
	if (m_alpha > k_alphaMax)
	{
		m_alpha = k_alphaMax;
		m_drawEndFlg = true;
	}

	// 背景の黒の画像の透明度の加算処理
	m_blackAlpha += k_addAlphaAmount;

	// 透明度が限界値を超えたら限界値をセットする&描画終了状態にする
	if (m_blackAlpha > k_blackAlphaMax)
	{
		m_blackAlpha = k_blackAlphaMax;
	}

	// 色の更新
	ColorUpdate();

	// 行列の更新
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_size);
	m_mWorld = scaleMat * transMat;

	m_brackMat = Math::Matrix::CreateTranslation({ 0,0,0 });
	m_clickMat = Math::Matrix::CreateTranslation(m_enterPos);
}

void GameOver::DrawSprite()
{
	// 背景の黒の画像
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_brackMat);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_blackTex, 0, 0, &m_blackRc, &m_blackColor);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);

	// ゲームオーバーのロゴ
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_textTex, 0, 0, &m_rc,&m_color);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);

	// エンターキーのロゴ
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_clickMat);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_clickTex, 0, 0, &enterRc, &m_clickColor);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}

void GameOver::ColorUpdate()
{
	m_blackColor = { m_jsonData["GameOver"].value("RgbMax",1.f),
		m_jsonData["GameOver"].value("RgbMax",1.f),
		m_jsonData["GameOver"].value("RgbMax",1.f),m_blackAlpha };

	m_color = { m_jsonData["GameOver"].value("RgbMax",1.f),
		m_jsonData["GameOver"].value("RgbMax",1.f),
		m_jsonData["GameOver"].value("RgbMax",1.f),m_alpha };

	m_clickColor = m_color;
}
