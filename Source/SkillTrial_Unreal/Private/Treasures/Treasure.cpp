// Fill out your copyright notice in the Description page of Project Settings.


#include "Treasures/Treasure.h"
#include "Characters/TrialCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

ATreasure::ATreasure() : AItem()
{
	InitializeItemState();
}

void ATreasure::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	ATrialCharacter* trialCharacter = Cast<ATrialCharacter>(OtherActor);
	if (!trialCharacter)
		return;
	
	CreatePickUpEffect();
	PlaySFX(_coinSFX, GetActorLocation());
	Destroy();
}

void ATreasure::InitializeItemState()
{
	UpdateItemState(EItemState::EIS_Hovering);
}

void ATreasure::PlaySFX(USoundBase* targetSFX, const FVector& targetLocation)
{
	if (!targetSFX)
		return;

	UGameplayStatics::PlaySoundAtLocation(this, targetSFX, targetLocation);
}

void ATreasure::CreatePickUpEffect()
{
	if (!_pickUpEffect)
		return;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, _pickUpEffect, GetActorLocation());
}