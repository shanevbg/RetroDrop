#pragma once
#define NOMINMAX // Prevents Windows headers from defining min and max macros
#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <string>
#include <iostream>
#pragma comment(lib, "ole32.lib")
//#pragma comment(lib, "combase.lib")
#pragma comment(lib, "winmm.lib")

// Include other necessary headers and libraries such as ole32.lib, combase.lib, etc.
//#define PKEY_Device_FriendlyName manually if not defined
#ifndef PKEY_Device_FriendlyName
DEFINE_PROPERTYKEY(PKEY_Device_FriendlyName, 0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, CURRENCY, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 14);
#endif

class AudioDeviceManager {
public:
    HRESULT Initialize(); // Initializes the COM library and creates an audio device enumerator instance.
    int EnumerateDevices(IMMDeviceCollection** pDevices); // Retrieves a collection of available audio endpoints.
    std::wstring GetSelectedDeviceName(IMMDevice* selectedDevice); // Returns the friendly name of a given IMMDevice pointer.
    float AdjustVolumeLevel(float levelScalar, IAudioEndpointVolume* volumeControl); // Sets the master volume level for an audio device.
private:
    HRESULT EnumerateEndpoints(); // Helper function to enumerate all available devices and store them in 'pDevices'.
    int SelectDeviceByNumber(IMMDeviceCollection* pDevices, std::wstring& selectedDeviceName); // Allows the user to select an audio device by its number.
};