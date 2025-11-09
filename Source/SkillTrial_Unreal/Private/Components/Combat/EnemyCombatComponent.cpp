// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/EnemyCombatComponent.h"
#include "Components/Event/ActorEventPropagationComponent.h"
#include "Components/State/EnemyPatternStateComponent.h"
#include "Components/Attribute/CharacterAttributeComponent.h"
#include "GameFramework/Character.h"
#include "Enums/EnemyPatternState.h"
#include "Engine/DamageEvents.h"

void UEnemyCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	UpdateAttackRestriction(ECharacterAttackRestriction::None);
}

void UEnemyCombatComponent::HandleHitEvent(const FVector& hitPoint, AActor* hitter)
{
	PlayTargetSFX(_hitSFX, hitPoint);
	PlayTargetVFX(_bloodVFX, hitPoint);

	auto ownerCondition = GetOwnerCondition();
	if (ownerCondition != ECharacterCondition::Alive)
		return;
	
	PlayHitMontage(hitter ? hitter->GetActorLocation() : hitPoint);
	TransitionToEngageState();
}

void UEnemyCombatComponent::TransitionToEngageState()
{
	if (!_owner)
		return;

	auto patternState = _owner->GetComponentByClass<UEnemyPatternStateComponent>();
	if (!patternState)
		return;

	patternState->UpdatePattenState(EEnemyPatternState::Engage);
}
