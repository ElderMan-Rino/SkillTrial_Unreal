// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/EnemyPatternState.h"
#include "EnemyPawnSensingController.generated.h"

class ATrialCharacter;

DECLARE_MULTICAST_DELEGATE_OneParam(OnTrialCharacterDetected, ATrialCharacter* /*Axis*/);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SKILLTRIAL_UNREAL_API UEnemyPawnSensingController : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnemyPawnSensingController();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void Setup(AActor* owner, TArray<UActorComponent*>& registeredSubComponents);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AEnemy> _owner;
	FORCEINLINE void SetOwner() { _owner = Cast<AEnemy>(GetOwner()); }

private:
	UPROPERTY(VisibleAnywhere, Category = Sensing)
	TObjectPtr<class UPawnSensingComponent> _pawnSensing;
	void CreatePawnSensing(AActor* owner, TArray<UActorComponent*>& registeredSubComponents);
	void SetupPawnSensing();

private:
	bool _isSensingEnabled = true;
	FORCEINLINE void SetIsSensingEnabled(bool isSensingEnabled) { _isSensingEnabled = isSensingEnabled; }

protected:
	UFUNCTION()
	void OnPawnSeen(APawn* seenPawn);

private:
	void SubscribeToSeePawn();
	void UnsubscribeToSeePawn();

private:
	void SubscribeToPatternState();
	void HandlePatternStateChange(EEnemyPatternState enemyPatternState);
	void UnsubscribeToPatternState();

public:
	FORCEINLINE OnTrialCharacterDetected& GetOnTrialCharacterDetected() { return _onTrialCharacterDetected; }
	FORCEINLINE const OnTrialCharacterDetected& GetOnTrialCharacterDetected() const { return _onTrialCharacterDetected; }
	FORCEINLINE OnTrialCharacterDetected& GetOnTrialCharacterPostDetected() { return _onTrialCharacterPostDetected; }
	FORCEINLINE const OnTrialCharacterDetected& GetOnTrialCharacterPostDetected() const { return _onTrialCharacterPostDetected; }
private:
	OnTrialCharacterDetected _onTrialCharacterDetected;
	OnTrialCharacterDetected _onTrialCharacterPostDetected;
	void BroadcastTrialCharacterDetected(ATrialCharacter* detectedCharacter);
};
