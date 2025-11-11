// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Movement/PlayerMovementController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/TrialCharacter.h"
#include "Components/Inputs/AxisInputController.h"
#include "Components/Inputs/ActionInputController.h"
#include "Components/State/PlayerStateComponent.h"
#include "Components/Animation/AniActivityComponent.h"
#include "Components/Animation/AnimPlayerComponent.h"
#include "Components/Attribute/CharacterAttributeComponent.h"
#include "Enums/CharacterMovementRestriction.h"

void UPlayerMovementController::BeginPlay()
{
	Super::BeginPlay();

	SetupAxisMap();
	SubscribeToAxisInput();
	SubscribeToActionInput();
	SubscribeToAniActivityChange();
}

void UPlayerMovementController::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	UnsubscribeToAniActivityChange();
	UnubscribeToActionInput();
	UnsubscribeToAxisInput();
	Super::EndPlay(endPlayReason);
}

void UPlayerMovementController::Setup(ACharacter* owner)
{
	Super::Setup(owner);

	SetupUseControllerValues(owner);
	SetupMovementValues(owner);
}

void UPlayerMovementController::SetupMovementValues(ACharacter* owner)
{
	auto movement = owner->GetCharacterMovement();
	movement->bOrientRotationToMovement = true;
	movement->RotationRate = FRotator(0.f, 400.f, 0.f);
}

void UPlayerMovementController::SubscribeToAxisInput()
{
	if (!_owner) 
		return;

	auto axisController = _owner->FindComponentByClass<UAxisInputController>();
	if (!axisController) 
		return;

	axisController->GetOnAxisInput().AddUObject(this, &UPlayerMovementController::HandleAxisInput);
}

void UPlayerMovementController::SetupAxisMap()
{
	_axisMap.Empty(); // 중복 초기화 방지
	_axisMap.Add(EAxisInputType::MoveForward, static_cast<FAxisFunc>(&UPlayerMovementController::HandleMoveForward));
	_axisMap.Add(EAxisInputType::MoveRight, static_cast<FAxisFunc>(&UPlayerMovementController::HandleMoveRight));
	_axisMap.Add(EAxisInputType::Turn, static_cast<FAxisFunc>(&UPlayerMovementController::HandleTurn));
	_axisMap.Add(EAxisInputType::LookUp, static_cast<FAxisFunc>(&UPlayerMovementController::HandleLookUp));
}

void UPlayerMovementController::HandleAxisInput(EAxisInputType axisInputType, float value)
{
	if (auto FuncPtr = _axisMap.Find(axisInputType))
		(this->* * FuncPtr)(value); 
}

void UPlayerMovementController::HandleMoveForward(const float value)
{
	UpdateForwardInput(value);
	if (value == 0.f)
		return;

	if (!_owner || !_owner->GetController())
		return;

	if (!CanControlMovement())
		return;

	_owner->AddMovementInput(GetControlDirection(EAxis::X), value);
}

void UPlayerMovementController::HandleMoveRight(const float value)
{
	UpdateRightInput(value);
	if (value == 0.f)
		return;

	if (!_owner || !_owner->GetController())
		return;

	if (!CanControlMovement())
		return;

	_owner->AddMovementInput(GetControlDirection(EAxis::Y), value);
}

void UPlayerMovementController::HandleTurn(const float value)
{
	_owner->AddControllerYawInput(value);
}

void UPlayerMovementController::HandleLookUp(const float value)
{
	_owner->AddControllerPitchInput(value);
}

const FVector UPlayerMovementController::GetControlDirection(EAxis::Type targetAxis)
{
	const FRotator controlRotation = _owner->GetControlRotation();
	const FRotator yawRoation(0.f, controlRotation.Yaw, 0.f);
	return FRotationMatrix(yawRoation).GetUnitAxis(targetAxis);
}

void UPlayerMovementController::UnsubscribeToAxisInput()
{
	if (!_owner)
		return;

	auto axisController = _owner->FindComponentByClass<UAxisInputController>();
	if (!axisController)
		return;

	axisController->GetOnAxisInput().RemoveAll(this);
}

