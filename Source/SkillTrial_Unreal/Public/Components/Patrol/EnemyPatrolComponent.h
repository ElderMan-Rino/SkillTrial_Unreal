// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/EnemyPatternState.h"
#include "EnemyPatrolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UEnemyPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyPatrolComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AEnemy> _owner;
	FORCEINLINE void SetOwner() { _owner = Cast<AEnemy>(GetOwner()); }

private:
	void SubscribeToPatternState();
	void HandlePatternStateChanged(EEnemyPatternState patternState);
	void UnsubscribeToPatternState();

private:
	FORCEINLINE void SetPatrolTickEnabled(bool enable) { PrimaryComponentTick.SetTickFunctionEnable(enable); }

private:
	UPROPERTY(EditInstanceOnly, Category = "Ai Nav")
	TObjectPtr<class AActor> _patrolTargetPoint = nullptr;
	UPROPERTY(EditInstanceOnly, Category = "Ai Nav")
	TArray<AActor*> _patrolTargetPoints;
	void SelectNextPatrolPoint();

private:
	UPROPERTY(EditAnywhere)
	double _patrolRadius = 200.f;
	bool _isAtPatrolPoint = false;
	FORCEINLINE void UpdateIsAtPatrolPoint(bool targetValue) { _isAtPatrolPoint = targetValue; }

private:
	void StartPatrol();
	void StopPatrol();
	void UpdatePatrol();

private:
	void StartMoveToTargetPatrolPoint();
	
private:
	UPROPERTY(EditAnywhere)
	float _patrolMinDelay = 3.f;
	UPROPERTY(EditAnywhere)
	float _patrolMaxDelay = 15.f;
	FTimerHandle _patrolDelayTimer;

	void StartPatrolDelayTimer();
	void ClearPatrolDelayTimer();
	void HandlePatrolTimerFinished();
	const float GetPatrolDelay();

private:
	void SubscribeToPawnSensing();
	void HandleTrialCharacterDetected(class ATrialCharacter* detectedCharacter);
	void UnsubscribeToPawnSensing();

private:
	void RequestChangeToChase();
};
