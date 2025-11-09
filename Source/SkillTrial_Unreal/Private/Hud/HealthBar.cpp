// Fill out your copyright notice in the Description page of Project Settings.


#include "Hud/HealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBar::UpdateHealthBarProgress(float percent)
{
	if (!_healthBar)
		return;

	_healthBar->SetPercent(percent);
}
void UHealthBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	InitializeHealthBar();
}
void UHealthBar::InitializeHealthBar()
{
	if (_healthBar)
		return;
	_healthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBar")));
}
