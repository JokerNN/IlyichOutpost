#pragma once
#include "panelbase.h"

class PanelTowers :
	public PanelBase
{
	CCSprite *selectionSprite;

public:
	PanelTowers(CCScene *scene, const CCPoint ccp, const CCSize size);
	virtual ~PanelTowers(void);

	void SelectCell(CCSprite *sprite);
	void UnSelectCell(CCSprite *sprite);
};

