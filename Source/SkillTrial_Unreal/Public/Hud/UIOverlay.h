// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIOverlay.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SKILLTRIAL_UNREAL_API UUIOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthBarPercent(float percent);
	void SetHealthValue(float current, float max);
private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthValue;

public:
	void SetStaminaBarPercent(float percent);
	void SetStaminaValue(float current, float max);
private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StaminaValue;

public:
	void SetCoinValue(int32 coin);
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CoinValue;
};
