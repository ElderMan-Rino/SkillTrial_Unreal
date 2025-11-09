// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/CharacterCondition.h"
#include "Enums/KnightDeadAniState.h"
#include "CharacterDeadComponent.generated.h"

class ACharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UCharacterDeadComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterDeadComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ACharacter> _owner;
private:
	FORCEINLINE void SetOwner() { _owner = Cast<ACharacter>(GetOwner()); }

private:
	void SubscribeToAttribute();
	void HandleConditionChanged(ECharacterCondition changedCondition);
	void UnsubscribeToAttribute();

private:
	UPROPERTY(EditDefaultsOnly, Category = Montage)
	class UAnimMontage* _deathMontage;

private:
	void PlayDeathMontage();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EKnightDeadAniState _deadAniState = EKnightDeadAniState::DeadOne;
	UPROPERTY(EditAnywhere, Category = SFX)
	TMap<EKnightDeadAniState, FName> _sectionNames;
private:
	FName GetDeathMontageSection();
	void SetDeathAniState();
};
