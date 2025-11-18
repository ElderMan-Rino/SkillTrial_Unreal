// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemStat.h"
#include "ItemDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SKILLTRIAL_UNREAL_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Item")
	FItemStat Stats;
	UPROPERTY(EditAnywhere, Category = "Item")
	TSoftObjectPtr<class UTexture2D> Icon;
};
