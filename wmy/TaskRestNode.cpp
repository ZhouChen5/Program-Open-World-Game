// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskRestNode.h"
#include "AIcontroller.h"
#include "BasePalCharacter.h"
#include "Math/UnrealMathUtility.h"
#include "DebugHelper.h"

UTaskRestNode::UTaskRestNode()
{
	NodeName = FString("Rest");
}

EBTNodeResult::Type UTaskRestNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	//ִ����Ϣ����0��1
	float RandomNum = FMath::RandRange(0.0f, 2.0f);
	//���������Ϣ����
	if (RandomNum <= 1) {
		Pal->PlayRest0();
	}
	else {
		Pal->PlayRest1();
	}

	return EBTNodeResult::Succeeded;
}
