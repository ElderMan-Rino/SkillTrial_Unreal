// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Data/EditorTools/EditablePropertyInfo.h"
#include "EditablePropertyViewWidget.generated.h"

UCLASS()
class SKILLTRIAL_UNREAL_API UEditablePropertyViewWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
    void SetPropertyInfo(const FEditablePropertyInfo& info);
private:
   UPROPERTY(meta = (BindWidget))
    class UTextBlock* _propertyType;
    void SetPropertyType(const FEditablePropertyInfo& info);

public:
    FName GetPropertyName();
private:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* _propertyName;
    void SetPropertyName(const FEditablePropertyInfo& info);

public:
    enum EPropertyWidgetType
    {
        None,
        Bool,
        Number,
        Text,
        Compound,
        Max,
    };
    bool GetBoolInputValue();
    float GetNumberInputValue();
    FString GetTextInputValue();
    TArray<float> GetCompoundInputValue();
private:
    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox* _boolGroup;
    UPROPERTY(meta = (BindWidget))
    class UCheckBox* _boolInput;
    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox* _numberGroup;
    UPROPERTY(meta = (BindWidget))
    class USpinBox* _numberInput;
    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox* _textGroup;
    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* _textInput;
    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox* _compoundGroup;
    UPROPERTY(meta = (BindWidget))
    class USpinBox* _compoundInputX;
    UPROPERTY(meta = (BindWidget))
    class USpinBox* _compoundInputY;
    UPROPERTY(meta = (BindWidget))
    class USpinBox* _compoundInputZ;
    void SetVisiblePropertyInputs(const FEditablePropertyInfo& info);
    EPropertyWidgetType GetWidgetType(const FEditablePropertyInfo& info);
    EPropertyWidgetType _widgetType;

public:
    void CopyInputValueToMemory(void* destinationPtr, FProperty* targetProperty);
private:
    void CopyInputToTextTypes(void* destinationPtr, FProperty* targetProperty);
};
