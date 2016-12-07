/**
* Copyright (c) 2000-2013 Liferay, Inc. All rights reserved.
*
* This library is free software; you can redistribute it and/or modify it under
* the terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; either version 2.1 of the License, or (at your option)
* any later version.
*
* This library is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
* details.
*/

#include "LiferayNativityOverlay.h"

using namespace std;

#pragma comment(lib, "shlwapi.lib")

extern HINSTANCE instanceHandle;

#define IDM_DISPLAY 0
#define IDB_OK 101

LiferayNativityOverlay::LiferayNativityOverlay() : _communicationSocket(0), _referenceCount(1)
{
}

LiferayNativityOverlay::LiferayNativityOverlay(PWSTR overlay_name) : _referenceCount(1)
{
	this->overlay_name = overlay_name;
}

LiferayNativityOverlay::~LiferayNativityOverlay(void)
{
}

IFACEMETHODIMP_(ULONG) LiferayNativityOverlay::AddRef()
{
	return InterlockedIncrement(&_referenceCount);
}

IFACEMETHODIMP LiferayNativityOverlay::QueryInterface(REFIID riid, void** ppv)
{
	HRESULT hr = S_OK;

	if (IsEqualIID(IID_IUnknown, riid) || IsEqualIID(IID_IShellIconOverlayIdentifier, riid))
	{
		*ppv = static_cast<IShellIconOverlayIdentifier*>(this);
	}
	else
	{
		hr = E_NOINTERFACE;
		*ppv = NULL;
	}

	if (*ppv)
	{
		AddRef();
	}

	return hr;
}

IFACEMETHODIMP_(ULONG) LiferayNativityOverlay::Release()
{
	ULONG cRef = InterlockedDecrement(&_referenceCount);
	if (0 == cRef)
	{
		delete this;
	}

	return cRef;
}

IFACEMETHODIMP LiferayNativityOverlay::GetPriority(int* pPriority)
{
	pPriority = 0;

	return S_OK;
}

IFACEMETHODIMP LiferayNativityOverlay::IsMemberOf(PCWSTR pwszPath, DWORD dwAttrib)
{
	// I believe I can safely ignore this for cloud sync app. overlays should always be on
	//if (!_IsOverlaysEnabled())
	//{
	//	return MAKE_HRESULT(S_FALSE, 0, 0);
	//}

	//if (!FileUtil::IsFileFiltered(pwszPath))
	//{
	//	return MAKE_HRESULT(S_FALSE, 0, 0);
	//}

	if (!_IsMonitoredFileState(pwszPath))
	{
		return MAKE_HRESULT(S_FALSE, 0, 0);
	}

	DLLCommunication comm(port);
	wstring message = DLLCommunication::DLLCommServer_Commands()[IS_SHARED_FOLDER];
	message += L"|";
	message += pwszPath;

	string response = comm.getDataFromServer(message);
	string on_server = string(DLLCommunication::ON_SERVER.begin(), DLLCommunication::ON_SERVER.end());

	if (response.compare(on_server) == 0)
	{
		return MAKE_HRESULT(S_OK, 0, 0);
	}

	else
	{
		return MAKE_HRESULT(S_FALSE, 0, 0);
	}
}

IFACEMETHODIMP LiferayNativityOverlay::GetOverlayInfo(PWSTR pwszIconFile, int cchMax, int* pIndex, DWORD* pdwFlags)
{
	// make sure you show the right icons
	*pIndex = ICON_POS_MAPPINGS.at(this->overlay_name);

	*pdwFlags = ISIOI_ICONFILE | ISIOI_ICONINDEX;

	if (GetModuleFileName(instanceHandle, pwszIconFile, cchMax) == 0)
	{
		HRESULT hResult = HRESULT_FROM_WIN32(GetLastError());

		return hResult;
	}

	return S_OK;
}

bool LiferayNativityOverlay::_IsOverlaysEnabled()
{
	int* enable = new int();
	bool success = false;

	if (RegistryUtil::ReadRegistry(REGISTRY_ROOT_KEY, REGISTRY_ENABLE_OVERLAY, enable))
	{
		if (enable)
		{
			success = true;
		}
	}

	delete enable;

	return success;
}

// adjusting the purpose of this function to determine whether or not this file should be "montiored" by this overlay ie. should we show this icon?
bool LiferayNativityOverlay::_IsMonitoredFileState(const wchar_t* filePath)
{
	bool hResult = false;

	if (this->overlay_name == OVERLAY_ERROR_NAME)
	{
		hResult = true;
	}

	return hResult;
}