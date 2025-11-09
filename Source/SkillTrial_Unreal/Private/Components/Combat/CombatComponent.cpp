// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/CombatComponent.h"
#include "Components/Inputs/ActionInputController.h"
#include "Components/State/PlayerStateComponent.h"
#include "Components/Animation/AnimPlayerComponent.h"
#include "Components/Animation/AniActivityComponent.h"
#include "Components/Equipment/EquipmentComponent.h"
#include "Components/SFX/CharacterSFXComponent.h"
#include "Components/VFX/CharacterVFXComponent.h"
#include "Components/Event/ActorEventPropagationComponent.h"
#include "Components/Attribute/CharacterAttributeComponent.h"
#include "Enums/CharacterAttackRestriction.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// CharacterCombat이랑 나누기 필요
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetOwner();
	SubscribeToAniActivityChange();
	SubscribeToEventPropagation();
}

void UCombatComponent::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	UnsubscribeToEventPropagation();
	UnsubscribeToAniActivityChange();
	Super::EndPlay(endPlayReason);
}

void UCombatComponent::UpdateAttackRestriction(EMovementMode movementMode)
{
	auto attackFlag = (uint32)_attackRestriction;
	const uint32 falling = (uint32)ECharacterAttackRestriction::Falling;
	if (movementMode == EMovementMode::MOVE_Falling)
		attackFlag |= falling;
	else
		attackFlag &= ~falling;

	_attackRestriction = (ECharacterAttackRestriction)attackFlag;
}

void UCombatComponent::SubscribeToAniActivityChange()
{
	if (!_owner)
		return;

	auto aniActivity = _owner->FindComponentByClass<UAniActivityComponent>();
	if (!aniActivity)
		return;

	aniActivity->GetOnAniActivityChange().AddUObject(this, &UCombatComponent::HandleAniActivityChange);
}

void UCombatComponent::HandleAniActivityChange(EAniActivityState aniActivityState)
{
	UpdateAttackRestriction(aniActivityState);
	CheckAndSetWeaponCollision();
}

void UCombatComponent::CheckAndSetWeaponCollision()
{
	const uint32 CollisionBlockingMask =
		(uint32)ECharacterAttackRestriction::AniActivity |
		(uint32)ECharacterAttackRestriction::Falling;

	const bool bShouldDisable = !((uint32)_attackRestriction & CollisionBlockingMask) &&
		_attackRestriction != ECharacterAttackRestriction::None;
	const bool bIsNoneOrUnequip = ((uint32)_attackRestriction == 0) || // None 상태 (값이 0)
		((uint32)_attackRestriction & (uint32)ECharacterAttackRestriction::Unequip); // Unequip 플래그 설정 확인

	if (bIsNoneOrUnequip)
		SetWeaponCollisionEnabled(false);
}

void UCombatComponent::UpdateAttackRestriction(EAniActivityState aniActivityState)
{
	auto attackFlag = (uint32)_attackRestriction;
	const uint32 aniActivity = (uint32)ECharacterAttackRestriction::AniActivity;
	switch (aniActivityState)
	{
	case EAniActivityState::EAS_Inactive:
		attackFlag &= ~aniActivity;
		break;
	case EAniActivityState::EAS_Active:
		attackFlag |= aniActivity;
		break;
	default:
		break;
	}
	_attackRestriction = (ECharacterAttackRestriction)attackFlag;
}

void UCombatComponent::UnsubscribeToAniActivityChange()
{
	if (!_owner)
		return;
	
	auto aniActivity = _owner->FindComponentByClass<UAniActivityComponent>();
	if (!aniActivity)
		return;

	aniActivity->GetOnAniActivityChange().RemoveAll(this);
}

bool UCombatComponent::CanControlAttack()
{
	return  _attackRestriction == ECharacterAttackRestriction::None; 
}

void UCombatComponent::PlayTargetMontage(UAnimMontage* targetMontage, const FName& sectionName)
{
	if (!targetMontage)
		return;

	auto animPlayer = _owner->GetComponentByClass<UAnimPlayerComponent>();
	if (!animPlayer)
		return;
	
	animPlayer->PlayTargetMontage(targetMontage, sectionName);
}

