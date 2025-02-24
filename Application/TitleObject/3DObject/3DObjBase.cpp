#include "3DObjBase.h"

nlohmann::json _3DObjectBase::m_jsonData;

_3DObjectBase::_3DObjectBase()
{
	// jsonファイルの読み込み
	if (m_jsonData.empty())
	{
		std::ifstream ifs("Asset/Data/TitleScene/3DObject/3DObjectData.json");
		if (ifs)
		{
			ifs >> m_jsonData;
		}
	}
}

void _3DObjectBase::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
