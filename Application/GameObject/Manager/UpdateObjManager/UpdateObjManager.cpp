#include"UpdateObjManager.h"

void UpdateObjManager::Init()
{
	for (int i = 0; i < 6; i++)
	{
		m_keyObjects[i].maguneforce = KdGameObject::NoForce;
	}
}
