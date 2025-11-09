// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Event/ActorEventPropagationComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values for this component's properties
UActorEventPropagationComponent::UActorEventPropagationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UActorEventPropagationComponent::BroadcastDamageApplied(float damage, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
	_onDamageApplied.Broadcast(damage, damageEvent, eventInstigator, damageCauser);
}