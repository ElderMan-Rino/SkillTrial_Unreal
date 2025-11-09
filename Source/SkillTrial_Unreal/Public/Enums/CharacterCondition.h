#pragma once

UENUM(BlueprintType)
enum class ECharacterCondition : uint8
{
	Alive UMETA(DisplayName = "Alive"),
	Dead UMETA(DisplayName = "Dead"),
	None UMETA(DisplayName = "None"),
};