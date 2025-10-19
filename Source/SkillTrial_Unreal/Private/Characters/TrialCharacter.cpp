#include "Characters/TrialCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Enums/CharacterEquipState.h"
#include "Enums/CharacterActionState.h"
#include "Animation/AnimMontage.h"

// Sets default values
ATrialCharacter::ATrialCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InitializeSpringArmComponent();
	InitializeViewCamComponent();
	SetUseControllerValues();
	SetCharacterMovementValues();
	InitializeHair();
	InitializeEyeBrows();
}

void ATrialCharacter::SetUseControllerValues()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}
void ATrialCharacter::SetCharacterMovementValues()
{
	auto movement = GetCharacterMovement();
	movement->bOrientRotationToMovement = true;
	movement->RotationRate = FRotator(0.f, 400.f, 0.f);
}
void ATrialCharacter::InitializeSpringArmComponent()
{
	_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	_springArm->SetupAttachment(GetRootComponent());
	_springArm->TargetArmLength = 150.f;
}

void ATrialCharacter::InitializeViewCamComponent()
{
	_viewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	_viewCamera->SetupAttachment(_springArm);
}

void ATrialCharacter::InitializeHair()
{
	_hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	_hair->SetupAttachment(GetMesh());
	_hair->AttachmentName = FString("head");
}
void ATrialCharacter::InitializeEyeBrows()
{
	_eyeBrows = CreateDefaultSubobject<UGroomComponent>(TEXT("EyeBrows"));
	_eyeBrows->SetupAttachment(GetMesh());
	_eyeBrows->AttachmentName = FString("head");
}
void ATrialCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetSpringArmLength();
}
void ATrialCharacter::SetSpringArmLength()
{
	_springArm->TargetArmLength = 300.f;
}
// Called every frame
void ATrialCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATrialCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	BindAxes(PlayerInputComponent);
	BindActions(PlayerInputComponent);
}

void ATrialCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type collsionType)
{
	if (!_equippedWeapon)
		return;

	_equippedWeapon->SetBoxCollisionEnable(collsionType);
}

void ATrialCharacter::BindAxes(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ATrialCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ATrialCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ATrialCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ATrialCharacter::LookUp);
}

const FVector ATrialCharacter::GetControlDirection(EAxis::Type targetAxis)
{
	const FRotator controlRotation = GetControlRotation();
	const FRotator yawRoation(0.f, controlRotation.Yaw, 0.f);
	return FRotationMatrix(yawRoation).GetUnitAxis(targetAxis);
}



void ATrialCharacter::MoveForward(float value)
{
	if (!Controller || value == 0.f || !CanMoving())
		return;

	AddMovementInput(GetControlDirection(EAxis::X), value);
}
void ATrialCharacter::MoveRight(float value)
{
	if (!Controller || value == 0.f || !CanMoving())
		return;

	AddMovementInput(GetControlDirection(EAxis::Y), value);
}
void ATrialCharacter::Turn(float value)
{
	UE_LOG(LogTemp, Warning, TEXT("Turn Value: %.3f"), value);
	AddControllerYawInput(value);
}
void ATrialCharacter::LookUp(float value)
{
	UE_LOG(LogTemp, Warning, TEXT("LookUp Value: %.3f"), value);
	AddControllerPitchInput(value);
}

void ATrialCharacter::BindActions(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ATrialCharacter::JumpAction);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &ATrialCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ATrialCharacter::Attack);
}
void ATrialCharacter::JumpAction()
{
	if (!CanMoving())
		return;

	ACharacter::Jump();
}
void ATrialCharacter::EKeyPressed()
{
	HandleEKeyPressed();
}
void ATrialCharacter::HandleEKeyPressed()
{
	if (_overlappingItem)
	{
		PickWeaponItem();
		UpdateEquipState(ECharacterEquipState::ECS_EquippedOneHanded);
	}
	else
	{
		if (!_equipMontage || !_equippedWeapon)
			return;

		if (CanDisarm())
		{
			PlayEquipMontage(FName("Unequip"));
			UpdateEquipState(ECharacterEquipState::ECS_Unequipped);
		}
		else if (CanArm())
		{
			PlayEquipMontage(FName("Equip"));
			UpdateEquipState(ECharacterEquipState::ECS_EquippedOneHanded);
		}
		UpdateActionState(ECharacterActionState::Equipping);
	}
}

void ATrialCharacter::PlayEquipMontage(FName sectionName)
{
	PlayTargetMontage(_equipMontage, sectionName);
}
void ATrialCharacter::PlayTargetMontage(UAnimMontage* targetMontage, FName sectionName)
{
	if (!targetMontage)
		return;

	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (!animInstance)
		return;

	animInstance->Montage_Play(targetMontage);
	animInstance->Montage_JumpToSection(sectionName, targetMontage);
}

bool ATrialCharacter::CanDisarm()
{
	return _actionState == ECharacterActionState::EAS_Unoccpled
		&& _equipState != ECharacterEquipState::ECS_Unequipped;
}
bool ATrialCharacter::CanArm()
{
	return _actionState == ECharacterActionState::EAS_Unoccpled
		&& _equipState == ECharacterEquipState::ECS_Unequipped
		&& _equippedWeapon;
}
void ATrialCharacter::PickWeaponItem()
{
	AWeapon* pickedWeapon = Cast<AWeapon>(_overlappingItem);
	if (!pickedWeapon)
		return;

	_overlappingItem = nullptr;
	_equippedWeapon = pickedWeapon;
	pickedWeapon->Equip(GetMesh(), FName("hand_rSocket"));
}

void ATrialCharacter::UpdateEquipState(ECharacterEquipState targetState)
{
	_equipState = targetState;
}

void ATrialCharacter::Attack()
{
	if (CanAttack())
	{
		PlayAttackMontage();
		UpdateActionState(ECharacterActionState::EAS_Attacking);
	}
}
void ATrialCharacter::Disarm()
{
	if (!_equippedWeapon)
		return;

	_equippedWeapon->AttachToSocket(GetMesh(), FName("spine_Socket"));
}
void ATrialCharacter::Arm()
{
	if (!_equippedWeapon)
		return;
	
	_equippedWeapon->AttachToSocket(GetMesh(), FName("hand_rSocket"));
	_equippedWeapon->PlaySlashSFX();
}
bool ATrialCharacter::CanAttack()
{
	return _actionState == ECharacterActionState::EAS_Unoccpled
		&& _equipState != ECharacterEquipState::ECS_Unequipped;
}

bool ATrialCharacter::CanMoving()
{
	return _actionState == ECharacterActionState::EAS_Unoccpled;
}

void ATrialCharacter::PlayAttackMontage()
{
	int32 selection = FMath::RandRange(0, 1);
	FName sectionName = FName();
	switch (selection)
	{
	case 0:
		sectionName = FName("AttackOne");
		break;
	case 1:
		sectionName = FName("AttackTwo");
		break;
	default:
		sectionName = FName("AttackOne");
		break;
	}
	PlayTargetMontage(_attackMontage, sectionName);
}

void ATrialCharacter::UpdateActionState(ECharacterActionState targetState)
{
	_actionState = targetState;
}
