// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FORCEINLINE float GetHealthPercent() const;
	FORCEINLINE void UpdateHealth(const float value) { _health = FMath::Clamp(0, _health + value, _maxHealth); }
	FORCEINLINE bool IsAlive() { return _health > 0.f; }
	FORCEINLINE void SetDead() { _isDead = true; }
	FORCEINLINE bool IsDead() { return _isDead; }
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float _health;
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float _maxHealth;
	bool _isDead = false;;
};
