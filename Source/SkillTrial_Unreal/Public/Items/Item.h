// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums/ItemState.h"
#include "Item.generated.h"

class UNiagaraComponent;

UCLASS()
class SKILLTRIAL_UNREAL_API AItem :  public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float _amplitude = 0.25f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float _timeConstant = 5.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* _itemMesh;
	UPROPERTY(EditAnywhere)
	EItemState _itemState = EItemState::EIS_Hovering;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* _embersEffect;

	UFUNCTION(BlueprintPure)
	float TransformedSin();
	UFUNCTION(BlueprintPure)
	float TransformedCos();

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void UpdateItemState(EItemState targetState);
	void SetSphereCollisionEnabled(ECollisionEnabled::Type collisionType);
	void DeactiveAmberEffect();

	template<typename T>
	T Avg(T first, T second);

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	float _runningTime = 0.f;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* _sphere;

	FVector _actorLocation = FVector::Zero();

	void InitializeSphereComponent();
	void InitializeEmbersEffect();
	void SubscribeToSphereBeginOverlap();
	void SubscribeToSphereEndOverlap();
	void ShowScreenDebugMessage();
	void ShowDebugSphere();
	void ShowUELog();
	void ShowDebugLine();
	void SetItemLocation();
	void ApplySineWaveFloat(float deltaTime);
	void Hovering();

private:
	void HandleOverlapPickItem(AActor* OtherActor);
	void HandleEndlapPickItem(AActor* OtherActor);
};

template<typename T>
inline T AItem::Avg(T first, T second)
{
	return (first + second) * 0.5f;
}
