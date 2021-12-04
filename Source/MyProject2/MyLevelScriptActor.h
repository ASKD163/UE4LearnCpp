// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Engine/TargetPoint.h"
#include "MyLevelScriptActor.generated.h"

class UBehaviorTree;
/**
 * 
 */
UCLASS()
class MYPROJECT2_API AMyLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	ATargetPoint* SpawnPoint;

	FTimerHandle InOutHandle;

	UFUNCTION()
	void SpawnTimer();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> PawnClass;

	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;
protected:
	virtual void BeginPlay() override;
	
};
