// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Animation/AniActivityComponent.h"
#include "Components/Event/ActorEventPropagationComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UAniActivityComponent::UAniActivityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAniActivityComponent::BeginPlay()
{
	Super::BeginPlay();
	SetOwner();
	SubscribeToEventPropagation();
}

void UAniActivityComponent::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	UnsubscribeToEventPropagation();
	Super::EndPlay(endPlayReason);
}

void UAniActivityComponent::UpdateAniActivityState(EAniActivityState aniActivityState)
{
	_aniActivityState = aniActivityState;
	BroadcastAniActivityChange();
}

void UAniActivityComponent::SubscribeToEventPropagation()
{
	if (!_owner)
		return;

	auto eventPropagation = _owner->GetComponentByClass<UActorEventPropagationComponent>();
	if (!eventPropagation)
		return;

	eventPropagation->GetOnMovementModeChanged().AddUObject(this, &UAniActivityComponent::HandleMovementModeChanged);
}

void UAniActivityComponent::HandleMovementModeChanged(const EMovementMode prevMode, const EMovementMode curMode)
{
	if (curMode == EMovementMode::MOVE_Falling)
		UpdateAniActivityState(EAniActivityState::EAS_Active);
	else if (prevMode == EMovementMode::MOVE_Falling)
		UpdateAniActivityState(EAniActivityState::EAS_Inactive);
}

void UAniActivityComponent::UnsubscribeToEventPropagation()
{
	if (!_owner)
		return;

	auto eventPropagation = _owner->GetComponentByClass<UActorEventPropagationComponent>();
	if (!eventPropagation)
		return;

	eventPropagation->GetOnMovementModeChanged().RemoveAll(this);
}


