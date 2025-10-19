#pragma once

UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
   EAS_Unoccpled UMETA(DisplayName = "Unoccpled"),
   EAS_Attacking UMETA(DisplayName = "Attacking"),
   Equipping UMETA(DisplayName = "Equipping"),
};