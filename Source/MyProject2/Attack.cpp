// Fill out your copyright notice in the Description page of Project Settings.


#include "Attack.h"
#include "Kismet/GameplayStatics.h"
#include "MyProject2Character.h"

EBTNodeResult::Type UAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* Player = Cast<AMyProject2Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		UGameplayStatics::ApplyDamage(Player, 0.3f, nullptr, nullptr,
			UDamageType::StaticClass());
	}
	return EBTNodeResult::Succeeded;
}
