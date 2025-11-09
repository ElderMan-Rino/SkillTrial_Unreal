// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Equipment/EquipmentComponent.h"
#include "Enums/CharacterCondition.h"
#include "EnemyEquipmentComponent.generated.h"

class AWeapon;

/**
 * 
 */
UCLASS()
class SKILLTRIAL_UNREAL_API UEnemyEquipmentComponent : public UEquipmentComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

private:
	void SubscribeToEventPropagation();
	void HandleActorDestroyed();
	void UnsubscribeToEventPropagation();

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> _weaponClass;
	void SpawnWeapon();
	void ClearWeapon();
};
