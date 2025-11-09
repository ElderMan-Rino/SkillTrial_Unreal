#pragma once

UENUM()
enum class ECharacterAttackRestriction : uint32
{
    None = 0,
    AniActivity = 1,
    Unequip = 1 << 1,
    Falling = 1 << 2,
    Hit = 1 << 3,
    Dead = 1 << 4,
};