// Fill out your copyright notice in the Description page of Project Settings.


#include "Hear.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UHear::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsHear", false);
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UHear::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsHear", false);
	return EBTNodeResult::Aborted;
}
