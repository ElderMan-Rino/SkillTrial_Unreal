// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;
class USceneComponent;
/**
 * 
 */
UCLASS()
class SKILLTRIAL_UNREAL_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();
	void Equip(USceneComponent* inParent, FName inSocketName);
	void AttachToSocket(USceneComponent* inParent, const FName& inSocketName);
	void PlaySlashSFX();
	
	void SetBoxCollisionEnable(ECollisionEnabled::Type collsionType);
protected:
	virtual void BeginPlay() override;

	virtual void HandleSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* _equipSFX;
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UBoxComponent* _weaponBox;
	UPROPERTY(EditAnywhere)
	USceneComponent* _boxTraceStart;
	UPROPERTY(EditAnywhere)
	USceneComponent* _boxTraceEnd;

	void InitializeBoxComponent();
	void InitializeBoxTrace();
	void SubscribeToBoxOverlap();
	void PlaySFX(USoundBase* targetSFX);
	void HandleBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void Tracing();
};
