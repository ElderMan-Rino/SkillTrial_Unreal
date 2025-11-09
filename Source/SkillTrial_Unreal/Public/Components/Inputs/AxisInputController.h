// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/AxisInputType.h"
#include "AxisInputController.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAxisInput, EAxisInputType /*Axis*/, float /*Value*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UAxisInputController : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAxisInputController();		

public:
	void BindAxisInputs(UInputComponent* PlayerInputComponent);
private:
	void HandleMoveForward(float value);
	void HandleMoveRight(float value);
	void HandleTurn(float value);
	void HandleLookUp(float value);

public:
	FORCEINLINE FOnAxisInput& GetOnAxisInput() { return _onAxisInput; }
	FORCEINLINE const FOnAxisInput& GetOnAxisInput() const { return _onAxisInput; }
private:
	FOnAxisInput _onAxisInput;
	void BroadcastAxisInput(EAxisInputType axisType, float value);
};
