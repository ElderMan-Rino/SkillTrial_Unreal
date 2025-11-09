// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/CombatComponent.h"
#include "PlayerCombatComponent.generated.h"

/**
 * 
 */
UCLASS()
class SKILLTRIAL_UNREAL_API UPlayerCombatComponent : public UCombatComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

private:
	void SubscribeToActionInput();
	void HandleActionInput(EActionInputType actionInputType);
	void HandleActionAttack();
	void UnsubscribeToActionInput();

private:
	void SubscribeToEquipment();
	void HandleEquipStateChange(ECharacterEquipState equipState);
	void UnubscribeToEquipment();

protected:
	virtual void SubscribeToEventPropagation() override;
	void HandleMovementModeChanged(const EMovementMode prevMode, const EMovementMode curMode);
	virtual void UnsubscribeToEventPropagation() override;

protected:
	virtual void HandleHitEvent(const FVector& hitPoint, AActor* hitter) override;

private:
	bool IsPlayingAnimation();

private:
	void SubscribeToAttribute();
	void HandleConditionChanged(ECharacterCondition characterCondition);
	void UnsubscribeToAttribute();
};
