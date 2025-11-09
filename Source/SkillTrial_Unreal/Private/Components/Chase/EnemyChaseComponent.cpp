// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Chase/EnemyChaseComponent.h"
#include "Enemy/Enemy.h"
#include "Components/PawnSensing/EnemyPawnSensingController.h"
#include "Components/State/EnemyPatternStateComponent.h"
#include "Components/Movement/EnemyMovementController.h"
#include "Characters/TrialCharacter.h"
#include "Helpers/VectorHelpers.h"

// Sets default values for this component's properties
UEnemyChaseComponent::UEnemyChaseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyChaseComponent::BeginPlay()
{
	Super::BeginPlay();
	SetOwner();
	SubscribeToPatternState();
	SubscribeToPawnSensing();
}

void UEnemyChaseComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnsubscribeToPawnSensing();
	UnsubscribeToPatternState();
	Super::EndPlay(EndPlayReason);
}

void UEnemyChaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateChase();
}

void UEnemyChaseComponent::UpdateChase()
{
	if (!_owner || !_chaseTarget)
		return;

	if (VectorHelpers::IsTargetInRange(_chaseTarget, _owner, _engageRadius))
	{
		BroadcastTargetInEngageRange();
		RequestChangeToEngage();
	}
	else if (VectorHelpers::IsTargetInRange(_chaseTarget, _owner, _chaseRadius))
	{
		if(!IsChasing())
			StartChase();
	}
	else 
	{
		ClearChaseTarget();
		RequestChangeToPatrol();
	}
}

void UEnemyChaseComponent::SubscribeToPatternState()
{
	if (!_owner)
		return;

	auto patternState = _owner->GetComponentByClass<UEnemyPatternStateComponent>();
	if (!patternState)
		return;

	patternState->GetOnPatternStateChange().AddUObject(this, &UEnemyChaseComponent::HandlePatternStateChange);
}

void UEnemyChaseComponent::HandlePatternStateChange(EEnemyPatternState enemyPatternState)
{
	SetChaseTickEnabled(enemyPatternState == EEnemyPatternState::Chase);
	StartChase();
}

void UEnemyChaseComponent::UnsubscribeToPatternState()
{
	if (!_owner)
		return;

	auto patternState = _owner->GetComponentByClass<UEnemyPatternStateComponent>();
	if (!patternState)
		return;

	patternState->GetOnPatternStateChange().RemoveAll(this);
}

void UEnemyChaseComponent::SubscribeToPawnSensing()
{
	if (!_owner)
		return;

	auto pawnSensing = _owner->GetComponentByClass<UEnemyPawnSensingController>();
	if (!pawnSensing)
		return;

	pawnSensing->GetOnTrialCharacterPostDetected().AddUObject(this, &UEnemyChaseComponent::HandleTrialCharacterDetected);
}

void UEnemyChaseComponent::HandleTrialCharacterDetected(class ATrialCharacter* detectedCharacter)
{
	SetChaseTarget(detectedCharacter);
	StartChase();
}

void UEnemyChaseComponent::UnsubscribeToPawnSensing()
{
	if (!_owner)
		return;

	auto pawnSensing = _owner->GetComponentByClass<UEnemyPawnSensingController>();
	if (!pawnSensing)
		return;

	pawnSensing->GetOnTrialCharacterPostDetected().RemoveAll(this);
}

void UEnemyChaseComponent::StartChase()
{
	if (!_owner || !_chaseTarget)
		return;

	auto movement = _owner->GetComponentByClass<UEnemyMovementController>();
	if (!movement)
		return;

	movement->MoveToTargetActor(_chaseTarget);
}

bool UEnemyChaseComponent::IsChasing()
{
	if (!_owner || !_chaseTarget)
		return false;

	auto movement = _owner->GetComponentByClass<UEnemyMovementController>();
	if (!movement)
		return false;

	return movement->IsMovingToTarget();
}

void UEnemyChaseComponent::RequestChangeToEngage()
{
	if (!_owner)
		return;

	auto patternState = _owner->GetComponentByClass<UEnemyPatternStateComponent>();
	if (!patternState)
		return;

	patternState->UpdatePattenState(EEnemyPatternState::Engage);
}

void UEnemyChaseComponent::RequestChangeToPatrol()
{
	if (!_owner)
		return;

	auto patternState = _owner->GetComponentByClass<UEnemyPatternStateComponent>();
	if (!patternState)
		return;

	patternState->UpdatePattenState(EEnemyPatternState::Patrol);
}
