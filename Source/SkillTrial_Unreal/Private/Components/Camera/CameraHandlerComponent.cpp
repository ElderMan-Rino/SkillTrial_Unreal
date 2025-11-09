#include "Camera/CameraComponent.h"
#include "Components/Camera/CameraHandlerComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"

UCameraHandlerComponent::UCameraHandlerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCameraHandlerComponent::Setup(AActor* owner, TArray<UActorComponent*>& registeredSubComponents)
{
    check(owner); 

    CreateSpringArm(owner, registeredSubComponents);
    SetupSpringArm(owner);

    CreateViewCamera(owner, registeredSubComponents);
    SetupViewCamera();
}

void UCameraHandlerComponent::CreateSpringArm(AActor* owner, TArray<UActorComponent*>& registeredSubComponents)
{
    _springArm = owner->CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    registeredSubComponents.Add(_springArm);
}

void UCameraHandlerComponent::SetupSpringArm(AActor* owner)
{
    _springArm->SetupAttachment(owner->GetRootComponent());
    _springArm->TargetArmLength = 200.f;
}

void UCameraHandlerComponent::CreateViewCamera(AActor* owner, TArray<UActorComponent*>& registeredSubComponents)
{
    _viewCamera = owner->CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
    registeredSubComponents.Add(_viewCamera);
}

void UCameraHandlerComponent::SetupViewCamera()
{
    _viewCamera->SetupAttachment(_springArm);
}
