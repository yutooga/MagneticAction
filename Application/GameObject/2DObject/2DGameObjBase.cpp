#include "2DGameObjBase.h"

nlohmann::json _2DGameObjBase::m_jsonData;

_2DGameObjBase::_2DGameObjBase()
{
	// jsonファイルから情報を読み込んでくる
	if (m_jsonData.empty())
	{
		std::ifstream ifs("Asset/Data/GameScene/2DObject/2DObjectData.json");
		if (ifs)
		{
			ifs >> m_jsonData;
		}
	}
}