void UCombatComponent::PlayAttackMontage()
{
	PlayTargetMontage(_attackMontage, GetRandomAttackSectionName());
}

FName UCombatComponent::GetRandomAttackSectionName()
{
	if (_attackMontageSections.IsEmpty())
		return FName();

	int32 selection = FMath::RandRange(0, _attackMontageSections.Num() - 1);
	return _attackMontageSections[selection];
}

void UCombatComponent::SetWeaponCollisionEnabled(bool bEnabled)
{
	if (!_owner)
		return;

	auto equipment = _owner->GetComponentByClass<UEquipmentComponent>();
	if (!equipment)
		return;

	equipment->UpdateWeaponCollision(bEnabled);
}

void UCombatComponent::PlayTargetSFX(USoundBase* targetSFX, const FVector& targetLocation)
{
	if (!_owner)
		return;

	auto sfxComponent = _owner->GetComponentByClass<UCharacterSFXComponent>();
	if (!sfxComponent)
		return;

	sfxComponent->PlaySFX(targetSFX, targetLocation);
}

void UCombatComponent::PlayTargetVFX(UParticleSystem* targetVFX, const FVector& targetLocation)
{
	if (!_owner)
		return;

	auto vfxComponent = _owner->GetComponentByClass<UCharacterVFXComponent>();
	if (!vfxComponent)
		return;

	vfxComponent->PlayVFX(targetVFX, targetLocation);
}

void UCombatComponent::PlayHitMontage(const FVector& hitPoint)
{
	PlayTargetMontage(_hitMontage, GetHitDirection(hitPoint));
}

ECharacterCondition UCombatComponent::GetOwnerCondition()
{
	if (!_owner)
		return ECharacterCondition::None;

	auto attribute = _owner->GetComponentByClass<UCharacterAttributeComponent>();
	if (!attribute)
		return ECharacterCondition::None;

	return attribute->GetCharacterCondition();
}

FName UCombatComponent::GetHitDirection(const FVector& hitPoint)
{
	const FVector forward = GetOwner()->GetActorForwardVector();
	const FVector actorLocation = GetOwner()->GetActorLocation();
	const FVector hitPointLowered(hitPoint.X, hitPoint.Y, actorLocation.Z);
	const FVector toHit = (hitPointLowered - actorLocation).GetSafeNormal();

	const double cosTheta = FVector::DotProduct(forward, toHit);
	double radian = FMath::Acos(cosTheta);
	double degree = FMath::RadiansToDegrees(radian);

	const FVector cross = FVector::CrossProduct(forward, toHit);
	if (cross.Z < 0)
		degree *= -1.f;

	if (degree >= -45.f && degree < 45.f)
		return FName("FromFront");
	else if (degree >= -135.f && degree < -45.f)
		return FName("FromLeft");
	else if (degree >= 45.f && degree < 135.f)
		return FName("FromRight");
	else
		return FName("FromBack");
}

void UCombatComponent::HandleTakeDamage(float damage, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
	HandleDamageValue(damage);
}

void UCombatComponent::HandleDamageValue(float damage)
{
	if (!_owner)
		return;

	auto attribute = _owner->FindComponentByClass<UCharacterAttributeComponent>();
	if (!attribute)
		return;

	attribute->UpdateHealth(-damage);
}

void UCombatComponent::SubscribeToEventPropagation()
{
	if (!_owner)
		return;

	auto eventPropagation = _owner->FindComponentByClass<UActorEventPropagationComponent>();
	if (!eventPropagation)
		return;

	eventPropagation->GetOnHitEvent().AddUObject(this, &UCombatComponent::HandleHitEvent);
	eventPropagation->GetOnDamageApplied().AddUObject(this, &UCombatComponent::HandleTakeDamage);
}

void UCombatComponent::UnsubscribeToEventPropagation()
{
	if (!_owner)
		return;

	auto eventPropagation = _owner->FindComponentByClass<UActorEventPropagationComponent>();
	if (!eventPropagation)
		return;

	eventPropagation->GetOnHitEvent().RemoveAll(this);
	eventPropagation->GetOnDamageApplied().RemoveAll(this);
}
