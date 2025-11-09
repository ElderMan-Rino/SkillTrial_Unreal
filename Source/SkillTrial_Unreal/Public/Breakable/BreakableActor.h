// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;
class ATreasure;
class UBoxComponent;
struct FChaosBreakEvent;

UCLASS()
class SKILLTRIAL_UNREAL_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakableActor();

	virtual void OnHit_Implementation(const FVector& hitPoint, AActor* hitter) override;
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	UFUNCTION(BlueprintCallable)
	void SetBreaking();
	UFUNCTION()
	void HandleChaosBreakEvent(const FChaosBreakEvent& BreakEvent);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent* _geometryCollection = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<ATreasure>> _treasureClases;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* _box;
	UPROPERTY(BlueprintReadOnly)
	bool _isBreaking = false;

private:
	void InitializeGeometry();
	void InitializeBox();
	void SubscribeToBreakEvent();
	void SpawnTreasure();
	void SetBoxCollisionNoCollision();
};
