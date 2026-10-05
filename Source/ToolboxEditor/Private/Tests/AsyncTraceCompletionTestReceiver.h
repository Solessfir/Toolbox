// Copyright Solessfir. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "AsyncTraceCompletionTestReceiver.generated.h"

UCLASS()
class UAsyncTraceCompletionTestReceiver : public UObject
{
    GENERATED_BODY()

public:
    int32 CompletionCount = 0;
    bool bHit = true;
    FHitResult OutHit;
    TArray<FHitResult> OutHits;

    UFUNCTION()
    void ReceiveSingle(bool bInHit, const FHitResult& InHit)
    {
        ++CompletionCount;
        bHit = bInHit;
        OutHit = InHit;
    }

    UFUNCTION()
    void ReceiveMulti(bool bInHit, const TArray<FHitResult>& InHits)
    {
        ++CompletionCount;
        bHit = bInHit;
        OutHits = InHits;
    }

    UFUNCTION()
    void ReceiveTest(bool bInHit)
    {
        ++CompletionCount;
        bHit = bInHit;
    }
};
