#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

namespace VectorHelpers
{
    static double GetDistanceBetweenActors(const AActor* TargetA, const AActor* TargetB)
    {
        if (!TargetA || !TargetB)
            return -1.0;

        return (TargetB->GetActorLocation() - TargetA->GetActorLocation()).Size();
    }

    /**
     * 대상이 범위 내에 있는지 확인합니다.
     */
    static bool IsTargetInRange(const AActor* Target, const AActor* Instigator, double Range)
    {
        if (Target == nullptr || Instigator == nullptr)
            return false;

        const double Distance = GetDistanceBetweenActors(Instigator, Target);
        return Distance <= Range;
    }
}