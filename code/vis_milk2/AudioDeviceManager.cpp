#include "AudioDeviceManager.h"
#define E_HRESULT E_FAIL // Define E_HRESULT as a fallback error code
// Include necessary headers and libraries...
HRESULT AudioDeviceManager::Initialize() {
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr)) return hr; // Return the error code for COM initialization failure.
    return S_OK; // Successful initialization.
}

    int AudioDeviceManager::EnumerateDevices(IMMDeviceCollection** pDevices) {
        // Declare and initialize pEnum
        IMMDeviceEnumerator* pEnum = nullptr;
        HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
        if (FAILED(hr)) return hr; // Return the HRESULT error code for enumerator creation failure.

        // Use pEnum to enumerate audio endpoints
        hr = pEnum->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, pDevices);
        if (FAILED(hr)) {
            pEnum->Release();
            return E_FAIL; // Return the error code from enumeration failure.
        }

        pEnum->Release(); // Release pEnum after use
        return S_OK; // Successful enumeration
    }

std::wstring AudioDeviceManager::GetSelectedDeviceName(IMMDevice* selectedDevice) {
    PROPVARIANT pvName;
    PropVariantInit(&pvName);
    IPropertyStore* pProps = nullptr;
    HRESULT hr = selectedDevice->OpenPropertyStore(STGM_READ, &pProps);
    if (FAILED(hr)) {
        return L"Failed to open property store"; // Return an error message as a wstring.
    }
    hr = pProps->GetValue(PKEY_Device_FriendlyName, &pvName);
    if (FAILED(hr)) {
        PropVariantClear(&pvName);
        pProps->Release();
        return L"Failed to get device name"; // Return an error message as a wstring.
    }

    std::wstring deviceName = pvName.pwszVal;
    PropVariantClear(&pvName);
    pProps->Release();
    return deviceName;
}

float AudioDeviceManager::AdjustVolumeLevel(float levelScalar, IAudioEndpointVolume* volumeControl) {
    // Adjust the master volume level
    HRESULT hr = volumeControl->SetMasterVolumeLevelScalar(
        static_cast<float>(levelScalar * 0.8f), // Volume level
        nullptr                                // Event context (set to nullptr if not needed)
    );
    if (FAILED(hr)) {
        std::wcerr << L"Failed to set master volume level." << std::endl; // Log the error
        return -1.0f; // Return an error indicator as a float
    }

    // Remove the invalid SetVolumeLevelScalar call
    // hr = volumeControl->SetVolumeLevelScalar(static_cast<float>(levelScalar * 0.8f));

    // Set the mute state based on the volume level
    hr = volumeControl->SetMute(levelScalar == 0.0f ? TRUE : FALSE, nullptr); // Mute if level is at the minimum value (muted).
    if (FAILED(hr)) {
        std::wcerr << L"Failed to mute." << std::endl; // Log the error
        return -1.0f; // Return an error indicator as a float
    }

    return 0.0f; // Return success as a float
}

HRESULT AudioDeviceManager::EnumerateEndpoints() {
    IMMDeviceEnumerator* pEnum = nullptr;
    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
    if (FAILED(hr)) return hr; // Return the HRESULT error code for enumerator creation failure.

    IMMDeviceCollection* pDevices = nullptr;
    hr = pEnum->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pDevices);
    if (FAILED(hr)) {
        pEnum->Release();
        return hr; // Return the HRESULT error code for enumeration failure.
    }

    UINT count = 0;
    hr = pDevices->GetCount(&count);
    if (FAILED(hr)) {
        pDevices->Release();
        pEnum->Release();
        return hr; // Return the HRESULT error code for getting device count.
    }

    for (UINT i = 0; i < count; ++i) {
        IMMDevice* pDevice = nullptr;
        hr = pDevices->Item(i, &pDevice);
        if (FAILED(hr)) {
            pDevices->Release();
            pEnum->Release();
            return hr; // Return the HRESULT error code for device retrieval failure.
        }
        pDevice->Release(); // Release the device after processing.
    }

    pDevices->Release();
    pEnum->Release();
    return S_OK; // Successful enumeration.
}
bool ValidateInput(int number, UINT count) {
    // Ensure the input number is within the valid range
    return number >= 1 && static_cast<UINT>(number) <= count;
}

int AudioDeviceManager::SelectDeviceByNumber(IMMDeviceCollection* pDevices, std::wstring& selectedDeviceName) {
    UINT count = 0;
    HRESULT hr = pDevices->GetCount(&count); // Retrieve the number of devices.
    if (FAILED(hr)) return E_HRESULT; // Return an error code from getting device count failure.

    std::wcout << "Available audio endpoints:" << std::endl;
    for (int i = 0; i < count; ++i) {
        IMMDevice* pDevice = nullptr;
        HRESULT hr = pDevices->Item(i, &pDevice);
        if (FAILED(hr))
        {
            pDevice->Release(); // Release the device after use
            return E_HRESULT; // Return an error code from device retrieval failure
        }
    	std::wstring deviceName = GetSelectedDeviceName(pDevice);
        std::wcout << L"[" << i + 1 << L"] " << deviceName << std::endl;
        // Use the retrieved device (pDevice) as needed
        pDevice->Release(); // Release the device after use
    }

    int index = -1;
    std::wcout << "Select an endpoint by number: ";
    wchar_t input[20];
    std::wcout << L"Select an endpoint by number: ";
    while (index == -1) {
        std::wcin.getline(input, 20); // Read input into the wchar_t array
        if (std::wcin.fail()) { // Check for invalid input
            std::wcin.clear(); // Clear the error state
            std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), L'\n'); // Discard invalid input
            std::wcerr << L"Invalid selection, try again." << std::endl;
            continue;
        }

        int number = _wtoi(input); // Convert the input to an integer
        if (!ValidateInput(number, count)) {
            std::wcerr << L"Invalid selection. Please choose a number between 1 and " << count << L".\n";
            continue;
        }

        index = number; // Set the valid index
    }
    IMMDevice* selectedDevice = nullptr; // Declare and initialize selectedDevice
    HRESULT hr3 = pDevices->Item(index - 1, &selectedDevice); // Retrieve the device at 'index - 1'.
        if (FAILED(hr3)) return E_HRESULT; // Return an error code from retrieval failure.

    IPropertyStore* pProps = nullptr; // Declare and initialize pProps
    HRESULT hr4 = selectedDevice->OpenPropertyStore(STGM_READ, &pProps);
        if (FAILED(hr4)) {
            std::cerr << "Failed to open property store." << std::endl;
            return E_HRESULT; // Return an error code from property retrieval failure.
        }

    PROPVARIANT pvName; // Declare the variable
    PropVariantInit(&pvName); // Initialize the variable

    hr = pProps->GetValue(PKEY_Device_FriendlyName, &pvName); // Use the variable
        if (FAILED(hr)) {
            std::cerr << "Failed to get device name." << std::endl;
            return E_HRESULT; // Return an error code from property retrieval failure.
        }

        selectedDeviceName = pvName.pwszVal;
        PropVariantClear(&pvName);
        pProps->Release();
    CoUninitialize(); // Uninitialize COM library.
    return S_OK; // Successful selection of a device.
}
