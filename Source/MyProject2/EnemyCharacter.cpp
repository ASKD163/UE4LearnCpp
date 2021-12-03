// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TargetPoint.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SetPeripheralVisionAngle(60.0f);
	

}


// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyCharacter::SeePawn);
	CurrentPoin = PatrolPoint1;
	BlackBoard = UAIBlueprintHelperLibrary::GetBlackboard(this);
	if (BlackBoard) BlackBoard->SetValueAsObject("PatrolPoint", CurrentPoin);
}

void AEnemyCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	ATargetPoint* tmp = Cast<ATargetPoint>(OtherActor);
	if (tmp)
	{
		if (tmp == PatrolPoint1) BlackBoard->SetValueAsObject("PatrolPoint", PatrolPoint2);
		else if (tmp == PatrolPoint2) BlackBoard->SetValueAsObject("PatrolPoint", PatrolPoint1);
	} 
}

void AEnemyCharacter::SeePawn(APawn* Player)
{
	if (Cast<AMyProject2Character>(Player))
	{
		CurrentPoin = Player;
		if (BlackBoard) BlackBoard->SetValueAsObject("Player", CurrentPoin);
	}
}






