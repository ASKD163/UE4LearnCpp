// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinWidget.generated.h"

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class MYPROJECT2_API UWinWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	UTextBlock* RoundText;
	
	UPROPERTY()
	UButton* PlayButton;

	UPROPERTY()
	UButton* ExitButton;
};
