// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Equipment/EquipmentComponent.h"
#include "Items/Weapons/Weapon.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	SetOwner();
}

void UEquipmentComponent::SetWeaponCollisionEnabled(ECollisionEnabled::Type collsionType)
{
	if (!_equippedWeapon)
		return;

	_equippedWeapon->SetBoxCollisionEnable(collsionType);
	_equippedWeapon->ClearIgnoreActors();
}

void UEquipmentComponent::AttachWeaponToCharacter(AWeapon* targetWeapon, FName socketName)
{
	targetWeapon->Equip(_owner->GetMesh(), socketName, _owner, _owner);
}

void UEquipmentComponent::UpdateWeaponCollision(bool enableCollison)
{
	if (!_equippedWeapon)
		return;

	_equippedWeapon->SetBoxCollisionEnable(enableCollison ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	_equippedWeapon->ClearIgnoreActors();
}
