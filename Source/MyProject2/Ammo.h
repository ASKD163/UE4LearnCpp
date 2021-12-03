// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "MyProject2Character.h"
#include "Ammo.generated.h"

UCLASS()
class MYPROJECT2_API AAmmo : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* Capsule;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* PickUpSound;

	int32 AmmoPickUp;
	
public:	
	// Sets default values for this actor's properties
	AAmmo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
