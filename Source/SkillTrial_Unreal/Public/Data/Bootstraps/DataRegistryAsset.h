// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UObject/SoftObjectPtr.h"
#include "DataAssetPathList.h"
#include "DataRegistryAsset.generated.h"

/**
 *
 */
UCLASS()
class SKILLTRIAL_UNREAL_API UDataRegistryAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Registry")
	TMap<FName, FDataAssetPathList> DataAssetRegistryMap;
};
