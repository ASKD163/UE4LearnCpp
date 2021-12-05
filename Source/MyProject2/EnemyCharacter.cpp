// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "MyProject2Projectile.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SetPeripheralVisionAngle(60.0f);
	PawnSensing->HearingThreshold = 1400;
	PawnSensing->LOSHearingThreshold = 1600;

	HP = 3;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyCharacter::SeePawn);
	PawnSensing->OnHearNoise.AddDynamic(this, &AEnemyCharacter::OnHearPawn);
	BlackBoard = UAIBlueprintHelperLibrary::GetBlackboard(this);
}

void AEnemyCharacter::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	
	AMyProject2Character* Player = Cast<AMyProject2Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	AMyProject2Projectile* Projectile = Cast<AMyProject2Projectile>(Other);\
	
	if (Projectile)
	{
		if (HP < 0)
		{
			UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation());
			UGameplayStatics::SpawnEmitterAtLocation(this, Emitter, GetActorLocation());
			Destroy();
			if (Player)
			{
				Player->SetKilledText(Player->GetKilled() + 1);
				Player->EnemyInGame--;
			}
		}
		else HP--;
	}

	if (Player && Player->GetKilled() >= Player->GetTarget()) Player->EndGame();
}

void AEnemyCharacter::SeePawn(APawn* Player)
{
	if (!BlackBoard) BlackBoard = UAIBlueprintHelperLibrary::GetBlackboard(this);
	if (Cast<AMyProject2Character>(Player))
	{
		if (BlackBoard) BlackBoard->SetValueAsObject("Player", Player);
	}
}

void AEnemyCharacter::OnHearPawn(APawn* Player, const FVector& Location, float Volume)
{
	if (!BlackBoard) BlackBoard = UAIBlueprintHelperLibrary::GetBlackboard(this);
	if (Cast<AMyProject2Character>(Player))
	{
		if (BlackBoard) BlackBoard->SetValueAsVector("HearLocation", Location);
		if (BlackBoard) BlackBoard->SetValueAsBool("IsHear", true);
	}
}





