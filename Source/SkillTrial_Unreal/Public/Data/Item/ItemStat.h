#pragma once

#include "CoreMinimal.h"
#include "ItemStat.generated.h"

USTRUCT(BlueprintType)
struct FItemStat
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Unique Property")
    uint32 Index;
    UPROPERTY(EditAnywhere, Category = "Unique Property")
    uint32 Uid;
    UPROPERTY(EditAnywhere, Category = "Unique Property")
    FName Key;

};