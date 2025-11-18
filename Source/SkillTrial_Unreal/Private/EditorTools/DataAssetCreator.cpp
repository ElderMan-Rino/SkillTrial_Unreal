// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorTools/DataAssetCreator.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/ComboBoxString.h"
#include "Data/EditorTools/PropertySetEntry.h"
#include "Data/EditorTools/EditablePropertyInfo.h"
#include "EditorTools/EditablePropertyViewWidget.h"
#include "Components/ScrollBox.h"
#include "DesktopPlatformModule.h" 
#include "HAL/FileManager.h"
#include "Components/TextBlock.h"
#include "AssetToolsModule.h"
#include "IContentBrowserSingleton.h" // Content Browser 싱글톤 인터페이스
#include "ContentBrowserModule.h"     // FContentBrowserModule

bool UDataAssetCreator::Initialize()
{
	auto isSuccess = Super::Initialize();
	SetupDataAssetClasses();
	SetupDataAssetDropDown();
	return isSuccess;
}

void UDataAssetCreator::SetupDataAssetClasses()
{
	const FString ProjectModuleName = FApp::GetProjectName(); // 예: SkillTrial_Unreal
	const FString ScriptPathPrefix = FString::Printf(TEXT("/Script/%s"), *ProjectModuleName);

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	AssetRegistry.SearchAllAssets(true);
	TArray<FTopLevelAssetPath> baseClassPaths;
	baseClassPaths.Add(UDataAsset::StaticClass()->GetClassPathName());

	TSet<FTopLevelAssetPath> DerivedPaths;
	AssetRegistry.GetDerivedClassNames(baseClassPaths, TSet<FTopLevelAssetPath>(), DerivedPaths);
	for (const FTopLevelAssetPath& ClassPath : DerivedPaths)
	{
		if (!ClassPath.GetPackageName().ToString().StartsWith(ScriptPathPrefix))
			continue;

		FString ClassPathString = ClassPath.ToString();
		UClass* foundClass = StaticLoadClass(UObject::StaticClass(), nullptr, *ClassPathString, nullptr, LOAD_NoWarn, nullptr);
		if (foundClass && foundClass->IsChildOf(UDataAsset::StaticClass()) && !foundClass->HasAnyClassFlags(CLASS_Abstract))
			_dataAssetClasses.Add(foundClass->GetName(), foundClass);
	}

}
void UDataAssetCreator::SetupDataAssetDropDown()
{
	if (_dataAssetClasses.IsEmpty() || !_dataAssetClassesDropDown)
		return;

	for (auto dataAssetPair : _dataAssetClasses)
		_dataAssetClassesDropDown->AddOption(dataAssetPair.Key);
}

void UDataAssetCreator::OnDataAssetSelected(FString selectedClassName)
{
	SetSelectedDataAsset(selectedClassName);
	SetupPropertyViewsScroll();
}

void UDataAssetCreator::SetSelectedDataAsset(FString selectedClassName)
{
	if (!_dataAssetClasses.Contains(selectedClassName))
		return;

	_selectedDataAsset = _dataAssetClasses[selectedClassName];
}

TArray<struct FEditablePropertyInfo> UDataAssetCreator::GetEditableProperties()
{
	TArray<FEditablePropertyInfo> propertiesInfo;
	if (!_selectedDataAsset)
		return propertiesInfo;

	for (TFieldIterator<FProperty> propertyIt(_selectedDataAsset); propertyIt; ++propertyIt)
	{
		FProperty* property = *propertyIt;
		if (property->HasAllPropertyFlags(CPF_Edit) && property->HasAnyPropertyFlags(CPF_BlueprintVisible))
		{
			FEditablePropertyInfo info;
			info.PropertyName = property->GetFName();
			info.PropertyType = property->GetClass()->GetName();
			info.Tooltip = property->GetToolTipText().ToString();

			propertiesInfo.Add(info);
		}
	}
	return propertiesInfo;
}

