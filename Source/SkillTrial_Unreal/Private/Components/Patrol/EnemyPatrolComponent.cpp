// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Patrol/EnemyPatrolComponent.h"
#include "Enemy/Enemy.h"
#include "Helpers/VectorHelpers.h"
#include "Components/State/EnemyPatternStateComponent.h"
#include "Components/Movement/EnemyMovementController.h"
#include "Components/PawnSensing/EnemyPawnSensingController.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UEnemyPatrolComponent::UEnemyPatrolComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyPatrolComponent::BeginPlay()
{
	Super::BeginPlay();
	SetOwner();
	SelectNextPatrolPoint();
	SubscribeToPatternState();
	SubscribeToPawnSensing();
}

void UEnemyPatrolComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnsubscribeToPawnSensing();
	UnsubscribeToPatternState();
	Super::EndPlay(EndPlayReason);
}

void UEnemyPatrolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdatePatrol();
}

void UEnemyPatrolComponent::SubscribeToPatternState()
{
	if (!_owner)
		return;

	auto patternState = _owner->GetComponentByClass<UEnemyPatternStateComponent>();
	if (!patternState)
		return;

	patternState->GetOnPatternStateChange().AddUObject(this, &UEnemyPatrolComponent::HandlePatternStateChanged);
}

void UEnemyPatrolComponent::HandlePatternStateChanged(EEnemyPatternState patternState)
{
	auto shuoldBePatrol = patternState == EEnemyPatternState::Patrol;
	SetPatrolTickEnabled(shuoldBePatrol);
	
	if (shuoldBePatrol)
		StartPatrol();
	else
		StopPatrol();
}

void UEnemyPatrolComponent::UnsubscribeToPatternState()
{
	if (!_owner)
		return;

	auto patternState = _owner->GetComponentByClass<UEnemyPatternStateComponent>();
	if (!patternState)
		return;

	patternState->GetOnPatternStateChange().RemoveAll(this);
}

void UEnemyPatrolComponent::SelectNextPatrolPoint()
{
	const int32 count = _patrolTargetPoints.Num();
	if (count <= 1)
		return;

	const int32 currentIndex = _patrolTargetPoints.Find(_patrolTargetPoint);
	int32 newIndex = FMath::RandRange(0, count - 1);
	if (newIndex == currentIndex)
		newIndex = (newIndex + 1) % count;

	_patrolTargetPoint = _patrolTargetPoints[newIndex];
}

void UEnemyPatrolComponent::StartPatrol()
{
	UpdateIsAtPatrolPoint(false);
	StartMoveToTargetPatrolPoint();
}

void UEnemyPatrolComponent::StopPatrol()
{
	UpdateIsAtPatrolPoint(false);
	ClearPatrolDelayTimer();
}

void UEnemyPatrolComponent::UpdatePatrol()
{
	if (_isAtPatrolPoint || !_patrolTargetPoint || !_owner)
		return;

	if (!VectorHelpers::IsTargetInRange(_patrolTargetPoint, _owner, _patrolRadius))
		return;

	UpdateIsAtPatrolPoint(true);
	StartPatrolDelayTimer();
}

void UEnemyPatrolComponent::StartMoveToTargetPatrolPoint()
{
	if (!_owner)
		return;

	auto movement = _owner->GetComponentByClass<UEnemyMovementController>();
	if (!movement)
		return;

	if (!_patrolTargetPoint)
		SelectNextPatrolPoint();

	movement->MoveToTargetActor(_patrolTargetPoint);
}

void UEnemyPatrolComponent::StartPatrolDelayTimer()
{
	UWorld* world = GetWorld();
	if (!world || _patrolDelayTimer.IsValid())
		return;

	world->GetTimerManager().SetTimer(_patrolDelayTimer, this, &UEnemyPatrolComponent::HandlePatrolTimerFinished, GetPatrolDelay());
}

void UEnemyPatrolComponent::ClearPatrolDelayTimer()
{
	UWorld* world = GetWorld();
	if (!world)
		return;

	world->GetTimerManager().ClearTimer(_patrolDelayTimer);
}

void UEnemyPatrolComponent::HandlePatrolTimerFinished()
{
	SelectNextPatrolPoint();
	UpdateIsAtPatrolPoint(false);
	StartMoveToTargetPatrolPoint();
}

const float UEnemyPatrolComponent::GetPatrolDelay()
{
	return FMath::RandRange(_patrolMinDelay, _patrolMaxDelay);
}

void UEnemyPatrolComponent::SubscribeToPawnSensing()
{
	if (!_owner)
		return;

	auto pawnSensing = _owner->GetComponentByClass<UEnemyPawnSensingController>();
	if (!pawnSensing)
		return;

	pawnSensing->GetOnTrialCharacterDetected().AddUObject(this, &UEnemyPatrolComponent::HandleTrialCharacterDetected);
}

void UEnemyPatrolComponent::HandleTrialCharacterDetected(class ATrialCharacter* detectedCharacter)
{
	RequestChangeToChase();
}

void UEnemyPatrolComponent::UnsubscribeToPawnSensing()
{
	if (!_owner)
		return;

	auto pawnSensing = _owner->GetComponentByClass<UEnemyPawnSensingController>();
	if (!pawnSensing)
		return;

	pawnSensing->GetOnTrialCharacterDetected().RemoveAll(this);
}

void UEnemyPatrolComponent::RequestChangeToChase()
{
	if (!_owner)
		return;

	auto patternState = _owner->GetComponentByClass<UEnemyPatternStateComponent>();
	if (!patternState)
		return;

	patternState->UpdatePattenState(EEnemyPatternState::Chase);
}