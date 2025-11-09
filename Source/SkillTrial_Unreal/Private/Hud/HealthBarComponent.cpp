// Fill out your copyright notice in the Description page of Project Settings.


#include "Hud/HealthBarComponent.h"
#include "Hud/HealthBar.h"
#include "Kismet/GameplayStatics.h"

UHealthBarComponent::UHealthBarComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthBarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	BillboardWidget();
}

void UHealthBarComponent::UpdateHealthPer(float percent)
{
	UHealthBar* healthBarWidget = GetHealthBarWidget();
	if (!healthBarWidget)
		return;
	healthBarWidget->UpdateHealthBarProgress(percent);
}

UHealthBar* UHealthBarComponent::GetHealthBarWidget() 
{
	if (_healthBarWidget)
		return _healthBarWidget;

	UUserWidget* userWidget = GetUserWidgetObject();
	if (!userWidget)
		return nullptr;

	return _healthBarWidget = Cast<UHealthBar>(userWidget);
}

void UHealthBarComponent::BillboardWidget()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController)
		return;

	FRotator CameraRotation = playerController->GetControlRotation();
	FRotator NewRotation(0.f, CameraRotation.Yaw + 180.f, 0.f);
	SetWorldRotation(NewRotation);
}
