// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Equipment/EnemyEquipmentComponent.h"
#include "Items/Weapons/Weapon.h"
#include "GameFramework/Character.h"
#include "Components/Event/ActorEventPropagationComponent.h"

void UEnemyEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeapon();
	SubscribeToEventPropagation();
}

void UEnemyEquipmentComponent::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	UnsubscribeToEventPropagation();
	Super::EndPlay(endPlayReason);
}

void UEnemyEquipmentComponent::SubscribeToEventPropagation()
{
	if (!_owner)
		return;

	auto actorEvent = _owner->GetComponentByClass<UActorEventPropagationComponent>();
	if (!actorEvent)
		return;

	actorEvent->GetOnActorDestroyed().AddUObject(this, &UEnemyEquipmentComponent::HandleActorDestroyed);
}

void UEnemyEquipmentComponent::HandleActorDestroyed()
{
	ClearWeapon();
}

void UEnemyEquipmentComponent::UnsubscribeToEventPropagation()
{
	if (!_owner)
		return;

	auto actorEvent = _owner->GetComponentByClass<UActorEventPropagationComponent>();
	if (!actorEvent)
		return;

	actorEvent->GetOnActorDestroyed().RemoveAll(this);
}

void UEnemyEquipmentComponent::SpawnWeapon()
{
	if (!_weaponClass)
		return;

	auto world = _owner->GetWorld();
	if (!world)
		return;
	
	_equippedWeapon = world->SpawnActor<AWeapon>(_weaponClass);
	if (!_equippedWeapon)
		return;

	AttachWeaponToCharacter(_equippedWeapon, FName("RightHandSocket"));
}

void UEnemyEquipmentComponent::ClearWeapon()
{
	if (!_equippedWeapon)
		return;
	_equippedWeapon->Destroy();
}
