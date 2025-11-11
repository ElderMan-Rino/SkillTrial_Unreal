// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Enemy.h"
#include "Components/State/EnemyPatternStateComponent.h"
#include "Components/Attribute/CharacterAttributeComponent.h"
#include "Components/Collision/EnemyCollisionController.h"
#include "Components/Movement/EnemyMovementController.h"
#include "Components/VFX/CharacterVFXComponent.h"
#include "Components/SFX/CharacterSFXComponent.h"
#include "Components/Animation/AnimPlayerComponent.h"
#include "Components/Animation/AniActivityComponent.h"
#include "Components/Widget/EnemyWidgetComponent.h"
#include "Components/Event/ActorEventPropagationComponent.h"
#include "Components/PawnSensing/EnemyPawnSensingController.h"
#include "Components/Patrol/EnemyPatrolComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Components/Chase/EnemyChaseComponent.h"
#include "Components/Engage/EnemyEngageComponent.h"
#include "Components/Dead/EnemyDeadComponent.h"
#include "Components/Equipment/EnemyEquipmentComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	SetupAttribute();
	SetupEnemyCombat();
	SetupPatternState();
	SetupPatrol();
	SetupChase();
	SetupPawnSensing();
	SetupEngage();
	SetupCollisionController();
	SetupMovement();
	SetupAnimPlayer();
	SetupAniActivity();
	SetupSFX();
	SetupVFX();
	SetupWidget();
	SetupEventPropagation();
	SetupDead();
	SetupEquipment();
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	SetEnemyTag();
}

void AEnemy::OnHit_Implementation(const FVector& hitPoint, AActor* hiiter)
{
	HandleHitEvent(hitPoint, hiiter);
}

float AEnemy::TakeDamage(float damage, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
	HandleTakeDamage(damage, damageEvent, eventInstigator, damageCauser);
	return damage;
}

void AEnemy::Destroyed()
{
	HandleDestroyed();
}

void AEnemy::SetupAttribute()
{
	_attribute = CreateDefaultSubobject<UCharacterAttributeComponent>("Attribute");
}

void AEnemy::SetupEnemyCombat()
{
	_enemyCombat = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("EnemyCombat"));
}

void AEnemy::SetupPatternState()
{
	_patternState = CreateDefaultSubobject<UEnemyPatternStateComponent>(TEXT("PatternState"));
}

void AEnemy::SetupPatrol()
{
	_patrol = CreateDefaultSubobject<UEnemyPatrolComponent>(TEXT("Patrol"));
}

void AEnemy::SetupChase()
{
	_chase = CreateDefaultSubobject<UEnemyChaseComponent>(TEXT("Chase"));
}

void AEnemy::SetupPawnSensing()
{
	_pawnSensing = CreateDefaultSubobject<UEnemyPawnSensingController>(TEXT("PawnSensingController"));
	_pawnSensing->Setup(this, _registeredSubComponents);
}

void AEnemy::SetupEngage()
{
	_engage = CreateDefaultSubobject<UEnemyEngageComponent>(TEXT("Engage"));
}

void AEnemy::SetupCollisionController()
{
	_collisionController = CreateDefaultSubobject<UEnemyCollisionController>(TEXT("CollisionController"));
	_collisionController->Setup(this);
}
void AEnemy::SetupMovement()
{
	_movement = CreateDefaultSubobject<UEnemyMovementController>(TEXT("MovementController"));
	_movement->Setup(this);
}

void AEnemy::SetupAnimPlayer()
{
	_animPlayer = CreateDefaultSubobject<UAnimPlayerComponent>(TEXT("AnimPlayer"));
}

void AEnemy::SetupAniActivity()
{
	_aniActivity = CreateDefaultSubobject<UAniActivityComponent>(TEXT("AniActivity"));
}

void AEnemy::SetupSFX()
{
	_sfx = CreateDefaultSubobject<UCharacterSFXComponent>(TEXT("SFX"));
}

void AEnemy::SetupVFX()
{
	_vfx = CreateDefaultSubobject<UCharacterVFXComponent>(TEXT("VFX"));
}

void AEnemy::SetupWidget()
{
	_enemyWidget = CreateDefaultSubobject<UEnemyWidgetComponent>(TEXT("EnemyWidget"));
	_enemyWidget->Setup(this, _registeredSubComponents);
}

void AEnemy::SetupEventPropagation()
{
	_eventPropagation = CreateDefaultSubobject<UActorEventPropagationComponent>(TEXT("EventPropagation"));
}

void AEnemy::SetupDead()
{
	_dead = CreateDefaultSubobject<UEnemyDeadComponent>(TEXT("Dead"));
}

void AEnemy::SetupEquipment()
{
	_equipment = CreateDefaultSubobject<UEnemyEquipmentComponent>(TEXT("Equipment"));
}

void AEnemy::SetEnemyTag()
{
	Tags.Add(FName("Enemy"));
}

void AEnemy::HandleHitEvent(const FVector& hitPoint, AActor* hitter)
{
	if (!_eventPropagation)
		return;

	_eventPropagation->BroadcastHitEvent(hitPoint, hitter);
}

void AEnemy::HandleTakeDamage(float damage, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
	if (!_eventPropagation)
		return;

	_eventPropagation->BroadcastDamageApplied(damage, damageEvent, eventInstigator, damageCauser);
}

void AEnemy::HandleDestroyed()
{
	if (!_eventPropagation)
		return;

	_eventPropagation->BroadcastActorDestroyed();
}
