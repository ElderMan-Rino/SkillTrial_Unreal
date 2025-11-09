// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

class UHealthBar;
/**
 * 
 */
UCLASS()
class SKILLTRIAL_UNREAL_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UHealthBarComponent(); // 생성자 선언 (Tick 활성화를 위해)
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void UpdateHealthPer(float percent);

private:
	UHealthBar* _healthBarWidget = nullptr;
	UHealthBar* GetHealthBarWidget();

private:
	void BillboardWidget();
};
