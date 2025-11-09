// Fill out your copyright notice in the Description page of Project Settings.


#include "Hud/TrialHud.h"
#include "Hud/UIOverlay.h"

void ATrialHud::BeginPlay()
{
	Super::BeginPlay();
	CreateUIOverlayInstance();
}

void ATrialHud::CreateUIOverlayInstance()
{
	if (!_uiOverlay)
		return;

	UWorld* world = GetWorld();
	if (!world)
		return;

	auto playerController = world->GetFirstPlayerController();
	if (!playerController)
		return;
	
	_uiOverlayInstance = CreateWidget<UUIOverlay>(playerController, _uiOverlay);
	_uiOverlayInstance->AddToViewport();
}
