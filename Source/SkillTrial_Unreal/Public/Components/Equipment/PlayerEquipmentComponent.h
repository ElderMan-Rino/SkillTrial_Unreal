// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Equipment/EquipmentComponent.h"
#include "PlayerEquipmentComponent.generated.h"



DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipStateChange, ECharacterEquipState)
/**
 * 
 */
UCLASS()
class SKILLTRIAL_UNREAL_API UPlayerEquipmentComponent : public UEquipmentComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

private:
	void SubscribeToActionInput();
	void UnsubscribeToActionInput();

public:
	void EquipWeapon(AWeapon* targetWeapon);

public:
	UFUNCTION(BlueprintCallable)
	void HandleDrawWeapon();
	UFUNCTION(BlueprintCallable)
	void HandleHolsterWeapon();
private:
	void HandleActionInput(EActionInputType actionInputType);
	void HandleEquippedWeapon();
	void DrawWeapon();
	void HolsterWeapon();

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ECharacterEquipState GetEquipState() { return _equipState; }
private:
	UPROPERTY(VisibleAnywhere)
	ECharacterEquipState _equipState = ECharacterEquipState::ECS_Unequipped;
	void UpdateEquipState(ECharacterEquipState targetState);

private:
	void PlayMontage(FName sectionName);

private:
	UPROPERTY(EditDefaultsOnly, Category = Montage)
	UAnimMontage* _equipMontage;

public:
	FORCEINLINE FOnEquipStateChange& GetOnEquipStateChange() { return _onEquipStateChange; }
	FORCEINLINE const FOnEquipStateChange& GetOnEquipStateChange() const { return _onEquipStateChange; }
private:
	FOnEquipStateChange _onEquipStateChange;
	FORCEINLINE void BroadcastEquipStateChange() { _onEquipStateChange.Broadcast(_equipState); }
};
