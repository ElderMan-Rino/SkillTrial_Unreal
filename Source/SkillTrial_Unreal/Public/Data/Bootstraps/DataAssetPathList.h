#pragma once

#include "CoreMinimal.h"
#include "DataAssetPathList.generated.h"

USTRUCT(BlueprintType)
struct FDataAssetPathList
{
    GENERATED_BODY()

    // 2. TSoftObjectPtr<UPrimaryDataAsset>을 포함하는 배열
    // UPROPERTY로 선언하여 에디터에서 편집 가능하게 만듭니다.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Path List")
    TArray<TSoftObjectPtr<UPrimaryDataAsset>> Paths;

    // FDataAssetPathList가 Map의 값으로 사용될 때 기본 생성자가 필요합니다.
    FDataAssetPathList() {}
};