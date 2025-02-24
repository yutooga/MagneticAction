#include "Magnet.h"

const float Magnet::k_addAmount = 30.f;

void Magnet::Init()
{
	// モデルの読み込み
	if(!m_model)
	{
		m_model = std::make_shared<KdModelWork>();
		m_model->SetModelData("Asset/Models/Title/Magnet/iman.gltf");
	}
}

void Magnet::Update()
{
	// 角度の更新処理
	m_angle += k_addAmount;

	// 角度が限界値を超えないようにする
	if (m_angle > k_rotAngleMax)
	{
		m_angle -= k_rotAngleMax;
	}

	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(0, 0, 0);
	Math::Matrix rotationMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle));
	m_mWorld = rotationMat * transMat;
}

void Magnet::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void Magnet::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
