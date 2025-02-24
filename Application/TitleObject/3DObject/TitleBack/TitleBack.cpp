#include "TitleBack.h"

const float TitleBack::k_modelSize = 1000.f;

void TitleBack::Init()
{
	// モデルの読み込み
	if(!m_model)
	{
		m_model = std::make_shared<KdModelWork>();
		m_model->SetModelData("Asset/Models/Terrains/BackGround/Sky.gltf");
	}

	// 行列の確定
	Math::Matrix scaleMat = Math::Matrix::CreateScale(k_modelSize);
	Math::Matrix transMat = Math::Matrix::CreateTranslation(0, 0, 0);
	m_mWorld = scaleMat * transMat;
}

void TitleBack::DrawUnLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
