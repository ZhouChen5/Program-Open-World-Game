// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAttackPlayer.h"
#include "BossCharacter.h"
#include "AIController.h"

UBossAttackPlayer::UBossAttackPlayer()
{
	NodeName = FString("BossAttackPlayer");
	bNotifyTick = true;
}

EBTNodeResult::Type UBossAttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* controller = OwnerComp.GetAIOwner();

	if (!controller)
		return EBTNodeResult::Failed;

	ABossCharacter* Boss = Cast<ABossCharacter>(controller->GetPawn());

	if (!Boss)
		return EBTNodeResult::Failed;

	Boss->PlayAttack();

	return EBTNodeResult::InProgress;
}

void UBossAttackPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* controller = OwnerComp.GetAIOwner();

	if (controller)
	{
		ABossCharacter* boss = Cast<ABossCharacter>(controller->GetPawn());
		if (boss && !boss->isAttacking)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		}
	}

}
