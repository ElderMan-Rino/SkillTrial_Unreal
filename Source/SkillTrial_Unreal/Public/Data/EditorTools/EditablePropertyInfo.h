#pragma once

#include "CoreMinimal.h"
#include "EditablePropertyInfo.generated.h"

USTRUCT(BlueprintType)
struct FEditablePropertyInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Property Info")
    FName PropertyName; // 변수 이름 (Name)

    UPROPERTY(BlueprintReadOnly, Category = "Property Info")
    FString PropertyType; // 변수 타입 (예: "IntProperty", "FStringProperty")

    UPROPERTY(BlueprintReadOnly, Category = "Property Info")
    FString Tooltip; // 변수 툴팁 (Description)
};