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
		switch (NetConnection->GetConnectionState())
		{
			case USOCK_Closed: return EToolboxConnectionState::Closed;
			case USOCK_Pending: return EToolboxConnectionState::Pending;
			case USOCK_Open: return EToolboxConnectionState::Open;
			case USOCK_Closing: return EToolboxConnectionState::Closing;
			case USOCK_Invalid:
			default: return EToolboxConnectionState::Invalid;
		}
	}

	return EToolboxConnectionState::Invalid;
}

void UToolboxNetworkFunctionLibrary::GetPacketLoss(const UObject* WorldContextObject, float& Incoming, float& Outgoing)
{
	Incoming = 0.f;
	Outgoing = 0.f;

	if (const UNetConnection* NetConnection = ToolboxHelpers::GetNetConnection(WorldContextObject))
	{
		Incoming = NetConnection->GetInLossPercentage().GetAvgLossPercentage();
		Outgoing = NetConnection->GetOutLossPercentage().GetAvgLossPercentage();
	}
}

void UToolboxNetworkFunctionLibrary::GetPacketRate(const UObject* WorldContextObject, int32& Incoming, int32& Outgoing)
{
	Incoming = 0;
	Outgoing = 0;

	if (const UNetConnection* NetConnection = ToolboxHelpers::GetNetConnection(WorldContextObject))
	{
		Incoming = NetConnection->InPacketsPerSecond;
		Outgoing = NetConnection->OutPacketsPerSecond;
	}
}

void UToolboxNetworkFunctionLibrary::GetPacketSize(const UObject* WorldContextObject, float& Incoming, float& Outgoing)
{
	Incoming = 0.f;
	Outgoing = 0.f;

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
	RPCs = 0;
	Delay = 0;

	if (const UNetConnection* NetConnection = ToolboxHelpers::GetNetConnection(WorldContextObject))
	{
		RPCs = NetConnection->TotalDelayedRPCs;
		Delay = NetConnection->TotalDelayedRPCsFrameCount;
	}
}
