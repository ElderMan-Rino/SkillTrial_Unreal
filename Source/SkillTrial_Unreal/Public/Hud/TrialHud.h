// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TrialHud.generated.h"

class UUIOverlay;
/**
 * 
 */
UCLASS()
class SKILLTRIAL_UNREAL_API ATrialHud : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE UUIOverlay* GetUIOverlay() { return _uiOverlayInstance; }
private:
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUIOverlay> _uiOverlay;
	void CreateUIOverlayInstance();
	UPROPERTY(VisibleAnywhere, Category = UI)
	UUIOverlay* _uiOverlayInstance;
	
};
