// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyWidgetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UEnemyWidgetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyWidgetComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ACharacter> _owner;
	FORCEINLINE void SetOwner() { _owner = Cast<ACharacter>(GetOwner()); }

public:
	void Setup(AActor* owner, TArray<UActorComponent*>& registeredSubComponents);

private:	
	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* _healthBarWidget;
	void CreateHealthBarWidght(AActor* owner, TArray<UActorComponent*>& registeredSubComponents);
	void SetVisibleHealthBarWidget(bool isVisible);
	void UpdateHealthBarWidget(const float healthPer);
	
private:
	void SubscribeToAttribute();
	void HandleHealthChanged();
	void UnsubscribeToAttribute();

private:
	void SubscribeToEventPropagation();
	void HandleHitEvent(const FVector& hitPoint, AActor* hitter);
	void UnsubscribeToEventPropagation();
};
