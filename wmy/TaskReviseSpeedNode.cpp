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
	//��ȡai���
	AAIController* controller = OwnerComp.GetAIOwner();

	if (!(controller)) {
		return EBTNodeResult::Failed;
	}
	//��ȡ����
	ABasePalCharacter* Pal = Cast<ABasePalCharacter>(controller->GetPawn());

	if (!(Pal)) {
		return EBTNodeResult::Failed;
	}
	//�����ٶȱ���
	Pal->GetCharacterMovement()->MaxWalkSpeed = Speed;

	return EBTNodeResult::Succeeded;
}
