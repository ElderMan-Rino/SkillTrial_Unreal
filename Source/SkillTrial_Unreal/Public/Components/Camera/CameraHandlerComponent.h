// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraHandlerComponent.generated.h"

// UCameraHandlerComponent.h
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SKILLTRIAL_UNREAL_API UCameraHandlerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCameraHandlerComponent();

public:
    void Setup(AActor* owner, TArray<UActorComponent*>& registeredSubComponents);

private:
    UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "SpringArm", meta = (AllowPrivateAccess = "true", ShowInnerProperties = "true"))
    TObjectPtr<class USpringArmComponent> _springArm = nullptr;
    void CreateSpringArm(AActor* owner, TArray<UActorComponent*>& registeredSubComponents);
    void SetupSpringArm(AActor* owner);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewCamera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UCameraComponent> _viewCamera = nullptr;
    void CreateViewCamera(AActor* owner, TArray<UActorComponent*>& registeredSubComponents);
    void SetupViewCamera();
};