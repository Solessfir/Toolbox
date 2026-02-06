// Copyright Solessfir. All Rights Reserved.

#include "ToolboxHardwareFunctionLibrary.h"
#include "GenericPlatform/GenericPlatformDriver.h"
#if PLATFORM_WINDOWS
#include "Windows/WindowsHWrapper.h"
#elif PLATFORM_LINUX
#include "SDL3/SDL.h"
#endif
#include UE_INLINE_GENERATED_CPP_BY_NAME(ToolboxHardwareFunctionLibrary)

DEFINE_LOG_CATEGORY_STATIC(LogToolboxHardware, Log, All);

FGPUInfo UToolboxHardwareFunctionLibrary::GetGPUInfo()
{
	#if PLATFORM_WINDOWS
	const FGPUDriverInfo& GPUDriverInfo = FPlatformMisc::GetGPUDriverInfo(GRHIAdapterName, false);
	#elif PLATFORM_LINUX
	FString DeviceDescription;
	const FGPUDriverInfo& GPUDriverInfo = FPlatformMisc::GetGPUDriverInfo(DeviceDescription);
	#endif

	TArray<FString> DateArray;
	GPUDriverInfo.DriverDate.ParseIntoArray(DateArray, TEXT("-"));

	const FDateTime DriverDate = DateArray.Num() >= 3
		? FDateTime(FCString::Atoi(*DateArray[2]), FCString::Atoi(*DateArray[0]), FCString::Atoi(*DateArray[1]))
		: FDateTime();

	return FGPUInfo(GPUDriverInfo.ProviderName, GPUDriverInfo.DeviceDescription, FCString::Atod(*GPUDriverInfo.UserDriverVersion), DriverDate);
}

FString UToolboxHardwareFunctionLibrary::GetCPUInfo()
{
	return FPlatformMisc::GetCPUBrand();
}

void UToolboxHardwareFunctionLibrary::GetRAMInfo(double& Total,double& Available, double& SystemUsed, double& ProcessUsed)
{
	const FPlatformMemoryStats MemoryStats = FPlatformMemory::GetStats();

	// 1024^3 bytes per GiB
	constexpr double BytesToGiB = 1.0 / (1024.0 * 1024.0 * 1024.0);

	Total = MemoryStats.TotalPhysical * BytesToGiB;
	Available = MemoryStats.AvailablePhysical * BytesToGiB;
	SystemUsed = Total - Available;
	ProcessUsed = MemoryStats.UsedPhysical * BytesToGiB;
}

TArray<FDisplayAdapterScreenData> UToolboxHardwareFunctionLibrary::GetAvailableResolutions()
{
	TArray<FDisplayAdapterScreenData> DisplayAdapterScreenData;
	TArray<FScreenResolutionRHI> ScreenResolutions;

	if (!RHIGetAvailableResolutions(ScreenResolutions, false))
	{
		return DisplayAdapterScreenData;
	}

	for (const auto& [Width, Height, RefreshRate] : ScreenResolutions)
	{
		// Exclude duplicates
		DisplayAdapterScreenData.AddUnique(FDisplayAdapterScreenData(Width, Height, RefreshRate));
	}

	return DisplayAdapterScreenData;
}

FString UToolboxHardwareFunctionLibrary::Conv_GPUInfoToString(const FGPUInfo& InGPUInfo)
{
	return FString::Printf(TEXT("Provider: %s\nDevice: %s\nDriver Version: %.2lf\nDriver Date: %s")
		, *InGPUInfo.ProviderName
		, *InGPUInfo.DeviceDescription
		, InGPUInfo.UserDriverVersion
		, *InGPUInfo.DriverDate.ToFormattedString(TEXT("%d/%m/%Y")));
}

FString UToolboxHardwareFunctionLibrary::Conv_DisplayAdapterScreenDataToString(const FDisplayAdapterScreenData& InDisplayAdapterScreenData)
{
	return FString::Format(TEXT("{0} x {1}, {2} Hz"),
	TArray<FStringFormatArg>
	{
		InDisplayAdapterScreenData.Height,
		InDisplayAdapterScreenData.Width,
		InDisplayAdapterScreenData.RefreshRate
	});
}

