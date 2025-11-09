// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h"

/**
 * 
 */
UCLASS()
class SKILLTRIAL_UNREAL_API ATreasure : public AItem
{
	GENERATED_BODY()
public:
	ATreasure();

protected:
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	void InitializeItemState();
	void PlaySFX(USoundBase* targetSFX, const FVector& targetLocation);
		
	UPROPERTY(EditAnywhere, Category = SFX)
	USoundBase* _coinSFX;

public:
	FORCEINLINE int32 GetTreasureValue() { return _value; }
private:
	UPROPERTY(EditAnywhere, Category = "Treasure Value")
	int32 _value = 0;

private:
	UPROPERTY(EditAnywhere, Category = SFX)
	class UNiagaraSystem* _pickUpEffect;
	void CreatePickUpEffect();
};
