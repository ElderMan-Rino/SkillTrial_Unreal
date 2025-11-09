// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PlayerCombatComponent.h"
#include "Components/Equipment/PlayerEquipmentComponent.h"
#include "Components/Event/ActorEventPropagationComponent.h"
#include "Components/Attribute/CharacterAttributeComponent.h"
#include "Components/Inputs/ActionInputController.h"
#include "Enums/CharacterCondition.h"
#include "GameFramework/Character.h"

void UPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	SubscribeToActionInput();
	SubscribeToEquipment();
	SubscribeToAttribute();
}

void UPlayerCombatComponent::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	UnsubscribeToAttribute();
	UnubscribeToEquipment();
	UnsubscribeToActionInput();
	Super::EndPlay(endPlayReason);
}

void UPlayerCombatComponent::SubscribeToActionInput()
{
	if (!_owner)
		return;

	auto actionController = _owner->FindComponentByClass<UActionInputController>();
	if (!actionController)
		return;
	
	actionController->GetOnActionInput().AddUObject(this, &UPlayerCombatComponent::HandleActionInput);
}

void UPlayerCombatComponent::HandleActionInput(EActionInputType actionInputType)
{
	if (actionInputType != EActionInputType::Attack)
		return;
	
	HandleActionAttack();
}

void UPlayerCombatComponent::HandleActionAttack()
{
	if (!CanControlAttack())
		return;
	
	PlayAttackMontage();
}

void UPlayerCombatComponent::UnsubscribeToActionInput()
{
	if (!_owner)
		return;

	auto actionController = _owner->FindComponentByClass<UActionInputController>();
	if (!actionController)
		return;

	actionController->GetOnActionInput().RemoveAll(this);
}

void UPlayerCombatComponent::SubscribeToEquipment()
{
	if (!_owner)
		return;

	auto equipment = _owner->FindComponentByClass<UPlayerEquipmentComponent>();
	if (!equipment)
		return;

	equipment->GetOnEquipStateChange().AddUObject(this, &UPlayerCombatComponent::HandleEquipStateChange);
}

void UPlayerCombatComponent::HandleEquipStateChange(ECharacterEquipState equipState)
{
	auto attackFlag = (uint32)GetAttackRestriction();
	const uint32 unequipFlag = (uint32)ECharacterAttackRestriction::Unequip;
	switch (equipState)
	{
	case ECharacterEquipState::ECS_Unequipped:
		attackFlag |= unequipFlag;
		break;
	case ECharacterEquipState::ECS_EquippedOneHanded:
	case ECharacterEquipState::ECS_EquippedTwoHande:
		attackFlag &= ~unequipFlag;
		break;
	default:
		break;
	}
	UpdateAttackRestriction((ECharacterAttackRestriction)attackFlag);
}

void UPlayerCombatComponent::UnubscribeToEquipment()
{
	if (!_owner)
		return;

	auto equipment = _owner->FindComponentByClass<UPlayerEquipmentComponent>();
	if (!equipment)
		return;

	equipment->GetOnEquipStateChange().RemoveAll(this);
}

void UPlayerCombatComponent::SubscribeToEventPropagation()
{
	Super::SubscribeToEventPropagation();

	if (!_owner)
		return;

	auto eventPropagation = _owner->GetComponentByClass<UActorEventPropagationComponent>();
	if (!eventPropagation)
		return;

	eventPropagation->GetOnMovementModeChanged().AddUObject(this, &UPlayerCombatComponent::HandleMovementModeChanged);
}

void UPlayerCombatComponent::HandleMovementModeChanged(const EMovementMode prevMode, const EMovementMode curMode)
{
	if (curMode == EMovementMode::MOVE_Falling)
		UpdateAttackRestriction(EMovementMode::MOVE_Falling);
	else if (prevMode == EMovementMode::MOVE_Falling)
		UpdateAttackRestriction(curMode);
}

void UPlayerCombatComponent::UnsubscribeToEventPropagation()
{
	Super::UnsubscribeToEventPropagation();

	if (!_owner)
		return;

	auto eventPropagation = _owner->GetComponentByClass<UActorEventPropagationComponent>();
	if (!eventPropagation)
		return;

	eventPropagation->GetOnMovementModeChanged().RemoveAll(this);
}

void UPlayerCombatComponent::HandleHitEvent(const FVector& hitPoint, AActor* hitter)
{
	PlayTargetSFX(_hitSFX, hitPoint);
	PlayTargetVFX(_bloodVFX, hitPoint);

	auto ownerCondition = GetOwnerCondition();
	if (ownerCondition != ECharacterCondition::Alive)
		return;

	if (!IsPlayingAnimation())
		return;

	PlayHitMontage(hitter ? hitter->GetActorLocation() : hitPoint);
}

bool UPlayerCombatComponent::IsPlayingAnimation()
{
	const uint32 DisallowedMask = (uint32)ECharacterAttackRestriction::AniActivity | (uint32)ECharacterAttackRestriction::Falling;
	return !((uint32)_attackRestriction & DisallowedMask);
}

void UPlayerCombatComponent::SubscribeToAttribute()
{
	if (!_owner)
		return;

	auto attribute = _owner->GetComponentByClass<UCharacterAttributeComponent>();
	if (!attribute)
		return;

	attribute->GetOnConditionChanged().AddUObject(this, &UPlayerCombatComponent::HandleConditionChanged);
}

void UPlayerCombatComponent::HandleConditionChanged(ECharacterCondition characterCondition)
{
	auto attackFlag = (uint32)GetAttackRestriction();
	const uint32 deadFlag = (uint32)ECharacterAttackRestriction::Dead;
	switch (characterCondition)
	{
	case ECharacterCondition::Dead:
		attackFlag |= deadFlag;
		break;
	default:
		attackFlag &= ~deadFlag;
		break;
	}
	UpdateAttackRestriction((ECharacterAttackRestriction)deadFlag);
}

void UPlayerCombatComponent::UnsubscribeToAttribute()
{
	if (!_owner)
		return;

	auto attribute = _owner->GetComponentByClass<UCharacterAttributeComponent>();
	if (!attribute)
		return;

	attribute->GetOnConditionChanged().RemoveAll(this);
}