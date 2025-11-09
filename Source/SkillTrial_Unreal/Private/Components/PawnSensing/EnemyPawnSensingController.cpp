// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PawnSensing/EnemyPawnSensingController.h"
#include "Components/State/EnemyPatternStateComponent.h"
#include "Components/Attribute/CharacterAttributeComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Characters/TrialCharacter.h"
#include "Enemy/Enemy.h"


UEnemyPawnSensingController::UEnemyPawnSensingController()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UEnemyPawnSensingController::BeginPlay()
{
	Super::BeginPlay();
	SetOwner();
	SubscribeToSeePawn();
	SubscribeToPatternState();
}

void UEnemyPawnSensingController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnsubscribeToSeePawn();
	UnsubscribeToPatternState();
	Super::EndPlay(EndPlayReason);
}

void UEnemyPawnSensingController::Setup(AActor* owner, TArray<UActorComponent*>& registeredSubComponents)
{
	check(owner);
	CreatePawnSensing(owner, registeredSubComponents);
	SetupPawnSensing();
}

void UEnemyPawnSensingController::CreatePawnSensing(AActor* owner, TArray<UActorComponent*>& registeredSubComponents)
{
	_pawnSensing = owner->CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	registeredSubComponents.Add(_pawnSensing);
}

void UEnemyPawnSensingController::SetupPawnSensing()
{
	_pawnSensing->HearingThreshold = 500.f;
	_pawnSensing->SightRadius = 1500.f;
	_pawnSensing->SetPeripheralVisionAngle(50.f);
}

void UEnemyPawnSensingController::SubscribeToSeePawn()
{
	if (!_pawnSensing)
		return;

	_pawnSensing->OnSeePawn.AddDynamic(this, &UEnemyPawnSensingController::OnPawnSeen);
}

void UEnemyPawnSensingController::UnsubscribeToSeePawn()
{
	if (!_pawnSensing)
		return;

	_pawnSensing->OnSeePawn.RemoveAll(this);
}

void UEnemyPawnSensingController::OnPawnSeen(APawn* seenPawn)
{
	if (!_isSensingEnabled)
		return;

	auto trialCharacter = Cast<ATrialCharacter>(seenPawn);
	if (!trialCharacter)
		return;

	BroadcastTrialCharacterDetected(trialCharacter);
}

void UEnemyPawnSensingController::SubscribeToPatternState()
{
	if (!_owner)
		return;

	auto patternState = _owner->GetComponentByClass<UEnemyPatternStateComponent>();
	if (!patternState)
		return;

	patternState->GetOnPatternPostStateChange().AddUObject(this, &UEnemyPawnSensingController::HandlePatternStateChange);
}

void UEnemyPawnSensingController::HandlePatternStateChange(EEnemyPatternState enemyPatternState)
{
	SetIsSensingEnabled(enemyPatternState == EEnemyPatternState::Patrol);
}

void UEnemyPawnSensingController::UnsubscribeToPatternState()
{
	if (!_owner)
		return;

	auto patternState = _owner->GetComponentByClass<UEnemyPatternStateComponent>();
	if (!patternState)
		return;

	patternState->GetOnPatternStateChange().RemoveAll(this);
}

void UEnemyPawnSensingController::BroadcastTrialCharacterDetected(ATrialCharacter* detectedCharacter)
{
	if (!_owner)
		return;

	auto attribute = _owner->GetComponentByClass<UCharacterAttributeComponent>();
	if (!attribute)
		return;

	if (!attribute->IsTargetCondition(ECharacterCondition::Alive))
		return;

	_onTrialCharacterDetected.Broadcast(detectedCharacter);
	_onTrialCharacterPostDetected.Broadcast(detectedCharacter);
}
