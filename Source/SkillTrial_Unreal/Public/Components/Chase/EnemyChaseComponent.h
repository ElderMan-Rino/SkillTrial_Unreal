// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/EnemyPatternState.h"
#include "EnemyChaseComponent.generated.h"

class ATrialCharacter;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTargetInEngageRange, ATrialCharacter* /*Axis*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UEnemyChaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyChaseComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AEnemy> _owner = nullptr;
	FORCEINLINE void SetOwner() { _owner = Cast<AEnemy>(GetOwner()); }
		
private:
	FORCEINLINE void SetChaseTickEnabled(bool enable) { PrimaryComponentTick.SetTickFunctionEnable(enable); }

public:
	FORCEINLINE void SetChaseTarget(ATrialCharacter* chaseTarget) { _chaseTarget = chaseTarget; }
private:
	TObjectPtr<ATrialCharacter> _chaseTarget = nullptr;
	FORCEINLINE void ClearChaseTarget() { _chaseTarget = nullptr; }

private:
	bool _isInEngageRange = false;
	FORCEINLINE void UpdateIsInEngageRange(bool targetValue) { _isInEngageRange = targetValue; }

private:
	UPROPERTY(EditAnywhere)
	double _engageRadius = 200.f;

private:
	UPROPERTY(EditAnywhere)
	double _chaseRadius = 1500.f;

private:
	void UpdateChase();

private:
	void SubscribeToPatternState();
	void HandlePatternStateChange(EEnemyPatternState enemyPatternState);
	void UnsubscribeToPatternState();

public:
	FORCEINLINE FOnTargetInEngageRange& GetOnTargetInEngageRange() { return _onTargetInEngageRange; }
	FORCEINLINE const FOnTargetInEngageRange& GetOnTargetInEngageRange() const { return _onTargetInEngageRange; }
private:
	FOnTargetInEngageRange _onTargetInEngageRange;
	FORCEINLINE void BroadcastTargetInEngageRange() { _onTargetInEngageRange.Broadcast(_chaseTarget); }

private:
	void SubscribeToPawnSensing();
	void HandleTrialCharacterDetected(class ATrialCharacter* detectedCharacter);
	void UnsubscribeToPawnSensing();

private:
	void StartChase();
	bool IsChasing();

private:
	void RequestChangeToEngage();
	void RequestChangeToPatrol();
};
