// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/EnemyPatternState.h"
#include "EnemyEngageComponent.generated.h"

class ATrialCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UEnemyEngageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyEngageComponent();

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
	UPROPERTY(EditAnywhere)
	double _engageRadius = 200.f;

private:
	FORCEINLINE void SetEngageTickEnabled(bool enable) { PrimaryComponentTick.SetTickFunctionEnable(enable); }

private:
	void SubscribeToPatternState();
	void HandlePatternStateChange(EEnemyPatternState enemyPatternState);
	void UnsubscribeToPatternState();

private:
	void SubscribeToChase();
	void HandleTargetInEngageRange(ATrialCharacter* chaseTarget);
	void UnsubscribeToChase();

private:
	void UpdateEngage(float deltaTime);
	bool IsTargetInFrontAngle() const;
	bool CanControlAttack();
	void LookAtTarget(float deltaTime);
	void ExecuteCombatAttack();
	void RequestChangeToChase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<ATrialCharacter> _engageTarget = nullptr;
private:
	FORCEINLINE void UpdateEngageTarget(ATrialCharacter* target) { _engageTarget = target; }
	FORCEINLINE void ClearEngageTarget() { _engageTarget = nullptr; }

private:
	bool IsTargetValidAndAlive() const;
};
