// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/CharacterEquipState.h"
#include "Enums/ActionInputType.h"
#include "EquipmentComponent.generated.h"

class AWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentComponent();

public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ACharacter> _owner;
private:
	FORCEINLINE void SetOwner() { _owner = Cast<ACharacter>(GetOwner()); }

public:
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type collsionType);
protected:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* _equippedWeapon = nullptr;
	FORCEINLINE void SetEquippedWeapon(AWeapon* targetWeapon) { _equippedWeapon = targetWeapon; }

protected:
	void AttachWeaponToCharacter(AWeapon* targetWeapon, FName socketName);

public:
	void UpdateWeaponCollision(bool enableCollison);
};
