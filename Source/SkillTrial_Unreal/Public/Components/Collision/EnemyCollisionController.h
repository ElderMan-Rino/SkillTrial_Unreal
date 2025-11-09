// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyCollisionController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UEnemyCollisionController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyCollisionController();

protected:
	virtual void BeginPlay() override;

public:
	void Setup(ACharacter* owner);
private:
	void SetupCollisionOptions(ACharacter* owner);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AEnemy> _owner;
	FORCEINLINE void SetOwner() { _owner = Cast<AEnemy>(GetOwner()); }

public:
	void SetCollisionEnabled(ECollisionEnabled::Type targetEnableType);
};
