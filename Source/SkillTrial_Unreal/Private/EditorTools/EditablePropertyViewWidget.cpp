// Fill out your copyright notice in the Description page of Project Settings.
#include "EditorTools/EditablePropertyViewWidget.h"
#include "Components/TextBlock.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/Input/SCheckBox.h"
#include "Components/PanelWidget.h"
#include "Components/HorizontalBox.h"
#include "UObject/UnrealType.h"    
#include "UObject/Field.h"
#include "Components/CheckBox.h"
#include "Components/SpinBox.h"
#include "Components/EditableTextBox.h"
#include "UObject/TextProperty.h"

void UEditablePropertyViewWidget::SetPropertyInfo(const FEditablePropertyInfo& info)
{
	SetPropertyType(info);
	SetPropertyName(info);
	SetVisiblePropertyInputs(info);
}

void UEditablePropertyViewWidget::SetPropertyType(const FEditablePropertyInfo& info)
{
	if (!_propertyType)
		return;

	_propertyType->SetText(FText::FromString(info.PropertyType));
}

FName UEditablePropertyViewWidget::GetPropertyName()
{
	auto propertyNameString = _propertyName->GetText().ToString();
	return FName(*propertyNameString);
}

void UEditablePropertyViewWidget::SetPropertyName(const FEditablePropertyInfo& info)
{
	if (!_propertyName)
		return;

	_propertyName->SetText(FText::FromName(info.PropertyName));
	_propertyName->SetToolTipText(FText::FromString(info.Tooltip));
}

bool UEditablePropertyViewWidget::GetBoolInputValue()
{
	return _boolInput->IsChecked();
}

float UEditablePropertyViewWidget::GetNumberInputValue()
{
	return _numberInput->GetValue();
}

FString UEditablePropertyViewWidget::GetTextInputValue()
{
	return _textInput->GetText().ToString();
}

TArray<float> UEditablePropertyViewWidget::GetCompoundInputValue()
{
	auto compoundInputs = TArray<float>();
	compoundInputs.Add(_compoundInputX->GetValue());
	compoundInputs.Add(_compoundInputY->GetValue());
	compoundInputs.Add(_compoundInputZ->GetValue());
	return compoundInputs;
}

void UEditablePropertyViewWidget::SetVisiblePropertyInputs(const FEditablePropertyInfo& info)
{
	_widgetType = GetWidgetType(info);
	if (_boolGroup) _boolGroup->SetVisibility(_widgetType == EPropertyWidgetType::Bool ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	if (_numberGroup) _numberGroup->SetVisibility(_widgetType == EPropertyWidgetType::Number ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	if (_textGroup) _textGroup->SetVisibility(_widgetType == EPropertyWidgetType::Text ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	if (_compoundGroup) _compoundGroup->SetVisibility(_widgetType == EPropertyWidgetType::Compound ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

UEditablePropertyViewWidget::EPropertyWidgetType UEditablePropertyViewWidget::GetWidgetType(const FEditablePropertyInfo& info)
{
	auto propertyType = info.PropertyType;
    if (propertyType == TEXT("BoolProperty"))
        return EPropertyWidgetType::Bool;
    else if (propertyType == TEXT("IntProperty") || propertyType == TEXT("FloatProperty") || propertyType == TEXT("DoubleProperty"))
        return EPropertyWidgetType::Number;
    else if (propertyType == TEXT("NameProperty") || propertyType == TEXT("StrProperty") || propertyType == TEXT("TextProperty"))
        return EPropertyWidgetType::Text;
    else if (propertyType == TEXT("StructProperty"))
        return EPropertyWidgetType::Compound;
    else
        return EPropertyWidgetType::None;
}

void UEditablePropertyViewWidget::CopyInputValueToMemory(void* destinationPtr, FProperty* targetProperty)
{
	if (!destinationPtr)
		return;

	switch (_widgetType)
	{
	case EPropertyWidgetType::Bool:
	{
		bool Value = GetBoolInputValue();
		FMemory::Memcpy(destinationPtr, &Value, sizeof(bool));
		break;
	}
	case EPropertyWidgetType::Number:
	{
		// Data Asset의 float/int 프로퍼티에 float 값을 복사합니다.
		// float는 가장 넓은 범위의 Number 타입으로 간주합니다.
		float Value = GetNumberInputValue();

		// DestinationPtr가 가리키는 실제 프로퍼티의 크기(float, int 등)에 따라 
		// sizeof(float) 대신 TargetProperty의 크기를 사용해야 더 안전합니다.
		// 여기서는 위젯의 입력 타입인 float 크기로 복사합니다.
		FMemory::Memcpy(destinationPtr, &Value, sizeof(float));
		break;
	}
	case EPropertyWidgetType::Text:
		CopyInputToTextTypes(destinationPtr, targetProperty);
		break;
	case EPropertyWidgetType::Compound:
	{
		TArray<float> Values = GetCompoundInputValue();
		if (FVector* VectorPtr = static_cast<FVector*>(destinationPtr))
		{
			if (Values.Num() >= 3)
			{
				VectorPtr->X = Values[0];
				VectorPtr->Y = Values[1];
				VectorPtr->Z = Values[2];
			}
		}
		break;
	}

	default:
		UE_LOG(LogTemp, Warning, TEXT("CopyInputValueToMemory: Unsupported Widget Type: %d"), (int32)_widgetType);
		break;
	}
}

void UEditablePropertyViewWidget::CopyInputToTextTypes(void* destinationPtr, FProperty* targetProperty)
{
	if (FStrProperty* StrProp = CastField<FStrProperty>(targetProperty))
	{
		// FString 프로퍼티: 안전하게 FString 대입
		FString NewValue = GetTextInputValue();
		StrProp->SetPropertyValue(destinationPtr, NewValue);
		return;
	}
	if (FNameProperty* NameProp = CastField<FNameProperty>(targetProperty))
	{
		// FName 프로퍼티: 안전하게 FName 대입
		FName NewValue = FName(*GetTextInputValue());
		NameProp->SetPropertyValue(destinationPtr, NewValue);
		return;
	}
	if (FTextProperty* TextProp = CastField<FTextProperty>(targetProperty))
	{
		// FText 프로퍼티: 안전하게 FText 대입 (FText는 FString 변환 로직이 복잡할 수 있으므로 주의 필요)
		FText NewValue = FText::FromString(GetTextInputValue());
		TextProp->SetPropertyValue(destinationPtr, NewValue);
		return;

	}
}
