#pragma once

#include "CoreMinimal.h"
#include "PropertySetEntry.generated.h"

USTRUCT(BlueprintType)
struct FPropertySetEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Property Value")
    FName PropertyName; // 설정할 변수 이름

    UPROPERTY(BlueprintReadWrite, Category = "Property Value")
    FString StringValue; // 설정된 변수 값 (문자열로 변환된 값)
};