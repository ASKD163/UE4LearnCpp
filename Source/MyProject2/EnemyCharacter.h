// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject2Character.h"
#include "GameFramework/Character.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Perception/PawnSensingComponent.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class MYPROJECT2_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY()
	UBlackboardComponent* BlackBoard;

	UFUNCTION()
	void SeePawn(APawn* Player);

	UFUNCTION()
	void OnHearPawn(APawn* Player, const FVector& Location, float Volume);

	int HP;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* Sound;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* Emitter;

	
public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit);

};
