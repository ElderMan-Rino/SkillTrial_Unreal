// Fill out your copyright notice in the Description page of Project Settings.


#include "Hud/UIOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UUIOverlay::SetHealthBarPercent(float percent)
{
	if (!HealthBar)
		return;
	HealthBar->SetPercent(percent);
}

void UUIOverlay::SetHealthValue(float current, float max)
{
	if (!HealthValue)
		return;
	
	const FString healthString = FString::Printf(TEXT("%d/%d"), (int32)current, (int32)max);
	const FText healthText = FText::FromString(healthString);
	HealthValue->SetText(healthText);
}

void UUIOverlay::SetStaminaBarPercent(float percent)
{
	if (!StaminaBar)
		return;

	StaminaBar->SetPercent(percent);
}

void UUIOverlay::SetStaminaValue(float current, float max)
{
	if (!StaminaValue)
		return;

	const FString staminaString = FString::Printf(TEXT("%d/%d"), (int32)current, (int32)max);
	const FText staminaText = FText::FromString(staminaString);
	StaminaValue->SetText(staminaText);
}

void UUIOverlay::SetCoinValue(int32 coin)
{
	if (!CoinValue)
		return;

	const FString goldString = FString::Printf(TEXT("%d"), (int32)coin);
	const FText goldText = FText::FromString(goldString);
	CoinValue->SetText(goldText);
}
