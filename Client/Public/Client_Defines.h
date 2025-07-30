#pragma once

#include <process.h>

namespace Client
{
	const unsigned int			g_iWinSizeX = { 1280 };
	const unsigned int			g_iWinSizeY = { 720 };

	struct COOR
	{
		int y;
		int x;
	};

	enum LEVELID { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };

	enum ITEMID
	{
		ITEM_NONE,
		ITEM_POTION_1,
		ITEM_POTION_2,
		ITEM_REDSTONEPOCKET,
		ITEM_BLUESTONEPOCKET,
		ITEM_REDSTONE,
		ITEM_BLUESTONE,
		ITEM_GRENADE_WIND,
		ITEM_GRENADE_DARK,
		ITEM_POTION_AWAKEN,
		ITEM_END
	};

	enum ITEMTYPE
	{
		ITEM_NORMAL,
		ITEM_BOX,
		ITEM_POTION,
		ITEMTYPE_END
	};
}

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace Client;
using namespace std;
