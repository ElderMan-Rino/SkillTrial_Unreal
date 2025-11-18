// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameVersion.generated.h"

/**
 * 
 */
UCLASS()
class SKILLTRIAL_UNREAL_API UGameVersion : public UDataAsset
{
	GENERATED_BODY()

public:
	FORCEINLINE FName GameVersion() { return _gameVersion; }
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName _gameVersion = "a.0.0.0.0.ver";
};
