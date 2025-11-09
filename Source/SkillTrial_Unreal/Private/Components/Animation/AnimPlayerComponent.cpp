// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Animation/AnimPlayerComponent.h"
#include "Characters/TrialCharacter.h"

// Sets default values for this component's properties
UAnimPlayerComponent::UAnimPlayerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UAnimPlayerComponent::BeginPlay()
{
	Super::BeginPlay();
	SetOwner();
}

void UAnimPlayerComponent::PlayTargetMontage(UAnimMontage* targetMontage, const FName& sectionName)
{
	if (!_owner || !targetMontage)
		return;

	UAnimInstance* animInstance = _owner->GetMesh()->GetAnimInstance();
	if (!animInstance)
		return;

	animInstance->Montage_Play(targetMontage);
	animInstance->Montage_JumpToSection(sectionName, targetMontage);
}