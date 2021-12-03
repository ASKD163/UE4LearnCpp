// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
class UTextBlock;
class UProgressBar;

UCLASS()
class MYPROJECT2_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UTextBlock* KilledText;
	
	UPROPERTY()
	UTextBlock* AmmoText;

	UPROPERTY()
	UProgressBar* HealthBar;

	UPROPERTY()
	UProgressBar* StaminaBar;


protected:

};
