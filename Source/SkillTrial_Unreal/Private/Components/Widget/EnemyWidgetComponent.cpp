// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Widget/EnemyWidgetComponent.h"
#include "Components/Attribute/CharacterAttributeComponent.h"
#include "Components/Event/ActorEventPropagationComponent.h"
#include "GameFramework/Character.h"
#include "Hud/HealthBarComponent.h"


// Sets default values for this component's properties
UEnemyWidgetComponent::UEnemyWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UEnemyWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	SetOwner();
	SubscribeToAttribute();
	SubscribeToEventPropagation();
	SetVisibleHealthBarWidget(false);
}

void UEnemyWidgetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnsubscribeToEventPropagation();
	UnsubscribeToAttribute();
	Super::EndPlay(EndPlayReason);
}



void UEnemyWidgetComponent::Setup(AActor* owner, TArray<UActorComponent*>& registeredSubComponents)
{
	check(owner);
	CreateHealthBarWidght(owner, registeredSubComponents);
}

void UEnemyWidgetComponent::CreateHealthBarWidght(AActor* owner, TArray<UActorComponent*>& registeredSubComponents)
{
	_healthBarWidget = owner->CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	_healthBarWidget->SetupAttachment(owner->GetRootComponent());
	registeredSubComponents.Add(_healthBarWidget);
}

void UEnemyWidgetComponent::SetVisibleHealthBarWidget(const bool isVisible)
{
	if (!_healthBarWidget || _healthBarWidget->IsVisible() == isVisible)
		return;

	_healthBarWidget->SetVisibility(isVisible);
}

void UEnemyWidgetComponent::UpdateHealthBarWidget(const float healthPer)
{
	if (!_healthBarWidget)
		return;

	_healthBarWidget->UpdateHealthPer(healthPer);
}

void UEnemyWidgetComponent::SubscribeToAttribute()
{
	if (!_owner)
		return;

	auto attribute = _owner->GetComponentByClass<UCharacterAttributeComponent>();
	if (!attribute)
		return;

	attribute->GetOnHealthChanged().AddUObject(this, &UEnemyWidgetComponent::HandleHealthChanged);
}

void UEnemyWidgetComponent::HandleHealthChanged()
{
	if (!_owner)
		return;

	auto attribute = _owner->GetComponentByClass<UCharacterAttributeComponent>();
	if (!attribute)
		return;

	UpdateHealthBarWidget(attribute->GetHealthPercent());
}

void UEnemyWidgetComponent::UnsubscribeToAttribute()
{
	if (!_owner)
		return;

	auto attribute = _owner->GetComponentByClass<UCharacterAttributeComponent>();
	if (!attribute)
		return;

	attribute->GetOnHealthChanged().RemoveAll(this);
}

void UEnemyWidgetComponent::SubscribeToEventPropagation()
{
	if (!_owner)
		return;

	auto eventPropagation = _owner->FindComponentByClass<UActorEventPropagationComponent>();
	if (!eventPropagation)
		return;

	eventPropagation->GetOnHitEvent().AddUObject(this, &UEnemyWidgetComponent::HandleHitEvent);
}

void UEnemyWidgetComponent::HandleHitEvent(const FVector& hitPoint, AActor* hitter)
{
	SetVisibleHealthBarWidget(true);
}

void UEnemyWidgetComponent::UnsubscribeToEventPropagation()
{
	if (!_owner)
		return;

	auto eventPropagation = _owner->FindComponentByClass<UActorEventPropagationComponent>();
	if (!eventPropagation)
		return;

	eventPropagation->GetOnHitEvent().RemoveAll(this);
}

