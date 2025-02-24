#include "BackGround.h"
#include "../../../Manager/ModelManager/ModelManager.h"

const float BackGround::k_addAngleValue = 0.1f;
const float BackGround::k_modelSize = 2500.f;

void BackGround::Init()
{
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("BackGround");
	}

	float modelSize = m_gimmickData["BackGround"].value("ModelSize", k_modelSize);

	Math::Matrix scaleMat = Math::Matrix::CreateScale(modelSize);
	Math::Matrix transMat = Math::Matrix::CreateTranslation(0, 0, 0);
	m_mWorld = scaleMat * transMat;
}

void BackGround::Update()
{
	// 回転モードでなければ処理をしない
	if (m_nowMode != Option::RotationMode)return;

	m_angle += k_addAngleValue;

	// 360度を超えないようにする
	if (m_angle > k_rotAngleMax)
	{
		m_angle -= k_rotAngleMax;
	}

	float modelSize = m_gimmickData["BackGround"].value("ModelSize", k_modelSize);

	Math::Matrix scaleMat = Math::Matrix::CreateScale(modelSize);
	Math::Matrix transMat = Math::Matrix::CreateTranslation(0, 0, 0);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle));
	m_mWorld = scaleMat * rotMat * transMat;
}

void BackGround::DrawUnLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}