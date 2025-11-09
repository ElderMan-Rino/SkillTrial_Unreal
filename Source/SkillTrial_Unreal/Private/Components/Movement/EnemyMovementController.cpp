// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Movement/EnemyMovementController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "AITypes.h"
#include "Navigation/PathFollowingComponent.h"

void UEnemyMovementController::BeginPlay()
{
	Super::BeginPlay();
	SetAiController();
}

void UEnemyMovementController::Setup(ACharacter* owner)
{
	Super::Setup(owner);
	SetupMovementValues(owner);
}

void UEnemyMovementController::SetupMovementValues(ACharacter* owner)
{
	auto movement = owner->GetCharacterMovement();
	movement->bOrientRotationToMovement = false;
}

void UEnemyMovementController::UpdateMovementWalkSpeed(float targetSpeed)
{
	if (!_owner)
		return;

	_owner->GetCharacterMovement()->MaxWalkSpeed = targetSpeed;
}

void UEnemyMovementController::SetAiController()
{
	_aiController = Cast<AAIController>(_owner->GetController());
}

void UEnemyMovementController::MoveToTargetActor(const AActor* target)
{
	if (!_aiController)
		return;

	FAIMoveRequest moveRequest;
	moveRequest.SetGoalActor(target);
	moveRequest.SetAcceptanceRadius(_acceptanceRadius);
	_aiController->MoveTo(moveRequest);
}

bool UEnemyMovementController::IsMovingToTarget() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
		return false;

	AAIController* AIController = Cast<AAIController>(OwnerPawn->GetController());
	if (!AIController)
		return false;

	EPathFollowingStatus::Type MoveStatus = AIController->GetMoveStatus();
	return MoveStatus == EPathFollowingStatus::Moving;
}