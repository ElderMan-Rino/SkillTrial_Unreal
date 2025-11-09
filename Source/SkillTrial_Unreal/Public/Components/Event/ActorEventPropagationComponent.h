// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorEventPropagationComponent.generated.h"

struct FDamageEvent;
class AItem;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHitEvent, const FVector&, AActor*);
DECLARE_MULTICAST_DELEGATE_FourParams(FOnDamageApplied, float, const FDamageEvent&, AController*, AActor*);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMovementModeChanged, const EMovementMode, const EMovementMode);
DECLARE_MULTICAST_DELEGATE(FOnPropagatedActorDestroyed);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPickedUpItem, AItem*);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UActorEventPropagationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorEventPropagationComponent();

public:
	FORCEINLINE FOnHitEvent& GetOnHitEvent() { return _onHitEvent; }
	FORCEINLINE const FOnHitEvent& GetOnHitEvent() const { return _onHitEvent; }
	FORCEINLINE void BroadcastHitEvent(const FVector& hitPoint, AActor* hitter) { _onHitEvent.Broadcast(hitPoint, hitter); }
private:
	FOnHitEvent _onHitEvent;

public:
	FORCEINLINE FOnDamageApplied& GetOnDamageApplied() { return _onDamageApplied; }
	FORCEINLINE const FOnDamageApplied& GetOnDamageApplied() const { return _onDamageApplied; }
	void BroadcastDamageApplied(float damage, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser);
private:
	FOnDamageApplied _onDamageApplied;

public:
	FORCEINLINE FOnMovementModeChanged& GetOnMovementModeChanged() { return _onMovementModeChanged; }
	FORCEINLINE const FOnMovementModeChanged& GetMovementModeChanged() const { return _onMovementModeChanged; }
	FORCEINLINE void BroadcastMovementModeChanged(const EMovementMode prevMode, const EMovementMode curMode) { _onMovementModeChanged.Broadcast(prevMode, curMode); }
private:
	FOnMovementModeChanged _onMovementModeChanged;

public:
	FORCEINLINE FOnPropagatedActorDestroyed& GetOnActorDestroyed() { return _onActorDestroyed; }
	FORCEINLINE const FOnPropagatedActorDestroyed& GetActorDestroyed() const { return _onActorDestroyed; }
	FORCEINLINE void BroadcastActorDestroyed() { _onActorDestroyed.Broadcast(); }
private:
	FOnPropagatedActorDestroyed _onActorDestroyed;

public:
	FORCEINLINE FOnPickedUpItem& GetOnPickedUpItem() { return _onPickedUpItemEvent; }
	FORCEINLINE const FOnPickedUpItem& GetOnPickedUpItem() const { return _onPickedUpItemEvent; }
	FORCEINLINE void BroadcastPickedUpItem(AItem* pickedItem) { _onPickedUpItemEvent.Broadcast(pickedItem); }
private:
	FOnPickedUpItem _onPickedUpItemEvent;
};