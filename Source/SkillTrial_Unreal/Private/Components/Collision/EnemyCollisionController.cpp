// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Collision/EnemyCollisionController.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/Enemy.h"

// Sets default values for this component's properties
UEnemyCollisionController::UEnemyCollisionController()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEnemyCollisionController::BeginPlay()
{
	Super::BeginPlay();
	SetOwner();
}

void UEnemyCollisionController::Setup(ACharacter* owner)
{
	SetupCollisionOptions(owner);
}

void UEnemyCollisionController::SetupCollisionOptions(ACharacter* owner)
{
	auto mesh = owner->GetMesh();
	mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	mesh->SetGenerateOverlapEvents(true);

	auto capsule = owner->GetCapsuleComponent();
	capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void UEnemyCollisionController::SetCollisionEnabled(ECollisionEnabled::Type targetEnableType)
{
	if (!_owner)
		return;
	_owner->GetCapsuleComponent()->SetCollisionEnabled(targetEnableType);
}
