// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums/CharacterEquipState.h"
#include "Enums/CharacterActionState.h"
#include "TrialCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;
class AWeapon;

UCLASS()
class SKILLTRIAL_UNREAL_API ATrialCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATrialCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type collsionType);

	FORCEINLINE void SetOverlappingItem(AItem* aItem) { _overlappingItem = aItem; }
	FORCEINLINE ECharacterEquipState GetEquipState() const { return _equipState; }

protected:
	UFUNCTION(BlueprintCallable) 
	void UpdateActionState(ECharacterActionState targetState);

	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);
	void EKeyPressed();
	void HandleEKeyPressed();
	void Attack();

	UFUNCTION(BlueprintCallable)
	void Disarm();
	UFUNCTION(BlueprintCallable)
	void Arm();

private:
	void SetUseControllerValues();
	void SetCharacterMovementValues();
	void InitializeSpringArmComponent();
	void InitializeViewCamComponent();
	void InitializeHair();
	void InitializeEyeBrows();
	void SetSpringArmLength();
	void BindAxes(UInputComponent* PlayerInputComponent);
	void BindActions(UInputComponent* PlayerInputComponent);
	const FVector GetControlDirection(EAxis::Type targetAxis);
	void PickWeaponItem();
	void UpdateEquipState(ECharacterEquipState targetState);
	void PlayAttackMontage();
	void JumpAction();
	bool CanAttack();
	bool CanMoving();
	bool CanDisarm();
	bool CanArm();
	void PlayEquipMontage(FName sectionName);
	void PlayTargetMontage(UAnimMontage* targetMontage, FName sectionName);
	
	
	UPROPERTY(VisibleAnywhere);
	USpringArmComponent* _springArm;
	UPROPERTY(VisibleAnywhere);
	UCameraComponent* _viewCamera;
	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* _hair;
	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* _eyeBrows;
	UPROPERTY(VisibleInstanceOnly)
	AItem* _overlappingItem;
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* _equippedWeapon;
	UPROPERTY(EditDefaultsOnly, Category = Montage)
	UAnimMontage* _attackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montage)
	UAnimMontage* _equipMontage;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ECharacterActionState _actionState = ECharacterActionState::EAS_Unoccpled;
	
	ECharacterEquipState _equipState = ECharacterEquipState::ECS_Unequipped;
};
