// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inputs/AxisInputController.h"
#include "Enums/AxisInputType.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UAxisInputController::UAxisInputController()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAxisInputController::BindAxisInputs(UInputComponent* playerInputComponent)
{
	if (!playerInputComponent) return;

	playerInputComponent->BindAxis(FName("MoveForward"), this, &UAxisInputController::HandleMoveForward);
	playerInputComponent->BindAxis(FName("MoveRight"), this, &UAxisInputController::HandleMoveRight);
	playerInputComponent->BindAxis(FName("Turn"), this, &UAxisInputController::HandleTurn);
	playerInputComponent->BindAxis(FName("LookUp"), this, &UAxisInputController::HandleLookUp);
}

void UAxisInputController::HandleMoveForward(const float value)
{
	BroadcastAxisInput(EAxisInputType::MoveForward, value);
}

void UAxisInputController::HandleMoveRight(const float value)
{
	BroadcastAxisInput(EAxisInputType::MoveRight, value);
}

void UAxisInputController::HandleTurn(const float value)
{
	BroadcastAxisInput(EAxisInputType::Turn, value);
}

void UAxisInputController::HandleLookUp(const float value)
{
	BroadcastAxisInput(EAxisInputType::LookUp, value);
}

void UAxisInputController::BroadcastAxisInput(EAxisInputType axisType, float value)
{
	_onAxisInput.Broadcast(axisType, value);
}
