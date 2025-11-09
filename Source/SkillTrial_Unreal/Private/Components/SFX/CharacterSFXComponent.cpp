// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SFX/CharacterSFXComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UCharacterSFXComponent::UCharacterSFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterSFXComponent::BeginPlay()
{
	Super::BeginPlay();
	SetOwner();
}

void UCharacterSFXComponent::PlaySFX(USoundBase* targetSFX, const FVector& targetLocation)
{
	if (!targetSFX || !_owner)
		return;

	UGameplayStatics::PlaySoundAtLocation(_owner->GetWorld(), targetSFX, targetLocation);
}