// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickUpItemComponent.generated.h"

class AItem;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPickedTreasureAdded, int32);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UPickUpItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickUpItemComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ACharacter> _owner = nullptr;
	FORCEINLINE void SetOwner() { _owner = Cast<ACharacter>(GetOwner()); }

private:
	void SubscribeToEventPropagation();
	void HandlePickedUpItem(class AItem* item);
	void UnsubscribeToEventPropagation();

public:
	FORCEINLINE int32 GetPickedTreasureValue() { return _pickedTreasureValue; }
private:
	int32 _pickedTreasureValue = 0;
	void AddTreasureValue(int32 treasureValue);
	void HandlePickedUpTreasure(class AItem* item);

public:
	FORCEINLINE FOnPickedTreasureAdded& GetOnPickedTreasureAdded() { return _onPickedTreasureAdded; }
	FORCEINLINE const FOnPickedTreasureAdded& GetOnPickedTreasureAdded() const { return _onPickedTreasureAdded; }
private:
	FOnPickedTreasureAdded _onPickedTreasureAdded;
	FORCEINLINE void BroadcastPickedTreasureAdded() { _onPickedTreasureAdded.Broadcast(_pickedTreasureValue); }
};
