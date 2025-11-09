// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/CombatComponent.h"
#include "Enums/KnightDeadAniState.h"
#include "Enums/CharacterCondition.h"
#include "Enums/CharacterCondition.h"
#include "EnemyCombatComponent.generated.h"

struct FDamageEvent;

UCLASS()
class SKILLTRIAL_UNREAL_API UEnemyCombatComponent : public UCombatComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

protected:
	virtual void HandleHitEvent(const FVector& hitPoint, AActor* hitter) override;
private:
	void TransitionToEngageState();
};
