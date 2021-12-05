// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class MYPROJECT2_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	UButton* PlayButton;

	UPROPERTY()
	UButton* RePlayButton;
	
	UPROPERTY()
	UButton* ExitButton;
};
