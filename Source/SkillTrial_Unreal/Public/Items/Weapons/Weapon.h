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
	void Equip(USceneComponent* inParent, FName inSocketName, AActor* owner, APawn* instigator);
	void AttachToSocket(USceneComponent* inParent, const FName& inSocketName);
	void PlaySlashSFX();
	void ClearIgnoreActors();
	
	void SetBoxCollisionEnable(ECollisionEnabled::Type collsionType);
protected:
	virtual void BeginPlay() override;


	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintImplementableEvent)
	void CreateForceFields(const FVector& targetLocation);
private:
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* _equipSFX;
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UBoxComponent* _weaponBox;
	UPROPERTY(EditAnywhere)
	USceneComponent* _boxTraceStart;
	UPROPERTY(EditAnywhere)
	USceneComponent* _boxTraceEnd;
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float _weaponDamage = 20.f;

	TArray<AActor*> _ignoreActors;

	void InitializeBoxComponent();
	void InitializeBoxTrace();
	void SubscribeToBoxOverlap();
	void PlaySFX(USoundBase* targetSFX);
	void HandleBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	const FHitResult TracingHitResult();
	void HandleHitResult(const FHitResult& hitResult);
	void ApplyDamage(const FHitResult& hitResult);

private:
	bool _bIsTracingActive = false;
	TArray<AActor*> _hitActors;

protected:
	virtual void Tick(float deltaTime) override;
	void PerformAttackTrace();

private:
	bool CheckFriendly(AActor* OtherActor);

protected:
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
private:
	void HandleOverlapPickEquipment(AActor* OtherActor);
	void HandleEndlapPickEquipment(AActor* OtherActor);
};
