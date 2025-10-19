// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/TrialCharacter.h"
#include "Enums/ItemState.h" 
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AWeapon::AWeapon() : AItem()
{
	InitializeBoxComponent();
	InitializeBoxTrace();
}

void AWeapon::InitializeBoxComponent()
{
	_weaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	_weaponBox->SetupAttachment(GetRootComponent());
	_weaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	_weaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	_weaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}
void AWeapon::InitializeBoxTrace()
{
	_boxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	_boxTraceStart->SetupAttachment(GetRootComponent());
	_boxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	_boxTraceEnd->SetupAttachment(GetRootComponent());
}
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	SubscribeToBoxOverlap();
}
void AWeapon::SubscribeToBoxOverlap()
{
	_weaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}
void AWeapon::HandleSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::HandleSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	//AttachToCharacterSocket(OtherActor);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HandleBoxOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
void AWeapon::HandleBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Tracing();

}
void AWeapon::Tracing()
{
	const FVector start = _boxTraceStart->GetComponentLocation();
	const FVector end = _boxTraceEnd->GetComponentLocation();

	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(this);
	FHitResult boxHit;
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		start,
		end,
		FVector(5.f, 5.f, 5.f),
		_boxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		actorsToIgnore,
		EDrawDebugTrace::ForDuration,
		boxHit,
		true);
}
void AWeapon::Equip(USceneComponent* inParent, FName inSocketName)
{
	AttachToSocket(inParent, inSocketName);
	UpdateItemState(EItemState::EIS_Equipped);
	PlaySFX(_equipSFX);
	SetSphereCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void AWeapon::PlaySFX(USoundBase* targetSFX)
{
	if (!targetSFX)
		return;

	UGameplayStatics::PlaySoundAtLocation(this, targetSFX, GetActorLocation());
}
void AWeapon::AttachToSocket(USceneComponent* inParent, const FName& inSocketName)
{
	FAttachmentTransformRules transformRules(EAttachmentRule::SnapToTarget, true);
	_itemMesh->AttachToComponent(inParent, transformRules, inSocketName);
}
void AWeapon::PlaySlashSFX()
{
	PlaySFX(_equipSFX);
}

void AWeapon::SetBoxCollisionEnable(ECollisionEnabled::Type collsionType)
{
	if (!_weaponBox)
		return;

	_weaponBox->SetCollisionEnabled(collsionType);
}

