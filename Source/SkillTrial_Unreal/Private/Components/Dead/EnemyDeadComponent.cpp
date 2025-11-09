// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Dead/EnemyDeadComponent.h"
#include "Components/Animation/AnimPlayerComponent.h"
#include "Components/SFX/CharacterSFXComponent.h"
#include "Components/Attribute/CharacterAttributeComponent.h"
#include "Components/Collision/EnemyCollisionController.h"
#include "Components/Equipment/EquipmentComponent.h"
#include "Components/State/EnemyPatternStateComponent.h"
#include "Treasures/Treasure.h"
#include "Enemy/Enemy.h"

// Sets default values for this component's properties
UEnemyDeadComponent::UEnemyDeadComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEnemyDeadComponent::BeginPlay()
{
	Super::BeginPlay();
	SetOwner();
	SubscribeToPatternState();
}

void UEnemyDeadComponent::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	UnsubscribeToPatternState();
	Super::EndPlay(endPlayReason);
}

void UEnemyDeadComponent::SubscribeToPatternState()
{
	if (!_owner)
		return;

	auto patternState = _owner->GetComponentByClass<UEnemyPatternStateComponent>();
	if (!patternState)
		return;

	patternState->GetOnPatternPostStateChange().AddUObject(this, &UEnemyDeadComponent::HandlePatternChanged);
}

void UEnemyDeadComponent::HandlePatternChanged(EEnemyPatternState changedPattern)
{
	if (changedPattern != EEnemyPatternState::Dead)
		return;

	SetWeaponCollisionDisabled();
	SpawnTreasure();
	PlayDeathSFX();
	PlayDeathMontage();
	SetLifeSpanForDead();
	SetCollisonForDead();
}

void UEnemyDeadComponent::UnsubscribeToPatternState()
{
	if (!_owner)
		return;

	auto patternState = _owner->GetComponentByClass<UEnemyPatternStateComponent>();
	if (!patternState)
		return;

	patternState->GetOnPatternPostStateChange().RemoveAll(this);
}

void UEnemyDeadComponent::PlayDeathMontage()
{
	if (!_deathMontage || !_owner)
		return;

	auto animPlayer = _owner->GetComponentByClass<UAnimPlayerComponent>();
	if (!animPlayer)
		return;

	SetDeathAniState();
	animPlayer->PlayTargetMontage(_deathMontage, GetDeathMontageSection());
}

FName UEnemyDeadComponent::GetDeathMontageSection()
{
	if (_sectionNames.Contains(_deadAniState))
		return _sectionNames[_deadAniState];
	return FName("DeathOne");
}

void UEnemyDeadComponent::SetDeathAniState()
{
	int32 selection = FMath::RandRange(0, 3);
	switch (selection)
	{
	case 0:
		_deadAniState = EKnightDeadAniState::DeadOne;
		break;
	case 1:
		_deadAniState = EKnightDeadAniState::DeadTwo;
		break;
	case 2:
		_deadAniState = EKnightDeadAniState::DeadThree;
		break;
	case 3:
		_deadAniState = EKnightDeadAniState::DeadFour;
		break;
	default:
		_deadAniState = EKnightDeadAniState::DeadOne;
		break;
	}
}

void UEnemyDeadComponent::PlayDeathSFX()
{
	if (!_owner || !_deathSFX)
		return;

	auto sfxComponent = _owner->GetComponentByClass<UCharacterSFXComponent>();
	if (!sfxComponent)
		return;

	sfxComponent->PlaySFX(_deathSFX, _owner->GetTargetLocation());
}

void UEnemyDeadComponent::SetLifeSpanForDead()
{
	if (!_owner)
		return;

	_owner->SetLifeSpan(60.f);
}

void UEnemyDeadComponent::SetCollisonForDead()
{
	if (!_owner)
		return;

	auto collsionController = _owner->GetComponentByClass<UEnemyCollisionController>();
	if (!collsionController)
		return;

	collsionController->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UEnemyDeadComponent::SetWeaponCollisionDisabled()
{
	if (!_owner)
		return;

	auto equipment = _owner->GetComponentByClass<UEquipmentComponent>();
	if (!equipment)
		return;

	equipment->UpdateWeaponCollision(false);
}

void UEnemyDeadComponent::SpawnTreasure()
{
	if (!_treasureClass || !_owner)
		return;

	auto world = GetWorld();
	if (!world)
		return;

	const FVector spawnLocation = _owner->GetActorLocation() + FVector(0.f, 0.f, 25.f);
	world->SpawnActor<ATreasure>(_treasureClass, spawnLocation, _owner->GetActorRotation());
}