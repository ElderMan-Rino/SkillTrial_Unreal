// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Collision/PlayerCollisionController.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

UPlayerCollisionController::UPlayerCollisionController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPlayerCollisionController::Setup(ACharacter* owner)
{
	SetupCollisionOptions(owner);
}

void UPlayerCollisionController::SetupCollisionOptions(ACharacter* owner)
{
	auto mesh = owner->GetMesh();
	mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	mesh->SetGenerateOverlapEvents(true);

	auto capsule = owner->GetCapsuleComponent();
	capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}