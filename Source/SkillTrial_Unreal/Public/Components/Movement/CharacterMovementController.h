// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/CharacterMovementRestriction.h"
#include "Enums/CharacterCondition.h"
#include "CharacterMovementController.generated.h"

class ACharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UCharacterMovementController : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterMovementController();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

public:
	virtual void Setup(ACharacter* owner);

protected:
	UPROPERTY(VisibleAnywhere)
	ACharacter* _owner = nullptr;
private:
	FORCEINLINE void SetOwner() { _owner = Cast<ACharacter>(GetOwner()); }

protected:
	void SetupUseControllerValues(class ACharacter* owner);

protected:
	ECharacterMovementRestriction _movementRestriction = ECharacterMovementRestriction::None;
	FORCEINLINE bool CanControlMovement() const { return  _movementRestriction == ECharacterMovementRestriction::None; }
	FORCEINLINE void UpdateMovementRestriction(ECharacterMovementRestriction targetRestriction) {  _movementRestriction = targetRestriction; }

public:
	void UpdateMovementWalkSpeed(float targetSpeed);

private:
	void SubscribeToAttribute();
	void HandleConditionChanged(ECharacterCondition characterCondition);
	void UnsubscribeToAttribute();
	
};
