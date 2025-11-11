// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/TrialCharacter.h"
#include "Components/BoxComponent.h"
#include "Enums/ItemState.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Equipment/PickEquipmentComponent.h"

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

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HandleBoxOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::HandleBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetOwner())
		return;

	auto boxHit = TracingHitResult();
	if (!boxHit.GetActor())
		return;

	if (!CheckFriendly(OtherActor) && !CheckFriendly(boxHit.GetActor()))
		return;

	ApplyDamage(boxHit);
	HandleHitResult(boxHit);
	CreateForceFields(boxHit.ImpactPoint);
}

const FHitResult AWeapon::TracingHitResult()
{
	const FVector start = _boxTraceStart->GetComponentLocation();
	const FVector end = _boxTraceEnd->GetComponentLocation();

	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(this);

	for (AActor* actor : _ignoreActors)
		actorsToIgnore.AddUnique(actor);

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
		EDrawDebugTrace::None,
		boxHit,
		true);
	return boxHit;
}

void AWeapon::HandleHitResult(const FHitResult& hitResult)
{
	AActor* hitActor = hitResult.GetActor();
	if (!hitActor)
		return;

	if (_ignoreActors.Contains(hitActor))
		return;

	IHitInterface* hitInterface = Cast<IHitInterface>(hitActor);
	if (!hitInterface)
		return;

	hitInterface->Execute_OnHit(hitResult.GetActor(), hitResult.ImpactPoint, GetOwner());
	_ignoreActors.AddUnique(hitActor);
}

void AWeapon::ApplyDamage(const FHitResult& hitResult)
{
	UGameplayStatics::ApplyDamage(
		hitResult.GetActor(),
		_weaponDamage,
		GetInstigator()->GetController(),
		this,
		UDamageType::StaticClass());
}

void AWeapon::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	PerformAttackTrace();
}

void AWeapon::PerformAttackTrace()
{
	if (!_bIsTracingActive)
		return;

	ACharacter* ownerChar = Cast<ACharacter>(GetOwner());
	if (!ownerChar)
		return;

	const FVector start = _boxTraceStart->GetComponentLocation();
	const FVector end = _boxTraceEnd->GetComponentLocation();
	const FVector HalfExtent(15.f, 15.f, 15.f); // 박스 크기 (작을수록 정확하지만 성능 부담)
	const FRotator Orientation = ownerChar->GetActorRotation(); // 캐릭터의 회전을 따름 (필요에 따라 무기 회전 사용)
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(ownerChar); // 공격자(자기 자신)는 무시합니다.
	Params.bTraceComplex = false;
	TArray<FHitResult> HitResults;
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,             // 결과를 받을 배열
		start,          // 트레이스 시작 위치
		end,            // 트레이스 끝 위치
		FQuat(Orientation),     // 박스의 회전
		ECC_GameTraceChannel1,  // 트레이스할 채널 (예: Enemy Trace Channel)
		FCollisionShape::MakeBox(HalfExtent), // 박스 모양
		Params                  // 설정된 파라미터
	);

	if (bHit)
	{
		for (const FHitResult& hitResult : HitResults)
		{
			AActor* hitActor = hitResult.GetActor();
			if (!hitActor) continue;

			if (_ignoreActors.Contains(hitActor))
				continue;

			if (!CheckFriendly(hitActor))
				continue;

			ApplyDamage(hitResult);
			HandleHitResult(hitResult); // 이 함수 내에서 _ignoreActors에 액터를 추가해야 합니다!
			CreateForceFields(hitResult.ImpactPoint);
		}
	}
}

void AWeapon::Equip(USceneComponent* inParent, FName inSocketName, AActor* owner, APawn* instigator)
{
	SetOwner(owner);
	SetInstigator(instigator);
	AttachToSocket(inParent, inSocketName);
	UpdateItemState(EItemState::EIS_Equipped);
	PlaySFX(_equipSFX);
	SetSphereCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	DeactiveAmberEffect();
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

void AWeapon::ClearIgnoreActors()
{
	_ignoreActors.Empty();
}

void AWeapon::SetBoxCollisionEnable(ECollisionEnabled::Type collsionType)
{
	if (!_weaponBox)
		return;
	_bIsTracingActive = collsionType != ECollisionEnabled::NoCollision;
	_hitActors.Empty();
}

bool AWeapon::CheckFriendly(AActor* OtherActor)
{
	if(!GetOwner() || !OtherActor)
		return false;

	if (GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy")))
		return false;

	return true;
}

void AWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HandleOverlapPickEquipment(OtherActor);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	HandleEndlapPickEquipment(OtherActor);
}

void AWeapon::HandleOverlapPickEquipment(AActor* OtherActor)
{
	ATrialCharacter* trialCharacter = Cast<ATrialCharacter>(OtherActor);
	if (!trialCharacter)
		return;

	auto pickEquipment = trialCharacter->GetComponentByClass<UPickEquipmentComponent>();
	if (!pickEquipment)
		return;

	pickEquipment->SetGroundItem(this);
}

void AWeapon::HandleEndlapPickEquipment(AActor* OtherActor)
{
	ATrialCharacter* trialCharacter = Cast<ATrialCharacter>(OtherActor);
	if (!trialCharacter)
		return;

	auto pickEquipment = trialCharacter->GetComponentByClass<UPickEquipmentComponent>();
	if (!pickEquipment)
		return;

	pickEquipment->SetGroundItem(nullptr);
}