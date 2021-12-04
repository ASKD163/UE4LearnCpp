// Fill out your copyright notice in the Description page of Project Settings.


#include "Reset.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UReset::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsObject("Player", nullptr);
	return EBTNodeResult::Succeeded;
}
