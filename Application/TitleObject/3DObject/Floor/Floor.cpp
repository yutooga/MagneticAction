#include "Floor.h"

void Floor::Init()
{
	// モデルの読み込み
	if(!m_model)
	{
		m_model = std::make_shared<KdModelWork>();
		m_model->SetModelData(m_jsonData["Floor"]["URL"]);
	}

	// モデルの表示座標
	m_pos = { m_jsonData["Floor"]["Pos"].value("X",0.f),
		m_jsonData["Floor"]["Pos"].value("Y",-2.f),m_jsonData["Floor"]["Pos"].value("Z",0.f)
	};

	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_jsonData["Floor"].value("ModelSize", 1.f));
	m_mWorld = scaleMat * transMat;
}
