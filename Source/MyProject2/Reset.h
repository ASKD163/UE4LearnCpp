// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Reset.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT2_API UReset : public UBTTaskNode
{
	GENERATED_BODY()

public:
	// UPROPERTY(EditAnywhere)
	// ;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(EditAnywhere)
	AActor* Value;
};
