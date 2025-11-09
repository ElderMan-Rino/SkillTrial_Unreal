// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Equipment/PickEquipmentComponent.h"
#include "Characters/TrialCharacter.h"
#include "Components/Inputs/ActionInputController.h"
#include "Components/Equipment/PlayerEquipmentComponent.h"

// Sets default values for this component's properties
UPickEquipmentComponent::UPickEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPickEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	SetOwner();
	SubscribeToActionInput();
}

void UPickEquipmentComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnsubscribeToActionInput();
	Super::EndPlay(EndPlayReason);
}

void UPickEquipmentComponent::SubscribeToActionInput()
{
	if (!_owner)
		return;

	auto actionController = _owner->FindComponentByClass<UActionInputController>();
	if (!actionController)
		return;

	actionController->GetOnPostActionInput().AddUObject(this, &UPickEquipmentComponent::HandleActionInput);
}

void UPickEquipmentComponent::HandleActionInput(EActionInputType actionInputType)
{
	if (actionInputType != EActionInputType::EKeyPressed)
		return;
	HandleEKeyPressed();
}

void UPickEquipmentComponent::HandleEKeyPressed()
{
	EquipGroundItem();
}

void UPickEquipmentComponent::EquipGroundItem()
{
	if (!_groundItem)
		return;

	auto equipment = _owner->GetComponentByClass<UPlayerEquipmentComponent>();
	if (!equipment)
		return;

	auto weapon = Cast<AWeapon>(_groundItem);
	if(!weapon)
		return;

	equipment->EquipWeapon(weapon);
	SetGroundItem(nullptr);
}

void UPickEquipmentComponent::UnsubscribeToActionInput()
{
	if (!_owner)
		return;

	auto actionController = _owner->FindComponentByClass<UActionInputController>();
	if (!actionController)
		return;

	actionController->GetOnPostActionInput().RemoveAll(this);
}
