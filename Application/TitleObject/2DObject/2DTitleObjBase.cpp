#include "2DTitleObjBase.h"

nlohmann::json _2DTitleObjBase::m_jsonData;

_2DTitleObjBase::_2DTitleObjBase()
{
	// jsonファイルの読み込み
	if (m_jsonData.empty())
	{
		std::ifstream ifs("Asset/Data/TitleScene/2DObject/2DObjectData.json");
		if (ifs)
		{
			ifs >> m_jsonData;
		}
	}
}
