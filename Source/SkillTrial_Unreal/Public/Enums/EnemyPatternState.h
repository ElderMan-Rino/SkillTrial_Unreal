#pragma once

UENUM(BlueprintType)
enum class EEnemyPatternState : uint8
{
    None UMETA(DisplayName = "None"),
    Patrol UMETA(DisplayName = "Patrol"),
    Chase UMETA(DisplayName = "Chase"),
    Engage UMETA(DisplayName = "Engage"),
    Dead UMETA(DisplayName = "Dead"),
};