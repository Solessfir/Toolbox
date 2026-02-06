// Copyright Solessfir. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToolboxHardwareFunctionLibrary.generated.h"

USTRUCT(BlueprintType)
struct FGPUInfo
{
	GENERATED_BODY()

	FGPUInfo()
		: ProviderName(FString("None"))
		, DeviceDescription(FString("None"))
		, UserDriverVersion(0.0)
		, DriverDate(FDateTime())
	{
	}

	explicit FGPUInfo(const FString& InProviderName, const FString& InDeviceDescription, const double InUserDriverVersion, const FDateTime& InDriverDate)
		: ProviderName(InProviderName)
		, DeviceDescription(InDeviceDescription)
		, UserDriverVersion(InUserDriverVersion)
		, DriverDate(FDateTime(InDriverDate))
	{
	}

	bool operator==(const FGPUInfo& Other) const
	{
		return ProviderName == Other.ProviderName && DeviceDescription == Other.DeviceDescription && UserDriverVersion == Other.UserDriverVersion && DriverDate == Other.DriverDate;
	}

	bool operator!=(const FGPUInfo& Other) const
	{
		return !operator==(Other);
	}

	friend uint32 GetTypeHash(const FGPUInfo& Other)
	{
		return FCrc::MemCrc32(&Other, sizeof(FGPUInfo));
	}

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString ProviderName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString DeviceDescription;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	double UserDriverVersion;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FDateTime DriverDate;
};

USTRUCT(BlueprintType)
struct FDisplayAdapterScreenData
{
	GENERATED_BODY()

	FDisplayAdapterScreenData()
		: Width(0)
		, Height(0)
		, RefreshRate(0)
	{
	}

	explicit FDisplayAdapterScreenData(const int32 InWidth, const int32 InHeight, const int32 InRefreshRate)
		: Width(InWidth)
		, Height(InHeight)
		, RefreshRate(InRefreshRate)
	{
	}

	bool operator==(const FDisplayAdapterScreenData& Other) const
	{
		return Width == Other.Width && Height == Other.Height && RefreshRate == Other.RefreshRate;
	}

	bool operator!=(const FDisplayAdapterScreenData& Other) const
	{
		return !operator==(Other);
	}

	friend uint32 GetTypeHash(const FDisplayAdapterScreenData& Other)
	{
		return FCrc::MemCrc32(&Other, sizeof(FDisplayAdapterScreenData));
	}

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 Width;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 Height;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 RefreshRate;
};

UENUM(BlueprintType)
enum class EGamepadTypes : uint8
{
	None,
	PS,		  // DualShock 4/5, DualSense
	XBox,	  // Xbox 360, One, Series X|S, 8BitDo
	Nintendo, // Switch Pro, JoyCons
	Valve,	  // Steam Controller, Steam Deck
	Nvidia	  // Shield Controller
};

UCLASS()
class TOOLBOX_API UToolboxHardwareFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, DisplayName = "Get GPU Info", Meta = (Keywords = "Video"), Category = "Toolbox|Hardware Library")
	static FGPUInfo GetGPUInfo();

	UFUNCTION(BlueprintPure, DisplayName = "Get CPU Info", Category = "Toolbox|Hardware Library")
	static FString GetCPUInfo();

	/**
	* Returns system and process RAM statistics in Gibibytes (GiB).
	* @param Total			Total Physical RAM installed in the system.
	* @param Available		Physical RAM currently available (System-wide).
	* @param SystemUsed		Total RAM used by the OS and all apps.
	* @param ProcessUsed	RAM used specifically by the Engine.
	*/
	UFUNCTION(BlueprintPure, DisplayName = "Get RAM Info", Category = "Toolbox|Hardware Library")
	static void GetRAMInfo(double& Total,double& Available, double& SystemUsed, double& ProcessUsed);

	UFUNCTION(BlueprintPure, Meta = (Keywords = "Screen"), Category = "Toolbox|Hardware Library")
	static TArray<FDisplayAdapterScreenData> GetAvailableResolutions();

	UFUNCTION(BlueprintPure, Meta = (BlueprintAutocast, DisplayName = "To String (GPUInfo)", CompactNodeTitle = "->", AutoCreateRefTerm = "InGPUInfo"), Category = "Toolbox|Hardware Library")
	static FString Conv_GPUInfoToString(const FGPUInfo& InGPUInfo);

	UFUNCTION(BlueprintPure, Meta = (BlueprintAutocast, DisplayName = "To String (DisplayAdapterScreenData)", CompactNodeTitle = "->", AutoCreateRefTerm = "InDisplayAdapterScreenData"), Category = "Toolbox|Hardware Library")
	static FString Conv_DisplayAdapterScreenDataToString(const FDisplayAdapterScreenData& InDisplayAdapterScreenData);

	UFUNCTION(BlueprintPure, Category = "Toolbox|Hardware Library")
	static EGamepadTypes GetConnectedGamepadType();
};
