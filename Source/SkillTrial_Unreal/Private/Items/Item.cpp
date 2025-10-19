// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Characters/TrialCharacter.h"
#include "Enums/ItemState.h"
#include "Components/SphereComponent.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_itemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = _itemMesh;

	InitializeSphereComponent();
}

void AItem::InitializeSphereComponent()
{
	_sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	_sphere->SetupAttachment(GetRootComponent());
}


void AItem::ShowScreenDebugMessage()
{
	if (!GEngine)
		return;
	GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, FString("Item OnScreen Message"));
}

void AItem::ShowDebugSphere()
{
	auto world = GetWorld();
	if (!world)
		return;

	auto currentActorLocation = GetActorLocation();
	if (_actorLocation == currentActorLocation)
		return;

	_actorLocation = GetActorLocation();
	DrawDebugSphere(world, _actorLocation, 50.f, 24, FColor::Red, false, 60.f);
}

void AItem::ShowUELog()
{
	UE_LOG(LogTemp, Warning, TEXT("Begin Play Called!"));
}

void AItem::ShowDebugLine()
{
	auto world = GetWorld();
	if (!world)
		return;

	auto actorLocation = GetActorLocation();
	auto actorForward = GetActorForwardVector();
	DrawDebugLine(world, actorLocation, actorLocation + actorForward * 100.f, FColor::Blue, true, -1.f, 0, 1.f);
}

void AItem::SetItemLocation()
{
	SetActorLocation(FVector(0.f, 0.f, 150.f));
}
float AItem::TransformedSin()
{
	return _amplitude * FMath::Sin(_runningTime * _timeConstant);
}
float AItem::TransformedCos()
{
	return _amplitude * FMath::Cos(_runningTime * _timeConstant);
}

void AItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HandleSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
void AItem::HandleSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATrialCharacter* trialCharacter = Cast<ATrialCharacter>(OtherActor);
	if (trialCharacter)
		trialCharacter->SetOverlappingItem(this);
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	HandleSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}


void AItem::HandleSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATrialCharacter* trialCharacter = Cast<ATrialCharacter>(OtherActor);
	if (trialCharacter)
		trialCharacter->SetOverlappingItem(nullptr);
}



void AItem::UpdateItemState(EItemState targetState)
{
	_itemState = targetState;
}

void AItem::SetSphereCollisionEnabled(ECollisionEnabled::Type collisionType)
{
	_sphere->SetCollisionEnabled(collisionType);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	SubscribeToSphereBeginOverlap();
	SubscribeToSphereEndOverlap();
}
void AItem::SubscribeToSphereBeginOverlap()
{
	_sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
}
void AItem::SubscribeToSphereEndOverlap()
{
	_sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}
void AItem::ApplySineWaveFloat(float deltaTime)
{
	_runningTime += deltaTime;
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ApplySineWaveFloat(DeltaTime);
	Hovering();
}
void AItem::Hovering()
{
	if (_itemState != EItemState::EIS_Hovering)
		return;
	AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
}
