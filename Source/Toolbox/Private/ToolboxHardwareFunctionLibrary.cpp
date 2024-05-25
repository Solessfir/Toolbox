// Copyright Solessfir. All Rights Reserved.

#include "ToolboxHardwareFunctionLibrary.h"
#include "GenericPlatform/GenericPlatformDriver.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ToolboxHardwareFunctionLibrary)

FGPUInfo UToolboxHardwareFunctionLibrary::GetGPUDriverInfo()
{
	const FGPUDriverInfo& GPUDriverInfo = FPlatformMisc::GetGPUDriverInfo(GRHIAdapterName, false);
	return FGPUInfo(GPUDriverInfo.ProviderName, GPUDriverInfo.DeviceDescription, GPUDriverInfo.UserDriverVersion, GPUDriverInfo.DriverDate);
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
	return FString::Format(TEXT("Provider: {0}\nDevice: {1}\nDriver Version: {2}\nDriver Date: {3}"),
	TArray<FStringFormatArg>
	{
		InGPUInfo.ProviderName,
		InGPUInfo.DeviceDescription,
		InGPUInfo.UserDriverVersion,
		InGPUInfo.DriverDate
	});
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
