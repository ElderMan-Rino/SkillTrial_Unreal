// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Equipment/PlayerEquipmentComponent.h"
#include "Characters/TrialCharacter.h"
#include "Components/Inputs/ActionInputController.h"
#include "Items/Weapons/Weapon.h"
#include "Components/Animation/AnimPlayerComponent.h"

void UPlayerEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	SubscribeToActionInput();
}

void UPlayerEquipmentComponent::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	UnsubscribeToActionInput();
	Super::EndPlay(endPlayReason);
}

void UPlayerEquipmentComponent::SubscribeToActionInput()
{
	if (!_owner)
		return;

	auto actionController = _owner->FindComponentByClass<UActionInputController>();
	if (!actionController)
		return;

	actionController->GetOnActionInput().AddUObject(this, &UPlayerEquipmentComponent::HandleActionInput);
}

void UPlayerEquipmentComponent::UnsubscribeToActionInput()
{
	if (!_owner)
		return;

	auto actionController = _owner->FindComponentByClass<UActionInputController>();
	if (!actionController)
		return;

	actionController->GetOnActionInput().RemoveAll(this);
}

void UPlayerEquipmentComponent::EquipWeapon(AWeapon* targetWeapon)
{
	SetEquippedWeapon(targetWeapon);
	AttachWeaponToCharacter(targetWeapon, FName("hand_rSocket"));
	UpdateEquipState(ECharacterEquipState::ECS_EquippedOneHanded);
}

void UPlayerEquipmentComponent::HandleDrawWeapon()
{
	if (!_equippedWeapon)
		return;

	_equippedWeapon->AttachToSocket(_owner->GetMesh(), FName("hand_rSocket"));
	_equippedWeapon->PlaySlashSFX();
}

void UPlayerEquipmentComponent::HandleHolsterWeapon()
{
	if (!_equippedWeapon)
		return;

	_equippedWeapon->AttachToSocket(_owner->GetMesh(), FName("spine_Socket"));
}

void UPlayerEquipmentComponent::HandleActionInput(EActionInputType actionInputType)
{
	if (actionInputType != EActionInputType::EKeyPressed)
		return;

	HandleEquippedWeapon();
}

void UPlayerEquipmentComponent::UpdateEquipState(ECharacterEquipState targetState)
{
	_equipState = targetState;
	BroadcastEquipStateChange();
}

void UPlayerEquipmentComponent::HandleEquippedWeapon()
{
	/*if (IsAnimationActiviting())
	return;*/

	if (!_equippedWeapon)
		return;

	switch (_equipState)
	{
	case ECharacterEquipState::ECS_Unequipped:
		DrawWeapon();
		break;
	case ECharacterEquipState::ECS_EquippedOneHanded:
	case ECharacterEquipState::ECS_EquippedTwoHande:
		HolsterWeapon();
		break;
	}
}

void UPlayerEquipmentComponent::DrawWeapon()
{
	PlayMontage(FName("Equip"));
	UpdateEquipState(ECharacterEquipState::ECS_EquippedOneHanded);
}

void UPlayerEquipmentComponent::HolsterWeapon()
{
	PlayMontage(FName("Unequip"));
	UpdateEquipState(ECharacterEquipState::ECS_Unequipped);
}

void UPlayerEquipmentComponent::PlayMontage(FName sectionName)
{
	auto animPlayer = _owner->GetComponentByClass<UAnimPlayerComponent>();
	if (!animPlayer)
		return;
	animPlayer->PlayTargetMontage(_equipMontage, sectionName);
}