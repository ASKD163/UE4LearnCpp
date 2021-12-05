// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoosWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class MYPROJECT2_API ULoosWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	UButton* RePlayButton;

	UPROPERTY()
	UButton* ExitButton;
};