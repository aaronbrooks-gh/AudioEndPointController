// ----------------------------------------------------------------------------
// EndPointController.cpp
// Provides a library for setting the default audio renderer and a few other nice things.
// Originally found on http://www.daveamenta.com/2011-05/programmatically-or-command-line-change-the-default-sound-playback-device-in-windows-7/
// Rewritten into DLL form by Aaron Brooks, 2012.
// ----------------------------------------------------------------------------


#include <stdio.h>
#include <wchar.h>
#include <tchar.h>
#include "windows.h"
#include "Mmdeviceapi.h"
#include "PolicyConfig.h"
#include "Functiondiscoverykeys_devpkey.h"

__declspec(dllexport) HRESULT SetDefaultAudioPlaybackDevice(LPCWSTR devID)
{	
	IPolicyConfigVista *pPolicyConfig;
	ERole reserved = eConsole;

    HRESULT hr = CoCreateInstance(__uuidof(CPolicyConfigVistaClient), 
		NULL, CLSCTX_ALL, __uuidof(IPolicyConfigVista), (LPVOID *)&pPolicyConfig);
	if (SUCCEEDED(hr))
	{
		hr = pPolicyConfig->SetDefaultEndpoint(devID, reserved);
		pPolicyConfig->Release();
	}
	return hr;
}

__declspec(dllexport) LPCWSTR GetDeviceID(int deviceCount)
{
	LPWSTR wstrID = NULL;
	IMMDeviceEnumerator *pEnum = NULL;
	IMMDeviceCollection *pDevices;
	IMMDevice *pDevice;

	CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
	pEnum->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pDevices);
	pDevices->Item(deviceCount, &pDevice);
	pDevice->GetId(&wstrID);
	return wstrID;
}

__declspec(dllexport) LPWSTR GetDeviceIDasLPWSTR(int deviceCount)
{
	LPWSTR wstrID = NULL;
	IMMDeviceEnumerator *pEnum = NULL;
	IMMDeviceCollection *pDevices;
	IMMDevice *pDevice;

	CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
	pEnum->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pDevices);
	pDevices->Item(deviceCount, &pDevice);
	pDevice->GetId(&wstrID);
	return wstrID;
}

__declspec(dllexport) LPWSTR GetDeviceFriendlyName(int deviceCount)
{
	IMMDeviceEnumerator *pEnum = NULL;
	IMMDeviceCollection *pDevices;
	IMMDevice *pDevice;
	IPropertyStore *pStore;
	PROPVARIANT friendlyName;

	CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
	pEnum->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pDevices);
	pDevices->Item(deviceCount, &pDevice);
	pDevice->OpenPropertyStore(STGM_READ, &pStore);
	pStore->GetValue(PKEY_Device_FriendlyName, &friendlyName);

	return friendlyName.pwszVal;
}

__declspec(dllexport) int GetDeviceCount()
{
	UINT count;
	IMMDeviceEnumerator *pEnum = NULL;
	IMMDeviceCollection *pDevices;

	CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
	pEnum->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pDevices);
	pDevices->GetCount(&count);
	return count;
}