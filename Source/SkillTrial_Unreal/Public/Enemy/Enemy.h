// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enums/KnightDeadAniState.h"
#include "Enums/EnemyState.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UAttributeComponent;
class UHealthBarComponent;
class AAIController;
class UPawnSensingComponent;

UCLASS()
class SKILLTRIAL_UNREAL_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnHit_Implementation(const FVector& hitPoint, AActor* hitter) override;
	virtual float TakeDamage(float damage, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser) override;
	virtual void Destroyed() override;

private:
	UPROPERTY(VisibleAnywhere)
	TArray<UActorComponent*> _registeredSubComponents;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCharacterAttributeComponent> _attribute = nullptr;
private:
	void SetupAttribute();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UEnemyCombatComponent> _enemyCombat = nullptr;
	void SetupEnemyCombat();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UEnemyPatternStateComponent> _patternState = nullptr;
	void SetupPatternState();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UEnemyPatrolComponent> _patrol = nullptr;
	void SetupPatrol();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UEnemyChaseComponent> _chase = nullptr;
	void SetupChase();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UEnemyPawnSensingController> _pawnSensing = nullptr;
	void SetupPawnSensing();


private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UEnemyEngageComponent> _engage = nullptr;
	void SetupEngage();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UEnemyCollisionController> _collisionController = nullptr;
	void SetupCollisionController();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UEnemyMovementController> _movement = nullptr;
	void SetupMovement();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAnimPlayerComponent> _animPlayer = nullptr;
	void SetupAnimPlayer();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAniActivityComponent> _aniActivity = nullptr;
private:
	void SetupAniActivity();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCharacterSFXComponent> _sfx = nullptr;
	void SetupSFX();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCharacterVFXComponent> _vfx = nullptr;
	void SetupVFX();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UEnemyWidgetComponent> _enemyWidget = nullptr;
private:
	void SetupWidget();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UActorEventPropagationComponent> _eventPropagation = nullptr;
	void SetupEventPropagation();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UEnemyDeadComponent> _dead = nullptr;
	void SetupDead();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UEnemyEquipmentComponent> _equipment = nullptr;
	void SetupEquipment();

private:
	void SetEnemyTag();

private:
	void HandleHitEvent(const FVector& hitPoint, AActor* hitter);

private:
	void HandleTakeDamage(float damage, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser);

private:
	void HandleDestroyed();
};
