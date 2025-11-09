// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/CharacterCondition.h"
#include "CharacterAttributeComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnHealthChanged);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnConditionChanged, ECharacterCondition);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnStaminaChanged, float, float, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UCharacterAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterAttributeComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ACharacter> _owner;
	FORCEINLINE void SetOwner() { _owner = Cast<ACharacter>(GetOwner()); }

public:
	FORCEINLINE bool IsTargetCondition(ECharacterCondition targetCondition) { return _condition == targetCondition; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ECharacterCondition GetCharacterCondition() { return _condition; }
	void UpdateCondition(ECharacterCondition targetCondition);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	ECharacterCondition _condition = ECharacterCondition::Alive;

public:
	FORCEINLINE FOnConditionChanged& GetOnConditionChanged() { return _onConditionChanged; }
	FORCEINLINE const FOnConditionChanged& GetOnConditionChanged() const { return _onConditionChanged; }
private:
	FOnConditionChanged _onConditionChanged;
	FORCEINLINE void BroadcastConditionChanged() { _onConditionChanged.Broadcast(_condition); }

public:
	FORCEINLINE float GetHealth() const { return _health; }
	FORCEINLINE float GetMaxHealth() const { return _maxHealth; }
	float GetHealthPercent() const;
	void UpdateHealth(const float value);
private:
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float _health;
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float _maxHealth;
	
public:
	FORCEINLINE FOnHealthChanged& GetOnHealthChanged() { return _onHealthChanged; }
	FORCEINLINE const FOnHealthChanged& GetOnHealthChanged() const { return _onHealthChanged; }
private:
	FOnHealthChanged _onHealthChanged;
	FORCEINLINE void BroadcastHealthChanged() { _onHealthChanged.Broadcast(); }

public:
	FORCEINLINE float GetStamina() const { return _stamina; }
	FORCEINLINE float GetMaxStamina() const { return _maxStamina; }
	float GetStaminaPercent() const;
	void UpdateStamina(const float value);
private:
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float _stamina = 0.f;
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float _maxStamina = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float _staminaRecoveryPerSecond = 10.f;

public:
	FORCEINLINE FOnStaminaChanged& GetOnStaminaChanged() { return _onStaminaChanged; }
	FORCEINLINE const FOnStaminaChanged& GetOnStaminaChanged() const { return _onStaminaChanged; }
private:
	FOnStaminaChanged _onStaminaChanged;
	FORCEINLINE void BroadcastStaminaChanged() { _onStaminaChanged.Broadcast(_stamina, _maxStamina, GetStaminaPercent()); }
	void HandleStaminaRecovery(float DeltaTime); 
};
