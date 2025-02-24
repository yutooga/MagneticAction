#include "2DObjectBase.h"

nlohmann::json _2DObjectBase::m_jsonData;

_2DObjectBase::_2DObjectBase()
{
	if (m_jsonData.empty())
	{
		std::ifstream ifs("Asset/Data/ResultScene/2DObject/2DObjectData.json");
		if (ifs) {
			ifs >> m_jsonData;
		}
	}
}

void _2DObjectBase::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0, &m_rc, &m_color);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}
