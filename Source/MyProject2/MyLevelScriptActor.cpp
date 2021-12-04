// Fill out your copyright notice in the Description page of Project Settings.


#include "MyLevelScriptActor.h"
#include "MyProject2Character.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"



void AMyLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(InOutHandle, this, &AMyLevelScriptActor::SpawnTimer, UKismetMathLibrary::RandomFloatInRange(1.0f, 2.0f), true);
}

void AMyLevelScriptActor::SpawnTimer()
{
	AMyProject2Character* Player = Cast<AMyProject2Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if (Player->EnemyInGame <= 5)
	{
		UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), PawnClass, BehaviorTree, SpawnPoint->GetActorLocation(),
			FRotator (0, UKismetMathLibrary::RandomFloatInRange(0.f,360.f), 0));
		++Player->EnemyInGame;
	}
	
}