// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Widget/PlayerWidgetComponent.h"
#include "Components/Attribute/CharacterAttributeComponent.h"
#include "Components/PickUpItem/PickUpItemComponent.h"
#include "GameFramework/Character.h"
#include "Hud/TrialHud.h"
#include "Hud/UIOverlay.h"

// Sets default values for this component's properties
UPlayerWidgetComponent::UPlayerWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	SetOwner();
	BindUIOverlay();
	SubscribeToAttribute();
	SubscribeToPickedTreasure();
	InitializeHealthWidget();
	InitializeStaminaWidget();
	InitializeTreasureWidget();

}

void UPlayerWidgetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnsubscribeToPickedTreasure();
	UnsubscribeToAttribute();
	Super::EndPlay(EndPlayReason);
}

void UPlayerWidgetComponent::BindUIOverlay()
{
	auto playerController = Cast<APlayerController>(_owner->GetController());
	if (!playerController)
		return;

	auto hud = Cast<ATrialHud>(playerController->GetHUD());
	if (!hud)
		return;
	
	_uiOverlay = hud->GetUIOverlay();
}


void UPlayerWidgetComponent::SubscribeToAttribute()
{
	if (!_owner)
		return;

	auto attribute = _owner->GetComponentByClass<UCharacterAttributeComponent>();
	if (!attribute)
		return;

	attribute->GetOnHealthChanged().AddUObject(this, &UPlayerWidgetComponent::HandleHealthChanged);
	attribute->GetOnStaminaChanged().AddUObject(this, &UPlayerWidgetComponent::HandleStaminaChanged);
}

void UPlayerWidgetComponent::HandleHealthChanged()
{
	UpdateHealthWidgets();
}

void UPlayerWidgetComponent::HandleStaminaChanged(float cur, float max, float per)
{
	UpdateStaminaWidgets(cur, max, per);
}

void UPlayerWidgetComponent::UpdateHealthWidgets()
{
	if (!_owner || !_uiOverlay)
		return;

	auto attribute = _owner->GetComponentByClass<UCharacterAttributeComponent>();
	if (!attribute)
		return;

	_uiOverlay->SetHealthBarPercent(attribute->GetHealthPercent());
	_uiOverlay->SetHealthValue(attribute->GetHealth(), attribute->GetMaxHealth());
}

void UPlayerWidgetComponent::UpdateStaminaWidgets(float cur, float max, float per)
{
	if (!_owner || !_uiOverlay)
		return;

	_uiOverlay->SetStaminaBarPercent(per);
	_uiOverlay->SetStaminaValue(cur, max);
}

void UPlayerWidgetComponent::UnsubscribeToAttribute()
{
	if (!_owner)
		return;

	auto attribute = _owner->GetComponentByClass<UCharacterAttributeComponent>();
	if (!attribute)
		return;

	attribute->GetOnHealthChanged().RemoveAll(this);
	attribute->GetOnStaminaChanged().RemoveAll(this);
}

void UPlayerWidgetComponent::InitializeHealthWidget()
{
	UpdateHealthWidgets();
}

void UPlayerWidgetComponent::InitializeStaminaWidget()
{
	if (!_owner)
		return;

	auto attribute = _owner->GetComponentByClass<UCharacterAttributeComponent>();
	if (!attribute)
		return;

	UpdateStaminaWidgets(attribute->GetStamina(), attribute->GetMaxStamina(), attribute->GetStaminaPercent());
}

void UPlayerWidgetComponent::SubscribeToPickedTreasure()
{
	if (!_owner || !_uiOverlay)
		return;

	auto pickUp = _owner->GetComponentByClass<UPickUpItemComponent>();
	if (!pickUp)
		return;

	pickUp->GetOnPickedTreasureAdded().AddUObject(this, &UPlayerWidgetComponent::HandleTreasureAdded);
}

void UPlayerWidgetComponent::HandleTreasureAdded(int32 addedTresureValue)
{
	UpdateTreasureWidget(addedTresureValue);
}

void UPlayerWidgetComponent::UpdateTreasureWidget(int32 addedTresureValue)
{
	if (!_uiOverlay)
		return;

	_uiOverlay->SetCoinValue(addedTresureValue);
}

void UPlayerWidgetComponent::UnsubscribeToPickedTreasure()
{
	if (!_owner || !_uiOverlay)
		return;

	auto pickUp = _owner->GetComponentByClass<UPickUpItemComponent>();
	if (!pickUp)
		return;

	pickUp->GetOnPickedTreasureAdded().RemoveAll(this);
}

void UPlayerWidgetComponent::InitializeTreasureWidget()
{
	if (!_owner || !_uiOverlay)
		return;

	auto pickUp = _owner->GetComponentByClass<UPickUpItemComponent>();
	if (!pickUp)
		return;

	UpdateTreasureWidget(pickUp->GetPickedTreasureValue());
}
