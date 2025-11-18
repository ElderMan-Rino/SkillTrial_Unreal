// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class SKILLTRIAL_UNREAL_API UDataSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	
	void SetupDatas();

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FString GetCurrentGameVersion() { return _gameVersion; }
private:
	FString _gameVersion = "ver.0.0.0.0";
	void SetupGameVersion();

};
