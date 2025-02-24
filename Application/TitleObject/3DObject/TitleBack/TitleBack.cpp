#include "TitleBack.h"

void TitleBack::Init()
{
	// モデルの読み込み
	if(!m_model)
	{
		m_model = std::make_shared<KdModelWork>();
		m_model->SetModelData(m_jsonData["TitleBack"]["URL"]);
	}

	// モデルの表示座標
	m_pos = { m_jsonData["TitleBack"]["Pos"].value("X",0.f),
		m_jsonData["TitleBack"]["Pos"].value("Y",0.f),m_jsonData["TitleBack"]["Pos"].value("Z",0.f)
	};

	// 行列の確定
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_jsonData["TitleBack"].value("ModelSize",1000.f));
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = scaleMat * transMat;
}

void TitleBack::DrawUnLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
