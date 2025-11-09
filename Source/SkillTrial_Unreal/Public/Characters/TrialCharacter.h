// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "Interfaces/HitInterface.h"
#include "Interfaces/PickUpInterface.h"
#include "TrialCharacter.generated.h"


UCLASS()
class SKILLTRIAL_UNREAL_API ATrialCharacter : public ACharacter, public IHitInterface, public IPickUpInterface
{
	GENERATED_BODY()

public:
	ATrialCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;
	virtual void OnHit_Implementation(const FVector& hitPoint, AActor* hitter) override;
	virtual float TakeDamage(float damage, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser) override;
	virtual void OnItemPickedUp(class AItem* targetItem) override;

private:
	UPROPERTY(VisibleAnywhere)
	TArray<UActorComponent*> _registeredSubComponents;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCharacterAttributeComponent> _attribute = nullptr;
private:
	void SetupAttribute();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCameraHandlerComponent> _cameraHandler = nullptr;
	void SetupCameraHandlerComponent();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPlayerCollisionController> _collisionController = nullptr;
	void SetupCollisionController();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAppearanceComponent> _appearance = nullptr;
	void SetupAppearenceComponent();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPlayerMovementController> _movementController = nullptr;
	void SetupMovementController();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAxisInputController> _axisInputController = nullptr;
	void SetupAxisInputContoller();
	void BindAxisInputs(UInputComponent* playerInputComponent);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UActionInputController> _actionInputController = nullptr;
	void SetupActionInputContoller();
	void BindActionInputs(UInputComponent* playerInputComponent);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPickEquipmentComponent> _pickEquipmentComponent = nullptr;
	void SetupPickEquipment();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPlayerEquipmentComponent> _equipmentComponent = nullptr;
	void SetupEquipment();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAnimPlayerComponent> _animPlayer = nullptr;
	void SetupAnimPlayer();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPlayerStateComponent> _playerState = nullptr;
private:
	void SetupPlayerState();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAniActivityComponent> _aniActivity = nullptr;
private:
	void SetupAniActivity();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPlayerCombatComponent> _playerCombat = nullptr;
private:
	void SetupCombat();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UActorEventPropagationComponent> _eventPropagation = nullptr;
	void SetupEventPropagation();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCharacterDeadComponent> _dead = nullptr;
private:
	void SetupDead();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCharacterSFXComponent> _sfx = nullptr;
	void SetupSFX();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCharacterVFXComponent> _vfx = nullptr;
	void SetupVFX();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPlayerWidgetComponent> _widget = nullptr;
	void SetupWidget();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPickUpItemComponent> _pickUpItem = nullptr;
	void SetupPickUpItem();

private:
	void SetCharacterTag();

private:
	void HandleMovementModeChanged(EMovementMode prevMovementMode);

private:
	void HandleHitEvent(const FVector& hitPoint, AActor* hitter);

private:
	void HandleTakeDamage(float damage, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser);

private:
	void HandlePickUpItem(class AItem* targetItem);
};
