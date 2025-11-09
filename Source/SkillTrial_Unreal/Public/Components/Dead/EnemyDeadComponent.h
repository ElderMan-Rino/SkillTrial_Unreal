// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/KnightDeadAniState.h"
#include "Enums/CharacterCondition.h"
#include "EnemyDeadComponent.generated.h"

class AEnemy;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UEnemyDeadComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyDeadComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AEnemy> _owner;
private:
	FORCEINLINE void SetOwner() { _owner = Cast<AEnemy>(GetOwner()); }

private:
	void SubscribeToPatternState();
	void HandlePatternChanged(EEnemyPatternState changedPattern);
	void UnsubscribeToPatternState();

protected:
	UPROPERTY(BlueprintReadOnly)
	EKnightDeadAniState _deadAniState = EKnightDeadAniState::DeadOne;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool _deadAniEnd = false;
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetDeadAniEnd() { _deadAniEnd = true; }

private:
	UPROPERTY(EditDefaultsOnly, Category = Montage)
	class UAnimMontage* _deathMontage;

private:
	void PlayDeathMontage();

private:
	FName GetDeathMontageSection();
	void SetDeathAniState();

private:
	UPROPERTY(EditAnywhere, Category = SFX)
	USoundBase* _deathSFX;
	void PlayDeathSFX();
	UPROPERTY(EditAnywhere, Category = SFX)
	TMap<EKnightDeadAniState, FName> _sectionNames;

private:
	void SetLifeSpanForDead();
	void SetCollisonForDead();

private:
	void SetWeaponCollisionDisabled();

private:
	UPROPERTY(EditAnywhere, Category = Spawn)
	TSubclassOf<class ATreasure> _treasureClass;
	void SpawnTreasure();
};
