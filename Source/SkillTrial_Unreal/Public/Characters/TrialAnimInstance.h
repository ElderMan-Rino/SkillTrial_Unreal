// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enums/CharacterEquipState.h"
#include "TrialAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SKILLTRIAL_UNREAL_API UTrialAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float deltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class ATrialCharacter* _trialCharacter;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* _trialCharacterMovement;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float _groundSpeed;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool _isFalling;
	UPROPERTY(BlueprintReadOnly, Category = EquipState)
	class UPlayerEquipmentComponent* _equipment;
	UPROPERTY(BlueprintReadOnly, Category = EquipState)
	ECharacterEquipState _equipState;
private:
	void InitializeTrialCharacter();
	void InitializeTrialCharacterMovement();
	void InitializeEquipment();
	void UpdateGroundSpeed();
	void UpdateFalling();
	void UpdateEquipState();

};

