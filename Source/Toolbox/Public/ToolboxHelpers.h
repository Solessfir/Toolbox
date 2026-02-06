// Copyright Solessfir. All Rights Reserved.

#pragma once

namespace ToolboxHelpers
{
	inline const UWorld* GetWorld(const UObject* WorldContextObject)
	{
		if (!GEngine || !WorldContextObject)
		{
			return nullptr;
		}

		return GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	}

	inline const APlayerController* GetLocalPlayerController(const UObject* WorldContextObject)
	{
		if (const UWorld* World = GetWorld(WorldContextObject))
		{
			return GEngine->GetFirstLocalPlayerController(World);
		}

		return nullptr;
	}

	inline const UNetConnection* GetNetConnection(const UObject* WorldContextObject)
	{
		if (const APlayerController* PlayerController = GetLocalPlayerController(WorldContextObject))
		{
			return PlayerController->GetNetConnection();
		}

		return nullptr;
	}
}
