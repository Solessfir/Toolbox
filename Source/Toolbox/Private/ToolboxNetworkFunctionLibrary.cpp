// Copyright Solessfir. All Rights Reserved.

#include "ToolboxNetworkFunctionLibrary.h"
#include "GameFramework/PlayerState.h"
#include "ToolboxHelpers.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ToolboxNetworkFunctionLibrary)

int32 UToolboxNetworkFunctionLibrary::GetPing(const UObject* WorldContextObject)
{
	const APlayerController* PlayerController = ToolboxHelpers::GetLocalPlayerController(WorldContextObject);
	if (!PlayerController)
	{
		return 0;
	}

	if (const APlayerState* PlayerState = PlayerController->GetPlayerState<APlayerState>())
	{
		return FMath::FloorToInt(PlayerState->GetPingInMilliseconds());
	}

	return 0;
}

EToolboxConnectionState UToolboxNetworkFunctionLibrary::GetConnectionState(const UObject* WorldContextObject)
{
	if (const UNetConnection* NetConnection = ToolboxHelpers::GetNetConnection(WorldContextObject))
	{
		return static_cast<EToolboxConnectionState>(NetConnection->GetConnectionState());
	}

	return EToolboxConnectionState::Invalid;
}

void UToolboxNetworkFunctionLibrary::GetPacketLoss(const UObject* WorldContextObject, float& Incoming, float& Outgoing)
{
	if (const UNetConnection* NetConnection = ToolboxHelpers::GetNetConnection(WorldContextObject))
	{
		Incoming = NetConnection->GetInLossPercentage().GetAvgLossPercentage();
		Outgoing = NetConnection->GetOutLossPercentage().GetAvgLossPercentage();
	}
}

void UToolboxNetworkFunctionLibrary::GetPacketRate(const UObject* WorldContextObject, int32& Incoming, int32& Outgoing)
{
	if (const UNetConnection* NetConnection = ToolboxHelpers::GetNetConnection(WorldContextObject))
	{
		Incoming = NetConnection->InPacketsPerSecond;
		Outgoing = NetConnection->OutPacketsPerSecond;
	}
}

void UToolboxNetworkFunctionLibrary::GetPacketSize(const UObject* WorldContextObject, float& Incoming, float& Outgoing)
{
	if (const UNetConnection* NetConnection = ToolboxHelpers::GetNetConnection(WorldContextObject))
	{
		Incoming = NetConnection->InPacketsPerSecond > 0
			? static_cast<float>(NetConnection->InBytesPerSecond) / static_cast<float>(NetConnection->InPacketsPerSecond)
			: 0.f;

		Outgoing = NetConnection->OutPacketsPerSecond > 0
			? static_cast<float>(NetConnection->OutBytesPerSecond) / static_cast<float>(NetConnection->OutPacketsPerSecond)
			: 0.f;
	}
}

int32 UToolboxNetworkFunctionLibrary::GetAcknowledgedPackets(const UObject* WorldContextObject)
{
	if (const UNetConnection* NetConnection = ToolboxHelpers::GetNetConnection(WorldContextObject))
	{
		return NetConnection->OutTotalAcks;
	}

	return 0;
}

void UToolboxNetworkFunctionLibrary::GetDelayedRPCs(const UObject* WorldContextObject, int32& RPCs, int32& Delay)
{
	if (const UNetConnection* NetConnection = ToolboxHelpers::GetNetConnection(WorldContextObject))
	{
		RPCs = NetConnection->TotalDelayedRPCs;
		Delay = NetConnection->TotalDelayedRPCsFrameCount;
	}
}
