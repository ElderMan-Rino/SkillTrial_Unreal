// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkillTrial01_UnrealPickUpComponent.h"

USkillTrial01_UnrealPickUpComponent::USkillTrial01_UnrealPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void USkillTrial01_UnrealPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &USkillTrial01_UnrealPickUpComponent::OnSphereBeginOverlap);
}

void USkillTrial01_UnrealPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	ASkillTrial01_UnrealCharacter* Character = Cast<ASkillTrial01_UnrealCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
