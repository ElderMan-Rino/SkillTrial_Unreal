#include "Characters/TrialCharacter.h"
#include "Components/Camera/CameraHandlerComponent.h"
#include "Components/Appearance/AppearanceComponent.h"
#include "Components/Equipment/PickEquipmentComponent.h"
#include "Components/Equipment/PlayerEquipmentComponent.h"
#include "Components/Movement/PlayerMovementController.h"
#include "Components/Inputs/AxisInputController.h"
#include "Components/Inputs/ActionInputController.h"
#include "Components/Animation/AnimPlayerComponent.h"
#include "Components/State/PlayerStateComponent.h"
#include "Components/Combat/PlayerCombatComponent.h"
#include "Components/Animation/AniActivityComponent.h"
#include "Components/Event/ActorEventPropagationComponent.h"
#include "Components/Dead/CharacterDeadComponent.h"
#include "Components/Collision/PlayerCollisionController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/VFX/CharacterVFXComponent.h"
#include "Components/SFX/CharacterSFXComponent.h"
#include "Components/Widget/PlayerWidgetComponent.h"
#include "Components/PickUpItem/PickUpItemComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Enums/CharacterEquipState.h"
#include "Enums/CharacterActionState.h"
#include "Enums/AniActivityState.h"
#include "Animation/AnimMontage.h"

// Sets default values
ATrialCharacter::ATrialCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetupAttribute();
	SetupCameraHandlerComponent();
	SetupCollisionController();
	SetupMovementController();
	SetupAppearenceComponent();
	SetupActionInputContoller();
	SetupAxisInputContoller();
	SetupPickEquipment();
	SetupEquipment();
	SetupAnimPlayer();
	SetupPlayerState();
	SetupAniActivity();
	SetupCombat();
	SetupEventPropagation();
	SetupDead();
	SetupSFX();
	SetupVFX();
	SetupWidget();
	SetupPickUpItem();
}

void ATrialCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetCharacterTag();
}

void ATrialCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	BindAxisInputs(PlayerInputComponent);
	BindActionInputs(PlayerInputComponent);
}

void ATrialCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	HandleMovementModeChanged(PrevMovementMode);
}

void ATrialCharacter::OnHit_Implementation(const FVector& hitPoint, AActor* hitter)
{
	HandleHitEvent(hitPoint, hitter);
}

float ATrialCharacter::TakeDamage(float damage, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
	HandleTakeDamage(damage, damageEvent, eventInstigator, damageCauser);
	return damage;
}

void ATrialCharacter::OnItemPickedUp(AItem* targetItem)
{
	HandlePickUpItem(targetItem);
}

void ATrialCharacter::SetupAttribute()
{
	_attribute = CreateDefaultSubobject<UCharacterAttributeComponent>("Attribute");
}

void ATrialCharacter::SetupCameraHandlerComponent()
{
	_cameraHandler = CreateDefaultSubobject<UCameraHandlerComponent>(TEXT("CameraHandler"));
	_cameraHandler->Setup(this, _registeredSubComponents);
}

void ATrialCharacter::SetupCollisionController()
{
	_collisionController = CreateDefaultSubobject<UPlayerCollisionController>(TEXT("CollisionController"));
	_collisionController->Setup(this);
}

void ATrialCharacter::SetupAppearenceComponent()
{
	_appearance = CreateDefaultSubobject<UAppearanceComponent>(TEXT("Appearence"));
	_appearance->Setup(this, _registeredSubComponents);
}

void ATrialCharacter::SetupMovementController()
{
	_movementController = CreateDefaultSubobject<UPlayerMovementController>(TEXT("MovementController"));
	_movementController->Setup(this);
}

void ATrialCharacter::SetupAxisInputContoller()
{
	_axisInputController = CreateDefaultSubobject<UAxisInputController>(TEXT("AxisInput"));
}

void ATrialCharacter::BindAxisInputs(UInputComponent* playerInputComponent)
{
	_axisInputController->BindAxisInputs(playerInputComponent);
}

void ATrialCharacter::SetupActionInputContoller()
{
	_actionInputController = CreateDefaultSubobject<UActionInputController>(TEXT("ActionInput"));
}

void ATrialCharacter::BindActionInputs(UInputComponent* playerInputComponent)
{
	_actionInputController->BindActionInputs(playerInputComponent);
}

void ATrialCharacter::SetupPickEquipment()
{
	_pickEquipmentComponent = CreateDefaultSubobject<UPickEquipmentComponent>(TEXT("PickEquipment"));
}

void ATrialCharacter::SetupEquipment()
{
	_equipmentComponentV2 = CreateDefaultSubobject<UPlayerEquipmentComponent>(TEXT("Equipment"));
}

void ATrialCharacter::SetupAnimPlayer()
{
	_animPlayer = CreateDefaultSubobject<UAnimPlayerComponent>(TEXT("AnimPlayer"));
}

void ATrialCharacter::SetupPlayerState()
{
	_playerState = CreateDefaultSubobject<UPlayerStateComponent>(TEXT("PlayerState"));
}

void ATrialCharacter::SetupAniActivity()
{
	_aniActivity = CreateDefaultSubobject<UAniActivityComponent>(TEXT("AniActivity"));
}

void ATrialCharacter::SetupCombat()
{
	_playerCombat = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("PlayerCombat"));
}

void ATrialCharacter::SetupEventPropagation()
{
	_eventPropagation = CreateDefaultSubobject<UActorEventPropagationComponent>(TEXT("EventPropagation"));
}

void ATrialCharacter::SetupDead()
{
	_dead = CreateDefaultSubobject<UCharacterDeadComponent>(TEXT("Dead"));
}

void ATrialCharacter::SetupSFX()
{
	_sfx = CreateDefaultSubobject<UCharacterSFXComponent>(TEXT("SFX"));
}

void ATrialCharacter::SetupVFX()
{
	_vfx = CreateDefaultSubobject<UCharacterVFXComponent>(TEXT("VFX"));
}

void ATrialCharacter::SetupWidget()
{
	_widget = CreateDefaultSubobject<UPlayerWidgetComponent>(TEXT("Widget"));
}

void ATrialCharacter::SetupPickUpItem()
{
	_pickUpItemV2 = CreateDefaultSubobject<UPickUpItemComponent>(TEXT("PickUpItem"));
}

void ATrialCharacter::SetCharacterTag()
{
	Tags.Add(FName("TrialCharacter"));
}

void ATrialCharacter::HandleMovementModeChanged(EMovementMode prevMovementMode)
{
	if (!_eventPropagation)
		return;
	
	auto movement = GetCharacterMovement();
	if (!movement)
		return;

	_eventPropagation->BroadcastMovementModeChanged(prevMovementMode, movement->MovementMode);
}

void ATrialCharacter::HandleHitEvent(const FVector& hitPoint, AActor* hitter)
{
	if (!_eventPropagation)
		return;

	_eventPropagation->BroadcastHitEvent(hitPoint, hitter);
}


void ATrialCharacter::HandleTakeDamage(float damage, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
	if (!_eventPropagation)
		return;

	_eventPropagation->BroadcastDamageApplied(damage, damageEvent, eventInstigator, damageCauser);
}

void ATrialCharacter::HandlePickUpItem(AItem* targetItem)
{
	if (!_eventPropagation)
		return;

	_eventPropagation->BroadcastPickedUpItem(targetItem);
}
