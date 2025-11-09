#pragma once

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
    Patrol UMETA(DisplayName = "Patrol"),
    Chase UMETA(DisplayName = "Chase"),
    Engage UMETA(DisplayName = "Engage"),
    Dead UMETA(DisplayName = "Dead"),
};