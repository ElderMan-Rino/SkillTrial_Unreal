// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/AniActivityState.h"
#include "AniActivityComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAniActivityChanged, EAniActivityState);

class ACharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UAniActivityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAniActivityComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ACharacter> _owner;
	FORCEINLINE void SetOwner() { _owner = Cast<ACharacter>(GetOwner()); }

public:
	FORCEINLINE FOnAniActivityChanged& GetOnAniActivityChange() { return _onAniActivityChange; }
	FORCEINLINE const FOnAniActivityChanged& GetOnAniActivityChange() const { return _onAniActivityChange; }
	UFUNCTION(BlueprintCallable)
	void UpdateAniActivityState(EAniActivityState actionInputType);
private:
	UPROPERTY(VisibleAnywhere)
	EAniActivityState _aniActivityState;
	FOnAniActivityChanged _onAniActivityChange;
	FORCEINLINE void BroadcastAniActivityChange() { _onAniActivityChange.Broadcast(_aniActivityState); }
		
private:
	void SubscribeToEventPropagation();
	void HandleMovementModeChanged(const EMovementMode prevMode, const EMovementMode curMode);
	void UnsubscribeToEventPropagation();
};
