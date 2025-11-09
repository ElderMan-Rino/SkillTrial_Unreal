// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/ActionInputType.h"
#include "Enums/CharacterAttackRestriction.h"
#include "Enums/CharacterEquipState.h"
#include "Enums/AniActivityState.h"
#include "Enums/CharacterCondition.h"
#include "CombatComponent.generated.h"


struct FDamageEvent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ACharacter> _owner;
private:
	FORCEINLINE void SetOwner() { _owner = Cast<ACharacter>(GetOwner()); }

public:
	void UpdateAttackRestriction(EMovementMode movementMode);
protected:
	virtual void HandleAniActivityChange(EAniActivityState aniActivityState);
private:
	void SubscribeToAniActivityChange();
	void UpdateAttackRestriction(EAniActivityState aniActivityState);
	void CheckAndSetWeaponCollision();
	void UnsubscribeToAniActivityChange();

public:
	virtual bool CanControlAttack();
protected:
	FORCEINLINE void UpdateAttackRestriction(ECharacterAttackRestriction attackRestriction) { _attackRestriction = attackRestriction; }
	FORCEINLINE ECharacterAttackRestriction GetAttackRestriction() const { return  _attackRestriction; }
protected:
	UPROPERTY(VisibleAnywhere)
	ECharacterAttackRestriction _attackRestriction = ECharacterAttackRestriction::Unequip;

protected:
	void PlayTargetMontage(class UAnimMontage* targetMontage, const FName& sectionName);

public:
	virtual void PlayAttackMontage();
protected:
	UPROPERTY(EditAnywhere)
	TArray<FName> _attackMontageSections;
	FName GetRandomAttackSectionName();
	UPROPERTY(EditAnywhere, Category = Montage)
	class UAnimMontage* _attackMontage;

protected:
	UFUNCTION(BlueprintCallable, Category = "Combat|Weapon")
	void SetWeaponCollisionEnabled(bool bEnabled);

protected:
	UPROPERTY(EditDefaultsOnly, Category = Montage)
	class UAnimMontage* _hitMontage;
	void PlayHitMontage(const FVector& hitPoint);

protected:
	ECharacterCondition GetOwnerCondition();

protected:
	UPROPERTY(EditAnywhere, Category = SFX)
	class USoundBase* _hitSFX;

protected:
	void PlayTargetSFX(USoundBase* targetSFX, const FVector& targetLocation);

protected:
	UPROPERTY(EditAnywhere, Category = VFX)
	class UParticleSystem* _bloodVFX;

protected:
	void PlayTargetVFX(UParticleSystem* targetVFX, const FVector& targetLocation);

protected:
	FName GetHitDirection(const FVector& hitPoint);

protected:
	virtual void HandleHitEvent(const FVector& hitPoint, AActor* hitter) PURE_VIRTUAL (UCombatComponent::HandleHitEvent, );
	virtual void HandleTakeDamage(float damage, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser);
	void HandleDamageValue(float damage);
	virtual void SubscribeToEventPropagation();
	virtual void UnsubscribeToEventPropagation();
};
