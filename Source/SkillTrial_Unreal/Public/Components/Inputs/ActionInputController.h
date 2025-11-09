// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/ActionInputType.h"
#include "ActionInputController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnActionInput, EActionInputType /*Axis*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UActionInputController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionInputController();

public:
	void BindActionInputs(UInputComponent* playerInputComponent);
private:
	void HandleJump();
	void HandleEKeyPressed();
	void HandleAttack();

public:
	FORCEINLINE FOnActionInput& GetOnActionInput() { return _onActionInput; }
	FORCEINLINE const FOnActionInput& GetOnActionInput() const { return _onActionInput; }
private:
	FOnActionInput _onActionInput;
	FORCEINLINE void BroadcastActionInput(EActionInputType actionInputType) { _onActionInput.Broadcast(actionInputType); }

public:
	FORCEINLINE FOnActionInput& GetOnPostActionInput() { return _onPostActionInput; }
	FORCEINLINE const FOnActionInput& GetOnPostActionInput() const { return _onPostActionInput; }
private:
	FOnActionInput _onPostActionInput;
	FORCEINLINE void BroadcastPostActionInput(EActionInputType actionInputType) { _onPostActionInput.Broadcast(actionInputType); }
};