// Copyright Solessfir. All Rights Reserved.

#pragma once

#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "Engine/NetConnection.h"
#include "GameFramework/PlayerController.h"

namespace ToolboxHelpers
{
	inline const UWorld* GetWorld(const UObject* WorldContextObject)
	{
		if (!GEngine)
		{
			return nullptr;
		}

		return GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	}

	inline APlayerController* GetLocalPlayerController(const UObject* WorldContextObject)
	{
		if (const UWorld* World = GetWorld(WorldContextObject))
		{
			return GEngine->GetFirstLocalPlayerController(World);
		}

		return nullptr;
	}

	inline const ULocalPlayer* GetLocalPlayer(const UObject* WorldContextObject)
	{
		if (const APlayerController* PlayerController = GetLocalPlayerController(WorldContextObject))
		{
			return PlayerController->GetLocalPlayer();
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
