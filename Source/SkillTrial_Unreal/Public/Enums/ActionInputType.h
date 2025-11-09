#pragma once

UENUM(BlueprintType)
enum class EActionInputType : uint8
{
	Jump UMETA(DisplayName = "Jump"),
	EKeyPressed UMETA(DisplayName = "EKeyPressed"),
	Attack UMETA(DisplayName = "Attack"),
};