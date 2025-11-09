// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PickUpItem/PickUpItemComponent.h"
#include "Components/Event/ActorEventPropagationComponent.h"
#include "Items/Item.h"
#include "Treasures/Treasure.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UPickUpItemComponent::UPickUpItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UPickUpItemComponent::BeginPlay()
{
	Super::BeginPlay();
	SetOwner();
	SubscribeToEventPropagation();
}

void UPickUpItemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnsubscribeToEventPropagation();
	Super::EndPlay(EndPlayReason);
}


void UPickUpItemComponent::SubscribeToEventPropagation()
{
	if (!_owner)
		return;

	auto actorEvent = _owner->GetComponentByClass<UActorEventPropagationComponent>();
	if (!actorEvent)
		return;

	actorEvent->GetOnPickedUpItem().AddUObject(this, &UPickUpItemComponent::HandlePickedUpItem);
}

void UPickUpItemComponent::HandlePickedUpItem(AItem* item)
{
	HandlePickedUpTreasure(item);
}

void UPickUpItemComponent::UnsubscribeToEventPropagation()
{
	if (!_owner)
		return;

	auto actorEvent = _owner->GetComponentByClass<UActorEventPropagationComponent>();
	if (!actorEvent)
		return;

	actorEvent->GetOnPickedUpItem().RemoveAll(this);
}

void UPickUpItemComponent::AddTreasureValue(int32 treasureValue)
{
	_pickedTreasureValue += treasureValue;
	BroadcastPickedTreasureAdded();
}

void UPickUpItemComponent::HandlePickedUpTreasure(AItem* item)
{
	auto treasure = Cast<ATreasure>(item);
	if (!treasure)
		return;

	AddTreasureValue(treasure->GetTreasureValue());
}
