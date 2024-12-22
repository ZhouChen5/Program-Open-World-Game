// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskAttackNode.h"
#include "AIController.h"
#include "BasePalCharacter.h"

UTaskAttackNode::UTaskAttackNode()
{
	NodeName = FString("AttackPlayer");
	bNotifyTick = true;
}



EBTNodeResult::Type UTaskAttackNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* controller = OwnerComp.GetAIOwner();
	//�Ҳ���AI
	if (!(controller)) {
		return EBTNodeResult::Failed;
	}

	ABasePalCharacter* Pal = Cast<ABasePalCharacter>(controller->GetPawn());
	//�Ҳ����������
	if (!(Pal)) {
		return EBTNodeResult::Failed;
	}

	Pal->PlayAttack();

	return EBTNodeResult::InProgress;
}

void UTaskAttackNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* controller = OwnerComp.GetAIOwner();
	//�Ҳ���AI
	if (controller) {

		ABasePalCharacter* Pal = Cast<ABasePalCharacter>(controller->GetPawn());
		//�Ҳ�����Ӧ����
		if (Pal && !Pal->IsAttacking) {
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}

}
