// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Movement/CharacterMovementController.h"
#include "Enums/AxisInputType.h"
#include "Enums/CharacterActionState.h"
#include "Enums/CharacterMovementRestriction.h"
#include "Enums/AniActivityState.h"
#include "PlayerMovementController.generated.h"

using FAxisFunc = void (UPlayerMovementController::*)(float);

/**
 * 
 */
UCLASS()
class SKILLTRIAL_UNREAL_API UPlayerMovementController : public UCharacterMovementController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

public:
	virtual void Setup(ACharacter* owner) override;
	
private:
	void SetupMovementValues(ACharacter* owner);

private:
	TMap<EAxisInputType, FAxisFunc> _axisMap;
	void SetupAxisMap();
	void SubscribeToAxisInput();
	UFUNCTION()
	void HandleAxisInput(EAxisInputType axisInputType, float value);
	void HandleMoveForward(const float value);
	void HandleMoveRight(const float value);
	void HandleTurn(const float value);
	void HandleLookUp(const float value);
	const FVector GetControlDirection(EAxis::Type targetAxis);
	void UnsubscribeToAxisInput();

protected:
	UFUNCTION(BlueprintCallable)
	void ResetDodgeRotaion();
private:
	void SubscribeToActionInput();
	void HandleActionInput(EActionInputType actionInputType);
	void HandleJump();
	void HandleDodge();
	void SetDodgeRotation();
	void UnubscribeToActionInput();

private:
	void SubscribeToAniActivityChange();
	void HandleAniActivityChange(EAniActivityState aniActivityState);
	ECharacterMovementRestriction CalculateNewMovementRestriction(EAniActivityState aniActivityState) const;
	void UnsubscribeToAniActivityChange();

private:
	float _forwardInput = 0.f;
	FORCEINLINE void UpdateForwardInput(float forwardInput) { _forwardInput = forwardInput; }
	float _rightInput = 0.f;
	FORCEINLINE void UpdateRightInput(float rightInput) { _rightInput = rightInput; }
	FORCEINLINE bool IsDodgeAction() { return _forwardInput != 0.f || _rightInput != 0.f; }

private:
	UPROPERTY(EditDefaultsOnly, Category = Montage)
	class UAnimMontage* _dodgeMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montage)
	float _dodgeStaminaCost = 30.f;
	bool CheckDodgeEnable();
	void ConsumeDodgeStamina();

};
