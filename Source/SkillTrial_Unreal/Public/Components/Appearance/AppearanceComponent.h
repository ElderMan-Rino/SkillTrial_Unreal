// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AppearanceComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLTRIAL_UNREAL_API UAppearanceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAppearanceComponent();

public:
	void Setup(AActor* owner, TArray<UActorComponent*>& registeredSubComponents);

private:
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "Appearance|Hair", meta = (AllowPrivateAccess = "true", ShowInnerProperties = "true"))
	TObjectPtr<class UGroomComponent> _hairGroom = nullptr;
	void CreateHair(AActor* owner, TArray<UActorComponent*>& registeredSubComponents);
	void SetupHair(ACharacter* character);

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "Appearance|Eyebrows", meta = (AllowPrivateAccess = "true", ShowInnerProperties = "true"))
	TObjectPtr<class UGroomComponent> _eyeBrowsGroom = nullptr;
	void CreateEyeBrows(AActor* owner, TArray<UActorComponent*>& registeredSubComponents);
	void SetupEyeBrows(ACharacter* character);
};
