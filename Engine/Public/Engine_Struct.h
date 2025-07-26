#pragma once

namespace Engine
{
	typedef struct
	{
		HWND			hWnd;
		unsigned int	iWinSizeX;
		unsigned int	iWinSizeY;
		bool			isWindowsed;
	}ENGINE_DESC;

	typedef struct ENGINE_DLL 
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		static const unsigned int	iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];	
	}VTXPOSTEX;

	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;

		static const unsigned int	iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXNORTEX;


}