// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskEncounterNode.h"
#include "AIController.h" 
#include"BasePalCharacter.h"

UTaskEncounterNode::UTaskEncounterNode()
{
	NodeName = FString("Encounter");
}

EBTNodeResult::Type UTaskEncounterNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//��ȡAI���
	AAIController* controller = OwnerComp.GetAIOwner();
	//���AI�Ƿ��ܹ�ʶ��
	if (!(controller)) {
		return EBTNodeResult::Failed;
	}

	//��ȡ��³���
	ABasePalCharacter* Pal = Cast<ABasePalCharacter>(controller->GetPawn());
	//����Ƿ��ܹ��ҵ���³
	if (!(Pal)) {
		return EBTNodeResult::Failed;
	}

	//ִ��ż������
	Pal->PlayEncounter();

	return EBTNodeResult::Succeeded;
}
