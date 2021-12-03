// Fill out your copyright notice in the Description page of Project Settings.


#include "Move.h"
#include "MyProject2Character.h"


// Sets default values
AMove::AMove()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Direction = FVector(0,1,0);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	isDestroyed = false;
}

// Called when the game starts or when spawned
void AMove::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMove::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Mesh->AddRelativeLocation(UKismetMathLibrary::Normal(Direction) * 200 * DeltaTime);
	FLatentActionInfo LatentInfo;
	LatentInfo.Linkage = 1;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = "ChangeDirection";
	LatentInfo.UUID = __LINE__;
	UKismetSystemLibrary::Delay(this, 2.0f, LatentInfo);
}

void AMove::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
	FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	AMyProject2Character* Player = Cast<AMyProject2Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));


	AMyProject2Projectile* Projectile = Cast<AMyProject2Projectile>(Other);
	if (Projectile)
	{
		if (isDestroyed)
		{
			UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation());
			UGameplayStatics::SpawnEmitterAtLocation(this, Emitter, GetActorLocation());
			Destroy();
			if (Player) Player->SetKilledText(Player->GetKilled() + 1);
		}
		else
		{
			isDestroyed = true;
		}
		Mesh->SetMaterial(0, Material);
		
		FLatentActionInfo LatentInfo;
		LatentInfo.Linkage = 1;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = "MaterialChange";
		LatentInfo.UUID = __LINE__;
		UKismetSystemLibrary::Delay(this, 0.5f, LatentInfo);
	}

	if (Player->GetKilled() >= Player->GetTarget()) Player->EndGame();
}

void AMove::MaterialChange()
{
	Mesh->SetMaterial(0, Origin);
}

void AMove::ChangeDirection()
{
	Direction *= -1;
}

