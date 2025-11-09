// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FORCEINLINE float UAttributeComponent::GetHealthPercent() const
{
	// Unreal에서는 FMath::IsNearlyZero를 쓰는 게 일반적이지만, 분기 예측 효율 측면에서는 단순 비교가 더 빠름
	if (_maxHealth <= 0.0001f)
		return 0.0f;
	const float percent = _health / _maxHealth;
	return FMath::Clamp(percent, 0.0f, 1.0f);
}

