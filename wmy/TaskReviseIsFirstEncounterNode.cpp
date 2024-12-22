// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskReviseIsFirstEncounterNode.h"
#include "BehaviorTree/BlackboardComponent.h"

UTaskReviseIsFirstEncounterNode::UTaskReviseIsFirstEncounterNode()
{
	NodeName = FString("ReviseEncounter");
}

EBTNodeResult::Type UTaskReviseIsFirstEncounterNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	//�����Ƿ���ںڰ����
	if (!blackboard) {
		return EBTNodeResult::Failed;
	}

	blackboard->SetValueAsBool("IsFirstEncounter", Encounter);
	return EBTNodeResult::Succeeded;
}
