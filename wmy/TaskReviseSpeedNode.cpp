// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskReviseSpeedNode.h"
#include "BasePalCharacter.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UTaskReviseSpeedNode::UTaskReviseSpeedNode()
{
	NodeName = FString("SetAISpeed");
}

EBTNodeResult::Type UTaskReviseSpeedNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//获取ai组件
	AAIController* controller = OwnerComp.GetAIOwner();

	if (!(controller)) {
		return EBTNodeResult::Failed;
	}
	//获取基类
	ABasePalCharacter* Pal = Cast<ABasePalCharacter>(controller->GetPawn());

	if (!(Pal)) {
		return EBTNodeResult::Failed;
	}
	//设置速度变量
	Pal->GetCharacterMovement()->MaxWalkSpeed = Speed;

	return EBTNodeResult::Succeeded;
}
