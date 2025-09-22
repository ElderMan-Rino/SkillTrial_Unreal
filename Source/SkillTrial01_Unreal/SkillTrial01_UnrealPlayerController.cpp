// Copyright Epic Games, Inc. All Rights Reserved.


#include "SkillTrial01_UnrealPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "SkillTrial01_UnrealCameraManager.h"
#include "Blueprint/UserWidget.h"
#include "SkillTrial01_Unreal.h"
#include "Widgets/Input/SVirtualJoystick.h"

ASkillTrial01_UnrealPlayerController::ASkillTrial01_UnrealPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = ASkillTrial01_UnrealCameraManager::StaticClass();
}

void ASkillTrial01_UnrealPlayerController::BeginPlay()
{
	Super::BeginPlay();

	
	// only spawn touch controls on local player controllers
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogSkillTrial01_Unreal, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void ASkillTrial01_UnrealPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
	
}
