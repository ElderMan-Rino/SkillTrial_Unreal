// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/EnemyPatternState.h"
#include "Enums/CharacterCondition.h"
#include "EnemyPatternStateComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPatternStateChange, EEnemyPatternState /*Axis*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UEnemyPatternStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyPatternStateComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AEnemy> _owner = nullptr;
	FORCEINLINE void SetOwner() { _owner = Cast<AEnemy>(GetOwner()); }

public:
	void UpdatePattenState(EEnemyPatternState patternState);
private:
	UPROPERTY(VisibleAnywhere)
	EEnemyPatternState _patternState = EEnemyPatternState::Patrol;

public:
	FORCEINLINE FOnPatternStateChange& GetOnPatternStateChange() { return _onPatternStateChanged; }
	FORCEINLINE const FOnPatternStateChange& GetOnPatternStateChange() const { return _onPatternStateChanged; }
	FORCEINLINE FOnPatternStateChange& GetOnPatternPostStateChange() { return _onPatternPostStateChanged; }
	FORCEINLINE const FOnPatternStateChange& GetOnPatternPostStateChange() const { return _onPatternPostStateChanged; }
private:
	FOnPatternStateChange _onPatternStateChanged;
	FOnPatternStateChange _onPatternPostStateChanged;
	void BroadcastPatternStateChange();

private:
	void PrepareInitialStateBroadcast();
	void BroadcastDelayedInitialState();

private:
	void SubscribeToAttribute();
	void HandleConditionChanged(ECharacterCondition changedCondition);
	void HandleHealthChanged();
	void UnsubscribeToAttribute();

protected:
	void SubscribeToEventPropagation();
	void HandleTakeDamage(float damage, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser);
	void SetChaseTarget(AController* eventInstigator);// chase¿¡ Å¸°Ù Àü´Þ 
	void UnsubscribeToEventPropagation();
};