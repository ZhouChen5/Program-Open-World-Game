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
	//获取AI组件
	AAIController* controller = OwnerComp.GetAIOwner();
	//检测AI是否能够识别
	if (!(controller)) {
		return EBTNodeResult::Failed;
	}

	//获取帕鲁组件
	ABasePalCharacter* Pal = Cast<ABasePalCharacter>(controller->GetPawn());
	//检测是否能够找到帕鲁
	if (!(Pal)) {
		return EBTNodeResult::Failed;
	}

	//执行偶遇动画
	Pal->PlayEncounter();

	return EBTNodeResult::Succeeded;
}
