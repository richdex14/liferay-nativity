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

#include <map>

#define	OVERLAY_ID					3
#define OVERLAY_SYNCED_GUID			L"{0DD5B4B0-25AF-4E09-A46B-9F274F3D7003}"
#define OVERLAY_PENDING_GUID		L"{0DD5B4B1-25AF-4E09-A46B-9F274F3D7003}"
#define OVERLAY_CHANGES_GUID		L"{0DD5B4B2-25AF-4E09-A46B-9F274F3D7003}"
#define OVERLAY_ERROR_GUID			L"{0DD5B4B3-25AF-4E09-A46B-9F274F3D7003}"

#define OVERLAY_SYNCED_NAME			L"     desktop-sync-icon-synced"
#define OVERLAY_PENDING_NAME		L"     desktop-sync-icon-pending"
#define OVERLAY_CHANGES_NAME		L"     desktop-sync-icon-changes"
#define OVERLAY_ERROR_NAME			L"     desktop-sync-icon-error"

#pragma once

template<class T> struct PWSTR_less
{
	// NOTE: THIS IS NOT TYPESAFE AND SHOULD ONLY BE USED TO COMPATE TYPES RESOLVING to wchar_t*
	bool operator()(const T& a, const T& b) const
	{
		std::wstring a_str((PWSTR)a);
		std::wstring b_str((PWSTR)b);

		int res = a_str.compare(b_str);

		return res > 0;
	}
};

const std::map<PWSTR, PWSTR, PWSTR_less<PWSTR>> GUID_mappings{ { OVERLAY_SYNCED_GUID, OVERLAY_SYNCED_NAME },{ OVERLAY_PENDING_GUID, OVERLAY_PENDING_NAME },{ OVERLAY_CHANGES_GUID, OVERLAY_CHANGES_NAME },{ OVERLAY_ERROR_GUID, OVERLAY_ERROR_NAME } };


#define REGISTRY_OVERLAY_KEY		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellIconOverlayIdentifiers"
#define REGISTRY_CLSID				L"CLSID"
#define REGISTRY_IN_PROCESS			L"InprocServer32"
#define REGISTRY_THREADING			L"ThreadingModel"
#define REGISTRY_APARTMENT			L"Apartment"
#define REGISTRY_VERSION			L"Version"
#define REGISTRY_VERSION_NUMBER		L"1.0"