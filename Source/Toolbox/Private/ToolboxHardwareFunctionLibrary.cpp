// Copyright Solessfir. All Rights Reserved.

#include "ToolboxHardwareFunctionLibrary.h"
#include "GenericPlatform/GenericPlatformDriver.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ToolboxHardwareFunctionLibrary)

FGPUInfo UToolboxHardwareFunctionLibrary::GetGPUInfo()
{
	const FGPUDriverInfo& GPUDriverInfo = FPlatformMisc::GetGPUDriverInfo(GRHIAdapterName, false);

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

void UToolboxHardwareFunctionLibrary::GetRAMInfo(double& Total, double& Used, double& Available)
{
	const FPlatformMemoryStats MemoryStats = FPlatformMemory::GetStats();
	constexpr double GiB = 1024.0 * 1024.0 * 1024.0; // Binary: 1,073,741,824 bytes
	Total = static_cast<double>(MemoryStats.TotalPhysical) / GiB;
	Used = static_cast<double>(MemoryStats.UsedPhysical) / GiB;
	Available = static_cast<double>(MemoryStats.AvailablePhysical) / GiB;
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