void UPlayerMovementController::ResetDodgeRotaion()
{
	FRotator CurrentRotation = _owner->GetActorRotation();
	FRotator TargetRotation = CurrentRotation;
	TargetRotation.Yaw += 50.0f;
	_owner->SetActorRotation(TargetRotation);
}

void UPlayerMovementController::SubscribeToActionInput()
{
	if (!_owner)
		return;

	auto actionController = _owner->FindComponentByClass<UActionInputController>();
	if (!actionController)
		return;

	actionController->GetOnActionInput().AddUObject(this, &UPlayerMovementController::HandleActionInput);
}

void UPlayerMovementController::HandleActionInput(EActionInputType actionInputType)
{
	if (actionInputType != EActionInputType::Jump)
		return;
	
	if (IsDodgeAction())
		HandleDodge();
	else
		HandleJump();
}

void UPlayerMovementController::HandleJump()
{
	if (!_owner || !_owner->GetController())
		return;

	if (!CanControlMovement())
		return;

	_owner->Jump();
}

void UPlayerMovementController::HandleDodge()
{
	if (!_owner || !_owner->GetController() || !_dodgeMontage)
		return;

	if (!CheckDodgeEnable())
		return;

	auto animPlayer = _owner->GetComponentByClass<UAnimPlayerComponent>();
	if (!animPlayer)
		return;

	ConsumeDodgeStamina();
	//SetDodgeRotation();
	animPlayer->PlayTargetMontage(_dodgeMontage, FName("Dodge"));
}

bool UPlayerMovementController::CheckDodgeEnable()
{
	if (!CanControlMovement())
		return false;
	auto attribute = _owner->GetComponentByClass<UCharacterAttributeComponent>();
	if (!attribute)
		return false;
	return attribute->GetStamina() >= _dodgeStaminaCost;
}

void UPlayerMovementController::ConsumeDodgeStamina()
{
	auto attribute = _owner->GetComponentByClass<UCharacterAttributeComponent>();
	if (!attribute)
		return;
	
	attribute->UpdateStamina(-_dodgeStaminaCost);
}

void UPlayerMovementController::SetDodgeRotation()
{
	FRotator CurrentRotation = _owner->GetActorRotation();
	FRotator TargetRotation = CurrentRotation;
	TargetRotation.Yaw += 45.0f;
	_owner->SetActorRotation(TargetRotation);
}

void UPlayerMovementController::UnubscribeToActionInput()
{
	if (!_owner)
		return;

	auto actionController = _owner->FindComponentByClass<UActionInputController>();
	if (!actionController)
		return;

	actionController->GetOnActionInput().RemoveAll(this);
}

void UPlayerMovementController::SubscribeToAniActivityChange()
{
	if (!_owner)
		return;

	auto aniActivity = _owner->FindComponentByClass<UAniActivityComponent>();
	if (!aniActivity)
		return;

	aniActivity->GetOnAniActivityChange().AddUObject(this, &UPlayerMovementController::HandleAniActivityChange);
}

void UPlayerMovementController::HandleAniActivityChange(EAniActivityState aniActivityState)
{
	UpdateMovementRestriction(CalculateNewMovementRestriction(aniActivityState));
}

ECharacterMovementRestriction UPlayerMovementController::CalculateNewMovementRestriction(EAniActivityState aniActivityState) const
{
	auto movementFlag = (uint32)_movementRestriction;
	const uint32 aniActivity = (uint32)ECharacterMovementRestriction::AniActivity;
	switch (aniActivityState)
	{
	case EAniActivityState::EAS_Inactive:
		movementFlag &= ~aniActivity;
		break;
	case EAniActivityState::EAS_Active:
		movementFlag |= aniActivity;
		break;
	default:
		break;
	}
	return (ECharacterMovementRestriction)movementFlag;
}

void UPlayerMovementController::UnsubscribeToAniActivityChange()
{
	if (!_owner)
		return;

	auto aniActivity = _owner->FindComponentByClass<UAniActivityComponent>();
	if (!aniActivity)
		return;

	aniActivity->GetOnAniActivityChange().RemoveAll(this);
}