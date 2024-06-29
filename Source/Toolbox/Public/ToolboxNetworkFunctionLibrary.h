// Copyright Solessfir. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToolboxNetworkFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class EToolboxConnectionState : uint8
{
	Invalid,	// Connection is invalid, possibly uninitialized
	Closed,		// Connection permanently closed
	Pending,	// Connection is awaiting connection
	Open,		// Connection is open
};

UCLASS()
class TOOLBOX_API UToolboxNetworkFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Returns the ping (in milliseconds) */
	UFUNCTION(BlueprintPure, Meta = (WorldContext = "WorldContextObject", ReturnDisplayName = "Milliseconds", Keywords = "Ping"), Category = "Toolbox|Network Library")
	static int32 GetPing(const UObject* WorldContextObject);

	/** Returns the state this connection is in */
	UFUNCTION(BlueprintPure, Meta = (WorldContext = "WorldContextObject", ReturnDisplayName = "State", Keywords = "Connection"), Category = "Toolbox|Network Library")
	static EToolboxConnectionState GetConnectionState(const UObject* WorldContextObject);

	/** Returns an average loss percentage over the previous stat periods */
	UFUNCTION(BlueprintPure, Meta = (WorldContext = "WorldContextObject", Keywords = "Packet Loss"), Category = "Toolbox|Network Library")
	static void GetPacketLoss(const UObject* WorldContextObject, float& Incoming, float& Outgoing);

	/** Returns Packets sent/received on this connection (per second) */
	UFUNCTION(BlueprintPure, Meta = (WorldContext = "WorldContextObject", Keywords = "Packet Rate"), Category = "Toolbox|Network Library")
	static void GetPacketRate(const UObject* WorldContextObject, int32& Incoming, int32& Outgoing);

	/** Returns Packets Size (in bytes) */
	UFUNCTION(BlueprintPure, Meta = (WorldContext = "WorldContextObject", Keywords = "Packet Size"), Category = "Toolbox|Network Library")
	static void GetPacketSize(const UObject* WorldContextObject, float& Incoming, float& Outgoing);

	/**
	* Returns total ACKs sent on this connection
	* Acknowledgement Packet (ACK) is a way for a Clients to acknowledge that they have received a message from the Server
	*/
	UFUNCTION(BlueprintPure, Meta = (WorldContext = "WorldContextObject", ReturnDisplayName = "Total Packets", Keywords = "ACK Size"), Category = "Toolbox|Network Library")
	static int32 GetAcknowledgedPackets(const UObject* WorldContextObject);

	/** Returns delayed RPCs and the Total Average Delay (in frames) */
	UFUNCTION(BlueprintPure, DisplayName = "Get Delayed RPCs", Meta = (WorldContext = "WorldContextObject", Keywords = "RPC Delay"), Category = "Toolbox|Network Library")
	static void GetDelayedRPCs(const UObject* WorldContextObject, int32& RPCs, int32& Delay);
};
