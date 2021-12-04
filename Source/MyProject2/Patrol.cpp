// Fill out your copyright notice in the Description page of Project Settings.


#include "Patrol.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FVector Origin = OwnerComp.GetAIOwner()->GetNavAgentLocation();
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation PatrolPoint;
	NavSystem->GetRandomPointInNavigableRadius(Origin, 10000.0f, PatrolPoint);
	OwnerComp.GetBlackboardComponent()->SetValueAsVector("PatrolPoint", PatrolPoint.Location);
	return EBTNodeResult::Succeeded;
}
