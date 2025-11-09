// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Treasures/Treasure.h"
#include "Components/BoxComponent.h"

// Sets default values
ABreakableActor::ABreakableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	InitializeGeometry();
	InitializeBox();
}

// Called when the game starts or when spawned
void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	SubscribeToBreakEvent();
}
void ABreakableActor::SubscribeToBreakEvent()
{
	_geometryCollection->OnChaosBreakEvent.AddDynamic(this, &ABreakableActor::HandleChaosBreakEvent);
}

void ABreakableActor::HandleChaosBreakEvent(const FChaosBreakEvent& BreakEvent)
{
	if (_isBreaking)
		return;

	SetBreaking();
	SetBoxCollisionNoCollision();
	SetLifeSpan(3.f);
}
void ABreakableActor::SetBoxCollisionNoCollision()
{
	_box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}
void ABreakableActor::SetBreaking()
{
	_isBreaking = true;
}

void ABreakableActor::OnHit_Implementation(const FVector& hitPoint, AActor* hitter)
{
	if (_isBreaking)
		return;

	SpawnTreasure();
}
void ABreakableActor::SpawnTreasure()
{
	if (_treasureClases.IsEmpty())
		return;

	FVector location = GetActorLocation();
	location.Z += 75.f;

	int32 randomIndex = FMath::RandRange(0, _treasureClases.Num() - 1);
	TSubclassOf<ATreasure> targetTreasure = _treasureClases[randomIndex];
	GetWorld()->SpawnActor<ATreasure>(targetTreasure, location, GetActorRotation());
}
void ABreakableActor::InitializeGeometry()
{
	_geometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(_geometryCollection);

	_geometryCollection->SetGenerateOverlapEvents(true);
	_geometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void ABreakableActor::InitializeBox()
{
	_box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	_box->SetupAttachment(GetRootComponent());
	_box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	_box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

// Called every frame
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

