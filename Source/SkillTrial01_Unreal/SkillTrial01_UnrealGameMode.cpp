// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkillTrial01_UnrealGameMode.h"
#include "SkillTrial01_UnrealCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASkillTrial01_UnrealGameMode::ASkillTrial01_UnrealGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
