// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierTaskAttackPlayer.h"
#include "AIController.h"
#include "BaseCharacter.h"

USoldierTaskAttackPlayer::USoldierTaskAttackPlayer()
{
	NodeName = FString("AttackPlayer");
	bNotifyTick = true;
}

EBTNodeResult::Type USoldierTaskAttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* controller = OwnerComp.GetAIOwner();

	if (!controller)
		return EBTNodeResult::Failed;

	ABaseCharacter* enemy = Cast<ABaseCharacter>(controller->GetPawn());

	if(!enemy)
		return EBTNodeResult::Failed;

	enemy->PlayAttack();

	return EBTNodeResult::InProgress;
}

void USoldierTaskAttackPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* controller = OwnerComp.GetAIOwner();

	if (controller)
	{
		ABaseCharacter* enemy = Cast<ABaseCharacter>(controller->GetPawn());
		if (enemy && !enemy->isAttacking)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		}
	}

}
