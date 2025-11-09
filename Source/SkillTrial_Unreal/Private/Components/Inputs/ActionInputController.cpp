// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inputs/ActionInputController.h"
#include "Enums/ActionInputType.h"

// Sets default values for this component's properties
UActionInputController::UActionInputController()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UActionInputController::BindActionInputs(UInputComponent* playerInputComponent)
{
	if (!playerInputComponent) return;

	playerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &UActionInputController::HandleJump);
	playerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &UActionInputController::HandleEKeyPressed);
	playerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &UActionInputController::HandleAttack);
}

void UActionInputController::HandleJump()
{
	BroadcastActionInput(EActionInputType::Jump);
}

void UActionInputController::HandleEKeyPressed()
{
	BroadcastActionInput(EActionInputType::EKeyPressed);
	BroadcastPostActionInput(EActionInputType::EKeyPressed);
}

void UActionInputController::HandleAttack()
{
	BroadcastActionInput(EActionInputType::Attack);
}