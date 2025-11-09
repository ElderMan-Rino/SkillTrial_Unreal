// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Attribute/CharacterAttributeComponent.h"

UCharacterAttributeComponent::UCharacterAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCharacterAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	SetOwner();
}
void UCharacterAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	HandleStaminaRecovery(DeltaTime);
}

void UCharacterAttributeComponent::UpdateCondition(ECharacterCondition targetCondition)
{
	if (_condition == targetCondition)
		return;

	_condition = targetCondition;
	BroadcastConditionChanged();
}

float UCharacterAttributeComponent::GetHealthPercent() const
{
	if (_maxHealth <= 0.0001f)
		return 0.0f;
	const float percent = _health / _maxHealth;
	return FMath::Clamp(percent, 0.0f, 1.0f);
}

void UCharacterAttributeComponent::UpdateHealth(const float value)
{
	_health = FMath::Clamp(_health + value, 0, _maxHealth);
	BroadcastHealthChanged();
	UpdateCondition(_health > 0 ? ECharacterCondition::Alive : ECharacterCondition::Dead);
}

float UCharacterAttributeComponent::GetStaminaPercent() const
{
	if (_maxStamina <= 0.0001f)
		return 0.0f;
	const float percent = _stamina / _maxStamina;
	return FMath::Clamp(percent, 0.0f, 1.0f);
}

void UCharacterAttributeComponent::UpdateStamina(const float value)
{
	_stamina = FMath::Clamp(_stamina + value, 0, _maxStamina);
	BroadcastStaminaChanged();
}

void UCharacterAttributeComponent::HandleStaminaRecovery(float DeltaTime)
{
	if (_stamina >= _maxStamina)
		return;

	const float recoveryAmount = _staminaRecoveryPerSecond * DeltaTime;
	UpdateStamina(recoveryAmount);
}