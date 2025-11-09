// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Movement/CharacterMovementController.h"
#include "EnemyMovementController.generated.h"

class ACharacter;
/**
 * 
 */
UCLASS()
class SKILLTRIAL_UNREAL_API UEnemyMovementController : public UCharacterMovementController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Setup(ACharacter* owner) override;

private:
	void SetupMovementValues(ACharacter* owner);

private:
	UPROPERTY(VisibleAnywhere)
	class AAIController* _aiController;
	void SetAiController();
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float _acceptanceRadius = 50.f;

public:
	void UpdateMovementWalkSpeed(float targetSpeed);

public:
	void MoveToTargetActor(const AActor* target);

public:
	bool IsMovingToTarget() const;

};
