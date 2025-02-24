#pragma once

#include"../BaseScene/BaseScene.h"

class TitleFadeIn;

class TitleScene : public BaseScene
{
public :

	TitleScene() { Init(); }
	~TitleScene() {}

	static const float k_alphaMax;	// 透明度の限界値

private :

	void Event() override;
	void Init() override;

	// フェードインの実態
	std::weak_ptr<TitleFadeIn> m_fadeIn;

	//SE再生されたかどうか判断フラグ
	bool m_sePlayFlg = false;
};
