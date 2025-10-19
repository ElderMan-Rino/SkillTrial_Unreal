#pragma once

UENUM(BlueprintType)
enum class ECharacterEquipState : uint8
{
    ECS_Unequipped UMETA(DisplayName = "Unequipped"),
    ECS_EquippedOneHanded UMETA(DisplayName = "Equipped OneHanded"),
    ECS_EquippedTwoHande UMETA(DisplayName = "Equipped TwoHanded")
};