// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AAmmo::AAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(Mesh);

	AmmoPickUp = 15;
}

// Called when the game starts or when spawned
void AAmmo::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAmmo::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AMyProject2Character* Player = Cast<AMyProject2Character>(OtherActor);
	if (Player) Player->SetAmmoText(AmmoPickUp + Player->GetAmmo());
	UGameplayStatics::PlaySoundAtLocation(this, PickUpSound, GetActorLocation(), .5f);
	Destroy();
}

// Called every frame
void AAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

