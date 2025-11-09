// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/State/EnemyPatternStateComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Components/PawnSensing/EnemyPawnSensingController.h"
#include "Components/Chase/EnemyChaseComponent.h"
#include "Components/Event/ActorEventPropagationComponent.h"
#include "Components/Attribute/CharacterAttributeComponent.h"
#include "Enums/CharacterCondition.h"
#include "Enemy/Enemy.h"

// Sets default values for this component's properties
UEnemyPatternStateComponent::UEnemyPatternStateComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEnemyPatternStateComponent::BeginPlay()
{
	Super::BeginPlay();
	SetOwner();
	SubscribeToAttribute();
	SubscribeToEventPropagation();
	PrepareInitialStateBroadcast();
}

void UEnemyPatternStateComponent::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	UnsubscribeToEventPropagation();
	UnsubscribeToAttribute();
	Super::EndPlay(endPlayReason);
}

void UEnemyPatternStateComponent::UpdatePattenState(EEnemyPatternState patternState)
{
	if (_patternState == patternState)
		return;

	_patternState = patternState;
	BroadcastPatternStateChange();
}

void UEnemyPatternStateComponent::BroadcastPatternStateChange()
{
	_onPatternStateChanged.Broadcast(_patternState);
	_onPatternPostStateChanged.Broadcast(_patternState);
}

void UEnemyPatternStateComponent::PrepareInitialStateBroadcast()
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UEnemyPatternStateComponent::BroadcastDelayedInitialState);
}

void UEnemyPatternStateComponent::BroadcastDelayedInitialState()
{
	BroadcastPatternStateChange();
}

void UEnemyPatternStateComponent::SubscribeToAttribute()
{
	if (!_owner)
		return;

	auto attribute = _owner->GetComponentByClass<UCharacterAttributeComponent>();
	if (!attribute)
		return;

	attribute->GetOnConditionChanged().AddUObject(this, &UEnemyPatternStateComponent::HandleConditionChanged);
}

void UEnemyPatternStateComponent::HandleConditionChanged(ECharacterCondition changedCondition)
{
	if (changedCondition != ECharacterCondition::Dead)
		return;

	UpdatePattenState(EEnemyPatternState::Dead);
}

void UEnemyPatternStateComponent::UnsubscribeToAttribute()
{
	if (!_owner)
		return;

	auto attribute = _owner->GetComponentByClass<UCharacterAttributeComponent>();
	if (!attribute)
		return;

	attribute->GetOnConditionChanged().RemoveAll(this);
}


void UEnemyPatternStateComponent::SubscribeToEventPropagation()
{
	if (!_owner)
		return;

	auto eventPropagation = _owner->FindComponentByClass<UActorEventPropagationComponent>();
	if (!eventPropagation)
		return;

	eventPropagation->GetOnDamageApplied().AddUObject(this, &UEnemyPatternStateComponent::HandleTakeDamage);
}

void UEnemyPatternStateComponent::HandleTakeDamage(float damage, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
	if (_patternState != EEnemyPatternState::Patrol)
		return;

	SetChaseTarget(eventInstigator);
	UpdatePattenState(EEnemyPatternState::Chase);
}

void UEnemyPatternStateComponent::SetChaseTarget(AController* eventInstigator)
{
	if (!eventInstigator || !_owner)
		return;

	auto chase = _owner->GetComponentByClass<UEnemyChaseComponent>();
	if (!chase)
		return;

	APawn* InstigatorPawn = eventInstigator->GetPawn();
	ATrialCharacter* attackerCharacter = Cast<ATrialCharacter>(InstigatorPawn);
	if (!attackerCharacter)
		return;

	chase->SetChaseTarget(attackerCharacter);
}

void UEnemyPatternStateComponent::UnsubscribeToEventPropagation()
{
	if (!_owner)
		return;

	auto eventPropagation = _owner->FindComponentByClass<UActorEventPropagationComponent>();
	if (!eventPropagation)
		return;

	eventPropagation->GetOnDamageApplied().RemoveAll(this);
}
