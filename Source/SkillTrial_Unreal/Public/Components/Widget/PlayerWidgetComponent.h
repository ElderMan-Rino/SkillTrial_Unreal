// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerWidgetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UPlayerWidgetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerWidgetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ACharacter> _owner = nullptr;
	FORCEINLINE void SetOwner() { _owner = Cast<ACharacter>(GetOwner()); }

private:
	void BindUIOverlay();
	class UUIOverlay* _uiOverlay;

private:
	void SubscribeToAttribute();
	void HandleHealthChanged();
	void HandleStaminaChanged(float cur, float max, float per);
	void UpdateHealthWidgets();
	void UpdateStaminaWidgets(float cur, float max, float per);
	void UnsubscribeToAttribute();
	void InitializeHealthWidget();
	void InitializeStaminaWidget();

private:
	void SubscribeToPickedTreasure();
	void HandleTreasureAdded(int32 addedTresureValue);
	void UpdateTreasureWidget(int32 addedTresureValue);
	void UnsubscribeToPickedTreasure();
	void InitializeTreasureWidget();
};
