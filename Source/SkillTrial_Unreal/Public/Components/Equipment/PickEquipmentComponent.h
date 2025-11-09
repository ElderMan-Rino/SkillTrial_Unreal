// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/ActionInputType.h"
#include "PickEquipmentComponent.generated.h"

class AItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UPickEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickEquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ATrialCharacter> _owner;
	FORCEINLINE void SetOwner() { _owner = Cast<ATrialCharacter>(GetOwner()); }

public:
	FORCEINLINE void SetGroundItem(AItem* groundItem) { _groundItem = groundItem; }
private:
	UPROPERTY(VisibleInstanceOnly)
	AItem* _groundItem = nullptr;

private:
	void SubscribeToActionInput();
	void HandleActionInput(EActionInputType actionInputType);
	void HandleEKeyPressed();
	void EquipGroundItem();
	void UnsubscribeToActionInput();
};