void UDataAssetCreator::SetupPropertyViewsScroll()
{
	if (_selectedDataAsset == nullptr || !_selectedDataAsset.Get())
		return;

	ClearPropertyViewScroll();
	auto editableProperties = GetEditableProperties();
	if (editableProperties.IsEmpty())
		return;

	for (const FEditablePropertyInfo& propertyInfo : editableProperties)
	{
		auto newRowWidget = CreateWidget<UEditablePropertyViewWidget>(this, _propertyViewWidgetClass);
		if (!newRowWidget)
			continue;

		_propertyViews.Add(newRowWidget);
		newRowWidget->SetPropertyInfo(propertyInfo);
		_propertyViewsScroll->AddChild(newRowWidget);
	}
}

void UDataAssetCreator::ClearPropertyViewScroll()
{
	_propertyViews.Empty();
	_propertyViewsScroll->ClearChildren();
}

void UDataAssetCreator::OnBrowseFolderClicked()
{
	SetBrowseFolder();
}

void UDataAssetCreator::SetBrowseFolder()
{
	auto desktopPlatform = FDesktopPlatformModule::Get();
	if (!desktopPlatform)
		return;
	void* ParentWindowHandle = nullptr;
	FString DefaultPath = FPaths::ProjectContentDir();
	FString FolderPath;

	// 폴더 선택 대화 상자 열기
	bool bFolderSelected = desktopPlatform->OpenDirectoryDialog(
		ParentWindowHandle, // 부모 윈도우 핸들 (nullptr로 두어도 대부분 작동)
		TEXT("Select Data Asset Save Directory"), // 대화 상자 제목
		DefaultPath,  // 초기 경로
		FolderPath    // 선택된 경로가 저장될 변수
	);

	if (bFolderSelected && !FolderPath.IsEmpty())
	{
		// 선택된 경로 저장
		_targetFolderPath = FolderPath;

		// UMG Editable Text 위젯에 경로를 표시 (널 체크 필수)
		if (_targetBrowserPathText)
			_targetBrowserPathText->SetText(FText::FromString(_targetFolderPath));
	}
}

void UDataAssetCreator::OnCreateButtonClicked()
{
	CreateTargetDataAsset();
}

void UDataAssetCreator::CreateTargetDataAsset()
{
	if (_propertyViews.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("CreateTargetDataAsset failed: No properties loaded or property view is empty."));
		return;
	}

	if (_targetBrowserPathText->GetText().IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("CreateTargetDataAsset failed: Save path is empty or invalid."));
		return;
	}

	const FString localPath = _targetBrowserPathText->GetText().ToString().TrimStartAndEnd();

	FString savePackagePath;
	if (!FPackageName::TryConvertFilenameToLongPackageName(localPath, savePackagePath))
	{
		UE_LOG(LogTemp, Error, TEXT("CreateTargetDataAsset failed: Failed to convert local path '%s' to LongPackageName."), *localPath);
		return;
	}

	UClass* dataAssetClass = _selectedDataAsset.Get();
	FString assetName = dataAssetClass->GetName() + TEXT("Data");
	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

	UObject* newAsset = AssetToolsModule.Get().CreateAsset(
		assetName,
		savePackagePath,
		dataAssetClass,
		nullptr
	);

	if (!newAsset)
		return;

	UDataAsset* dataAssetInstance = Cast<UDataAsset>(newAsset);

	if (dataAssetInstance)
	{
		for (UEditablePropertyViewWidget* propertyViewWidget : _propertyViews)
		{
			auto propertyName = propertyViewWidget->GetPropertyName();
			FProperty* targetProperty = dataAssetInstance->GetClass()->FindPropertyByName(propertyName);
			if (targetProperty)
			{
				void* propertyValuePtr = targetProperty->ContainerPtrToValuePtr<void>(dataAssetInstance);
				propertyViewWidget->CopyInputValueToMemory(propertyValuePtr, targetProperty);
			}
		}
		dataAssetInstance->MarkPackageDirty();
	}

	TArray<UObject*> assetsToSelect;
	assetsToSelect.Add(newAsset);
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	ContentBrowserModule.Get().SyncBrowserToAssets(assetsToSelect);
}
