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

	//执行休息动画0或1
	float RandomNum = FMath::RandRange(0.0f, 2.0f);
	//随机播放休息动画
	if (RandomNum <= 1) {
		Pal->PlayRest0();
	}
	else {
		Pal->PlayRest1();
	}

	return EBTNodeResult::Succeeded;
}
