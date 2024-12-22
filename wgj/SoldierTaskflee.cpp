// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierTaskflee.h"
#include "BaseCharacter.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

USoldierTaskFleeAway::USoldierTaskFleeAway()
{
	NodeName = FString("FleeAway");
}

EBTNodeResult::Type USoldierTaskFleeAway::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* controller = OwnerComp.GetAIOwner();

	if (!controller)
		return EBTNodeResult::Failed;

	ABaseCharacter* enemy = Cast<ABaseCharacter>(controller->GetPawn());
	if (!enemy)
		return EBTNodeResult::Failed;

	enemy->GetCharacterMovement()->MaxWalkSpeed = speed;

	return EBTNodeResult::Succeeded;
}
