// Copyright Solessfir. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToolboxConfigFunctionLibrary.generated.h"

UCLASS()
class TOOLBOX_API UToolboxConfigFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CustomThunk, Meta = (CustomStructureParam = "WCPinForInt,WCPinForFloat,WCSummOutput", BlueprintInternalUseOnly = true), Category = "Toolbox Config Function Library")
	bool WildcardFunctionTest(FString StringProperty, const int32& WCPinForInt, const int32& WCPinForFloat, int32& WCSummOutput);
	DECLARE_FUNCTION(execWildcardFunctionTest);

	UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true))
	static void LogNumber(const int32 Number)
	{
		UE_LOG(LogTemp, Warning, TEXT("Log Number is: %d"), Number)
	}
	
	// void GetConfigOprions(UPARAM(Meta=(GetOptions="Engine.KismetSystemLibrary.GetCollisionProfileNames")) FName ProfileName)
	
	UFUNCTION(BlueprintCallable, Category = "Toolbox Config Function Library")
	// tatic void GetConfigOptions(UPARAM(Meta=(GetOptions="Engine.ToolboxConfigFunctionLibrary.GetConfigFilenames")) FName ProfileName)
	static void GetConfigOptions(FName ProfileName)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hello"))
	};

	UFUNCTION(BlueprintCallable, Category = "Toolbox Config Function Library")
	static TArray<FName> GetConfigFilenames()
	{
		TArray<FName> IniFiles;
		for (const FString& FileName : GConfig->GetFilenames())
		{
			if (!FileName.Contains(":"))
			{
				IniFiles.Add(FName(FileName));
			}
		}

		return IniFiles;
	};
	
};
