// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Movement/CharacterMovementController.h"
#include "Components/Attribute/CharacterAttributeComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UCharacterMovementController::UCharacterMovementController()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterMovementController::Setup(ACharacter* owner)
{
    check(owner);
	SetupUseControllerValues(owner);
}

void UCharacterMovementController::BeginPlay()
{
	Super::BeginPlay();
	SetOwner();
	SubscribeToAttribute();
}

void UCharacterMovementController::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	UnsubscribeToAttribute();
	Super::EndPlay(endPlayReason);
}

void UCharacterMovementController::SetupUseControllerValues(ACharacter* owner)
{
	owner->bUseControllerRotationPitch = false;
	owner->bUseControllerRotationYaw = false;
	owner->bUseControllerRotationRoll = false;
}

void UCharacterMovementController::UpdateMovementWalkSpeed(float targetSpeed)
{
	_owner->GetCharacterMovement()->MaxWalkSpeed = targetSpeed;
}

void UCharacterMovementController::SubscribeToAttribute()
{
	if (!_owner)
		return;

	auto attribute = _owner->GetComponentByClass<UCharacterAttributeComponent>();
	if (!attribute)
		return;

	attribute->GetOnConditionChanged().AddUObject(this, &UCharacterMovementController::HandleConditionChanged);
}

void UCharacterMovementController::HandleConditionChanged(ECharacterCondition characterCondition)
{
	auto movementFlag = (uint32)_movementRestriction;
	const uint32 deadFlag = (uint32)ECharacterMovementRestriction::Dead;
	switch (characterCondition)
	{
	case ECharacterCondition::Dead:
		movementFlag |= deadFlag;
		break;
	default:
		movementFlag &= ~deadFlag;
		break;
	}
	UpdateMovementRestriction((ECharacterMovementRestriction)deadFlag);
}

void UCharacterMovementController::UnsubscribeToAttribute()
{
	if (!_owner)
		return;

	auto attribute = _owner->GetComponentByClass<UCharacterAttributeComponent>();
	if (!attribute)
		return;

	attribute->GetOnConditionChanged().RemoveAll(this);
}
