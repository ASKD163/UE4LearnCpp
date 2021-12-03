// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"


/**
 * 
 */
UCLASS()
class MYPROJECT2_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;

protected:
	virtual void BeginPlay() override;
	
};
