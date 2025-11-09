// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterSFXComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UCharacterSFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterSFXComponent();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ACharacter> _owner;
	FORCEINLINE void SetOwner() { _owner = Cast<ACharacter>(GetOwner()); }

public:
	void PlaySFX(USoundBase* targetSFX, const FVector& targetLocation);
};
