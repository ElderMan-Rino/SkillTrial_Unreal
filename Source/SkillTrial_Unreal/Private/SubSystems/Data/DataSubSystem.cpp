// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/Data/DataSubSystem.h"

void UDataSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	SetupDatas();
	Super::Initialize(Collection);
}

void UDataSubSystem::Deinitialize()
{

	Super::Deinitialize();
}

void UDataSubSystem::SetupGameVersion()
{

}

void UDataSubSystem::SetupDatas()
{

}
