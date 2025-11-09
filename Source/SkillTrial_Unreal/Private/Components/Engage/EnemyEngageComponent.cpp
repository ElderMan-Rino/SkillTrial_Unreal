// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Engage/EnemyEngageComponent.h"
#include "Components/Chase/EnemyChaseComponent.h"
#include "Components/State/EnemyPatternStateComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Components/Attribute/CharacterAttributeComponent.h"
#include "Enums/CharacterCondition.h"
#include "Enemy/Enemy.h"
#include "Characters/TrialCharacter.h"
#include "Helpers/VectorHelpers.h"

UEnemyEngageComponent::UEnemyEngageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyEngageComponent::BeginPlay()
{
	Super::BeginPlay();
	SetOwner();
	SubscribeToPatternState();
	SubscribeToChase();
}

void UEnemyEngageComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnsubscribeToChase();
	UnsubscribeToPatternState();
	Super::EndPlay(EndPlayReason);
}

void UEnemyEngageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateEngage(DeltaTime);
}

void UEnemyEngageComponent::UpdateEngage(float deltaTime)
{
	if (!_owner || !_engageTarget)
		return;

	if (!CanControlAttack())
		return;

	if (IsTargetValidAndAlive() && VectorHelpers::IsTargetInRange(_engageTarget, _owner, _engageRadius))
	{
		if(IsTargetInFrontAngle())
			ExecuteCombatAttack();
		else
			LookAtTarget(deltaTime);
	}
	else
	{
		ClearEngageTarget();
		RequestChangeToChase();
	}
}

bool UEnemyEngageComponent::IsTargetInFrontAngle() const
{
	if (!_engageTarget || !_owner)
		return false;

	// 1. 타겟 방향 벡터 계산 (수평만)
	FVector DirectionToTarget = _engageTarget->GetActorLocation() - _owner->GetActorLocation();
	DirectionToTarget.Z = 0.0f;
	DirectionToTarget.Normalize();

	// 2. 오너의 전방 벡터 (Forward Vector)
	FVector OwnerForward = _owner->GetActorForwardVector();
	OwnerForward.Z = 0.0f; // 수평 계산을 위해 Z축 무시

	// 3. 내적(Dot Product) 계산
	// 두 정규화된 벡터의 내적은 cos(θ) 값입니다.
	const float DotProduct = FVector::DotProduct(OwnerForward, DirectionToTarget);

	// 4. 각도 비교
	// cos(45도) = 0.707107입니다. (90도 범위 = 정면에서 좌우 45도까지)
	const float CosHalfAngle = FMath::Cos(FMath::DegreesToRadians(15.f));

	// 내적 값이 0.707보다 커야 45도 이내에 있습니다.
	return DotProduct >= CosHalfAngle;
}

bool UEnemyEngageComponent::CanControlAttack()
{
	if (!_owner)
		return false;

	auto combat = _owner->GetComponentByClass<UEnemyCombatComponent>();
	if (!combat)
		return false;

	return combat->CanControlAttack();
}

void UEnemyEngageComponent::LookAtTarget(float deltaTime)
{
	if (!_engageTarget || !_owner)
		return;

	// 1. 목표 회전(Target Rotation) 계산 (기존 로직)
	const FVector TargetLocation = _engageTarget->GetActorLocation();
	const FVector OwnerLocation = _owner->GetActorLocation();

	FVector Direction = TargetLocation - OwnerLocation;
	Direction.Z = 0.0f; // 수평 회전만 계산

	if (Direction.IsNearlyZero()) // 타겟이 너무 가까워 방향 계산이 불가능할 경우 예외 처리
	{
		return;
	}

	Direction.Normalize();

	const FRotator TargetRotation = Direction.Rotation();
	const FRotator CurrentRotation = _owner->GetActorRotation();
	const float InterpSpeed = 10.0f;

	const FRotator NewRotation = FMath::RInterpTo(
		CurrentRotation,
		TargetRotation,
		deltaTime,
		InterpSpeed
	);

	// 4. 보간된 회전 값 적용
	_owner->SetActorRotation(NewRotation);
}

void UEnemyEngageComponent::ExecuteCombatAttack()
{
	if (!_owner)
		return;

	auto combat = _owner->GetComponentByClass<UEnemyCombatComponent>();
	if (!combat)
		return;

	combat->PlayAttackMontage();
}

void UEnemyEngageComponent::RequestChangeToChase()
{
	if (!_owner)
		return;

	auto patternState = _owner->GetComponentByClass<UEnemyPatternStateComponent>();
	if (!patternState)
		return;

	patternState->UpdatePattenState(EEnemyPatternState::Chase);
}

void UEnemyEngageComponent::SubscribeToPatternState()
{
	if (!_owner)
		return;

	auto patternState = _owner->GetComponentByClass<UEnemyPatternStateComponent>();
	if (!patternState)
		return;

	patternState->GetOnPatternStateChange().AddUObject(this, &UEnemyEngageComponent::HandlePatternStateChange);
}

void UEnemyEngageComponent::HandlePatternStateChange(EEnemyPatternState enemyPatternState)
{
	if (enemyPatternState != EEnemyPatternState::Engage)
		ClearEngageTarget();
	SetEngageTickEnabled(enemyPatternState == EEnemyPatternState::Engage);
}

void UEnemyEngageComponent::UnsubscribeToPatternState()
{
	if (!_owner)
		return;

	auto patternState = _owner->GetComponentByClass<UEnemyPatternStateComponent>();
	if (!patternState)
		return;

	patternState->GetOnPatternStateChange().RemoveAll(this);
}

void UEnemyEngageComponent::SubscribeToChase()
{
	if (!_owner)
		return;

	auto chase = _owner->GetComponentByClass<UEnemyChaseComponent>();
	if (!chase)
		return;

	chase->GetOnTargetInEngageRange().AddUObject(this, &UEnemyEngageComponent::HandleTargetInEngageRange);
}

void UEnemyEngageComponent::HandleTargetInEngageRange(ATrialCharacter* chaseTarget)
{
	UpdateEngageTarget(chaseTarget);
}

void UEnemyEngageComponent::UnsubscribeToChase()
{
	if (!_owner)
		return;

	auto chase = _owner->GetComponentByClass<UEnemyChaseComponent>();
	if (!chase)
		return;

	chase->GetOnTargetInEngageRange().RemoveAll(this);
}

bool UEnemyEngageComponent::IsTargetValidAndAlive() const
{
	if (!_engageTarget)
		return false;

	auto attribute = _engageTarget->GetComponentByClass<UCharacterAttributeComponent>();
	if (!attribute)
		return false;

	return attribute->IsTargetCondition(ECharacterCondition::Alive);
}

