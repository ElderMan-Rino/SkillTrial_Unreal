// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/VFX/CharacterVFXComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UCharacterVFXComponent::UCharacterVFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UCharacterVFXComponent::BeginPlay()
{
	Super::BeginPlay();
	SetOwner();
}

void UCharacterVFXComponent::PlayVFX(UParticleSystem* targetVFX, const FVector& targetLocation)
{
	if (!targetVFX || !_owner)
		return;

	UGameplayStatics::SpawnEmitterAtLocation(_owner->GetWorld(), targetVFX, targetLocation);
}