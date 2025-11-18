// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "DataAssetCreator.generated.h"

/**
 * 
 */
UCLASS()
class SKILLTRIAL_UNREAL_API UDataAssetCreator : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
    virtual bool Initialize() override;

private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> _createDataAssetButton;

private:
    TMap<FString, TSubclassOf<UDataAsset>> _dataAssetClasses;
    void SetupDataAssetClasses();

private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UComboBoxString> _dataAssetClassesDropDown;
    void SetupDataAssetDropDown();

protected:
    UFUNCTION(BlueprintCallable, Category = "DataAssetCreator")
    void OnDataAssetSelected(FString selectedClassName);

private:
    TSubclassOf<UDataAsset> _selectedDataAsset;
    void SetSelectedDataAsset(FString selectedClassName);
    TArray<struct FEditablePropertyInfo> GetEditableProperties();

protected:
    UFUNCTION(BlueprintCallable, Category = "DataAssetCreator")
    void SetupPropertyViewsScroll();
private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UScrollBox> _propertyViewsScroll; 
    void ClearPropertyViewScroll();
    TArray<TObjectPtr<class UEditablePropertyViewWidget>> _propertyViews;
    

private:
    UPROPERTY(EditAnywhere, Category = "Property View")
    TSubclassOf<class UEditablePropertyViewWidget> _propertyViewWidgetClass;

protected:
    UFUNCTION(BlueprintCallable, Category = "DataAssetCreator")
    void OnBrowseFolderClicked();
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> _targetBrowserPathText;
private:
    FString _targetFolderPath;
    void SetBrowseFolder();

protected:
    UFUNCTION(BlueprintCallable, Category = "DataAssetCreator")
    void OnCreateButtonClicked();
private:
    void CreateTargetDataAsset();
};
