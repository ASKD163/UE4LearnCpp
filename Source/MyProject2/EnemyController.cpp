// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"

void AEnemyController()
{

}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
}
