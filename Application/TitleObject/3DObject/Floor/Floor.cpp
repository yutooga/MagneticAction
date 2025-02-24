#include "Floor.h"

void Floor::Init()
{
	// モデルの読み込み
	if(!m_model)
	{
		m_model = std::make_shared<KdModelWork>();
		m_model->SetModelData("Asset/Models/Title/Floor/Floor.gltf");
	}

	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(0, -2.0, 0);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(1.0f);
	m_mWorld = scaleMat * transMat;
}

void Floor::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
