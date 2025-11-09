#pragma once

UENUM(BlueprintType)
enum class EAxisInputType : uint8
{
	MoveForward UMETA(DisplayName = "MoveForward"),
	MoveRight UMETA(DisplayName = "MoveRight"),
	Turn UMETA(DisplayName = "Turn"),
	LookUp UMETA(DisplayName = "LookUp"),
};