EGamepadTypes UToolboxHardwareFunctionLibrary::GetConnectedGamepadType()
{
	// Helper to determine type from Vendor ID
	auto DetermineGamepadTypeFromVID = [](const uint32 VendorID) -> EGamepadTypes
	{
		switch (VendorID)
		{
			case 0x054C: return EGamepadTypes::PS;		 // Sony (DualShock 4/5, DualSense)
			case 0x045E: return EGamepadTypes::XBox;	 // Microsoft (Xbox 360, One, Series X|S)
			case 0x2DC8: return EGamepadTypes::XBox;	 // 8BitDo Controllers
			case 0x057E: return EGamepadTypes::Nintendo; // Nintendo (Switch Pro, JoyCons)
			case 0x28DE: return EGamepadTypes::Valve;	 // Valve (Steam Controller, Steam Deck)
			case 0x0955: return EGamepadTypes::Nvidia;	 // Nvidia (Shield Controller)
			default: return EGamepadTypes::None;
		}
	};

	#if PLATFORM_WINDOWS
	uint32 DeviceCount = 0;
	if (GetRawInputDeviceList(nullptr, &DeviceCount, sizeof(RAWINPUTDEVICELIST)) == static_cast<uint32>(-1))
	{
		UE_LOGFMT(LogToolboxHardware, Error, "Failed to query raw input device count. | {0}:{1}", __FUNCTION__, __LINE__);
		return EGamepadTypes::None;
	}

	if (DeviceCount == 0)
	{
		return EGamepadTypes::None;
	}

	TArray<RAWINPUTDEVICELIST> DeviceList;
	DeviceList.SetNumUninitialized(DeviceCount + 2); // Add a slight buffer in case a device is plugged in between the two calls

	DeviceCount = GetRawInputDeviceList(DeviceList.GetData(), &DeviceCount, sizeof(RAWINPUTDEVICELIST));
	if (DeviceCount == static_cast<uint32>(-1))
	{
		UE_LOGFMT(LogToolboxHardware, Error, "Failed to retrieve raw input device list. | {0}:{1}", __FUNCTION__, __LINE__);
		return EGamepadTypes::None;
	}

	TArray<char> DeviceNameBuffer;
	DeviceNameBuffer.Reserve(256);

	for (uint32 Index = 0; Index < DeviceCount; ++Index)
	{
		const auto& [hDevice, dwType] = DeviceList[Index];

		if (dwType != RIM_TYPEHID)
		{
			continue;
		}

		// Get Device Name Length
		uint32 NameLen = 0;
		if (GetRawInputDeviceInfoA(hDevice, RIDI_DEVICENAME, nullptr, &NameLen) == static_cast<uint32>(-1))
		{
			continue;
		}

		// Resize buffer if needed
		if (static_cast<uint32>(DeviceNameBuffer.Num()) < NameLen + 1)
		{
			DeviceNameBuffer.SetNumUninitialized(NameLen + 1, EAllowShrinking::No);
		}

		// Get Device Name
		if (GetRawInputDeviceInfoA(hDevice, RIDI_DEVICENAME, DeviceNameBuffer.GetData(), &NameLen) == static_cast<uint32>(-1))
		{
			DWORD LastError = GetLastError();
			if (LastError != ERROR_FILE_NOT_FOUND)
			{
				UE_LOGFMT(LogToolboxHardware, Warning, "Error reading device name (Code: {0}) | {1}:{2}", static_cast<uint32>(LastError), __FUNCTION__, __LINE__);
			}
			continue;
		}

		// Get Device Info
		RID_DEVICE_INFO RawDeviceInfo = {};
		RawDeviceInfo.cbSize = sizeof(RID_DEVICE_INFO);
		uint32 DeviceInfoLen = sizeof(RID_DEVICE_INFO);

		if (GetRawInputDeviceInfo(hDevice, RIDI_DEVICEINFO, &RawDeviceInfo, &DeviceInfoLen) == static_cast<uint32>(-1))
		{
			continue;
		}

		// Check Logic
		const EGamepadTypes DetectedType = DetermineGamepadTypeFromVID(RawDeviceInfo.hid.dwVendorId);

		// If it's a Microsoft ID - verify it is actually a Gamepad/Joystick
		// Microsoft Vendor ID (0x045E) is shared by keyboards, mice, and headsets
		if (DetectedType == EGamepadTypes::XBox && RawDeviceInfo.hid.dwVendorId == 0x045E)
		{
			// Usage Page 1 = Generic Desktop, Usage 4 = Joystick, 5 = Gamepad, 8 = Multi-axis
			if (RawDeviceInfo.hid.usUsagePage != 0x01 || (RawDeviceInfo.hid.usUsage != 0x04 && RawDeviceInfo.hid.usUsage != 0x05 && RawDeviceInfo.hid.usUsage != 0x08))
			{
				// It's a Microsoft device, but not a controller
				continue;
			}
		}

		if (DetectedType != EGamepadTypes::None)
		{
			return DetectedType;
		}
	}
	return EGamepadTypes::None;
	#elif PLATFORM_LINUX
	EGamepadTypes DetectedType = EGamepadTypes::None;

	// Initialize SDL Joystick subsystem if not already active
	if (!SDL_WasInit(SDL_INIT_JOYSTICK))
	{
		if (!SDL_InitSubSystem(SDL_INIT_JOYSTICK))
		{
			return DetectedType;
		}
	}

	int32 Count = 0;
	if (SDL_JoystickID* Joysticks = SDL_GetJoysticks(&Count))
	{
		for (int32 Index = 0; Index < Count; ++Index)
		{
			const uint16 VendorId = SDL_GetJoystickVendorForID(Joysticks[Index]);
			DetectedType = DetermineGamepadTypeFromVID(VendorId);
			if (DetectedType != EGamepadTypes::None)
			{
				break;
			}
		}
		SDL_free(Joysticks);
	}
	return DetectedType;
	#else
	return EGamepadTypes::None;
	#endif
}
