// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterVFXComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UCharacterVFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterVFXComponent();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class ACharacter* _owner;
	FORCEINLINE void SetOwner() { _owner = Cast<ACharacter>(GetOwner()); }
		
public:
	void PlayVFX(UParticleSystem* targetVFX, const FVector& targetLocation);
};
