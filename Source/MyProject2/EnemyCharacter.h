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

	UPROPERTY(EditInstanceOnly)
	AActor* PatrolPoint1;
	
	UPROPERTY(EditInstanceOnly)
	AActor* PatrolPoint2;

	UFUNCTION()
	void SeePawn(APawn* Player);
	
public:
	// Sets default values for this character's properties
	AEnemyCharacter();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	AActor* CurrentPoin;
};
