// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Dead/CharacterDeadComponent.h"
#include "GameFramework/Character.h"
#include "Components/Attribute/CharacterAttributeComponent.h"
#include "Components/Animation/AnimPlayerComponent.h"

// Sets default values for this component's properties
UCharacterDeadComponent::UCharacterDeadComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterDeadComponent::BeginPlay()
{
	Super::BeginPlay();
	SetOwner();
	SubscribeToAttribute();
}

void UCharacterDeadComponent::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	UnsubscribeToAttribute();
	Super::EndPlay(endPlayReason);
}


void UCharacterDeadComponent::SubscribeToAttribute()
{
	if (!_owner)
		return;

	auto attribute = _owner->GetComponentByClass<UCharacterAttributeComponent>();
	if (!attribute)
		return;

	attribute->GetOnConditionChanged().AddUObject(this, &UCharacterDeadComponent::HandleConditionChanged);
}

void UCharacterDeadComponent::HandleConditionChanged(ECharacterCondition changedCondition)
{
	if (changedCondition != ECharacterCondition::Dead)
		return;

	PlayDeathMontage();
}

void UCharacterDeadComponent::UnsubscribeToAttribute()
{
	if (!_owner)
		return;

	auto attribute = _owner->GetComponentByClass<UCharacterAttributeComponent>();
	if (!attribute)
		return;

	attribute->GetOnConditionChanged().RemoveAll(this);
}

void UCharacterDeadComponent::PlayDeathMontage()
{
	if (!_deathMontage || !_owner)
		return;

	auto animPlayer = _owner->GetComponentByClass<UAnimPlayerComponent>();
	if (!animPlayer)
		return;

	SetDeathAniState();
	animPlayer->PlayTargetMontage(_deathMontage, GetDeathMontageSection());
}

FName UCharacterDeadComponent::GetDeathMontageSection()
{
	if (_sectionNames.Contains(_deadAniState))
		return _sectionNames[_deadAniState];
	return FName("Death0");
}

void UCharacterDeadComponent::SetDeathAniState()
{
	int32 selection = FMath::RandRange(0, 3);
	switch (selection)
	{
	case 0:
		_deadAniState = EKnightDeadAniState::DeadOne;
		break;
	case 1:
		_deadAniState = EKnightDeadAniState::DeadTwo;
		break;
	case 2:
		_deadAniState = EKnightDeadAniState::DeadThree;
		break;
	default:
		_deadAniState = EKnightDeadAniState::DeadOne;
		break;
	}
}