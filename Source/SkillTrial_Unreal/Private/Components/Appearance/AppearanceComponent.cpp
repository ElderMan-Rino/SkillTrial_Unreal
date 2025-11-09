// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Appearance/AppearanceComponent.h"
#include "GameFramework/Actor.h"
#include "GroomComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UAppearanceComponent::UAppearanceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAppearanceComponent::Setup(AActor* owner, TArray<UActorComponent*>& registeredSubComponents)
{
	check(owner);

	ACharacter* ownerCharacter = Cast<ACharacter>(owner);
	if (!ownerCharacter)
		return;

	CreateHair(owner, registeredSubComponents);
	SetupHair(ownerCharacter);

	CreateEyeBrows(owner, registeredSubComponents);
	SetupEyeBrows(ownerCharacter);
}

void UAppearanceComponent::CreateHair(AActor* owner, TArray<UActorComponent*>& registeredSubComponents)
{
	_hairGroom = owner->CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));

	registeredSubComponents.Add(_hairGroom);
}	

void UAppearanceComponent::SetupHair(ACharacter* character)
{
	_hairGroom->SetupAttachment(character->GetMesh());
	_hairGroom->AttachmentName = FString("head");
}

void UAppearanceComponent::CreateEyeBrows(AActor* owner, TArray<UActorComponent*>& registeredSubComponents)
{
	_eyeBrowsGroom = owner->CreateDefaultSubobject<UGroomComponent>(TEXT("EyeBrows"));

	registeredSubComponents.Add(_eyeBrowsGroom);
}

void UAppearanceComponent::SetupEyeBrows(ACharacter* character)
{
	_eyeBrowsGroom->SetupAttachment(character->GetMesh());
	_eyeBrowsGroom->AttachmentName = FString("head");
}
