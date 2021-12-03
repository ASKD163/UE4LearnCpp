// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MyProject2Projectile.h"
#include "Move.generated.h"

UCLASS()
class MYPROJECT2_API AMove : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* Material;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* Origin;

	UPROPERTY(VisibleAnywhere)
	bool isDestroyed;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* Sound;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* Emitter;
	
public:	
	// Sets default values for this actor's properties
	AMove();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp,
		bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	UFUNCTION()
	void MaterialChange();
	FVector Direction;
	
	UFUNCTION()
	void ChangeDirection();
};
