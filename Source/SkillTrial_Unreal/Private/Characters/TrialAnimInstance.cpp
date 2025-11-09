// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/TrialAnimInstance.h"
#include "Characters/TrialCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enums/CharacterEquipState.h"
#include "Components/Equipment/PlayerEquipmentComponent.h"

void UTrialAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	InitializeTrialCharacter();
	InitializeTrialCharacterMovement();
	InitializeEquipment();
}

void UTrialAnimInstance::InitializeTrialCharacter()
{
	_trialCharacter = Cast<ATrialCharacter>(TryGetPawnOwner());
}
void UTrialAnimInstance::InitializeTrialCharacterMovement()
{
	if (!_trialCharacter)
		return;
	_trialCharacterMovement = _trialCharacter->GetCharacterMovement();
}

void UTrialAnimInstance::InitializeEquipment()
{
	if (!_trialCharacter)
		return;
	_equipment = _trialCharacter->GetComponentByClass<UPlayerEquipmentComponent>();
}

void UTrialAnimInstance::NativeUpdateAnimation(float deltaTime)
{
	Super::NativeUpdateAnimation(deltaTime);
	if (!_trialCharacterMovement)
		return;

	UpdateGroundSpeed();
	UpdateFalling();
	UpdateEquipState();
}
void UTrialAnimInstance::UpdateGroundSpeed()
{
	_groundSpeed = UKismetMathLibrary::VSizeXY(_trialCharacterMovement->Velocity);
}

void UTrialAnimInstance::UpdateFalling()
{
	_isFalling = _trialCharacterMovement->IsFalling();
}

void UTrialAnimInstance::UpdateEquipState()
{
	if (!_equipment)
		return;
	_equipState = _equipment->GetEquipState();
}

