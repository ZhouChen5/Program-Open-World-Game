// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierTaskGetRandomPosition.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

USoldierTaskGetRandomPosition::USoldierTaskGetRandomPosition()
{
	NodeName = FString("GetRandomPosition");

}

EBTNodeResult::Type USoldierTaskGetRandomPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//获取黑板和AIcontroller
	AAIController* Controller = OwnerComp.GetAIOwner();
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	if (!Controller || !blackboard)
		return EBTNodeResult::Failed;

	//获取AIcontroller对应的Pawn
	APawn* pawn = Controller->GetPawn();
	if(!pawn)
		return EBTNodeResult::Failed;

	FVector location = pawn->GetActorLocation();

	//获取导航系统
	UNavigationSystemV1* naviSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

	if(!naviSystem)
		return EBTNodeResult::Failed;

	//根据导航网络随机位置
	FNavLocation navLocation;
	bool isFound=naviSystem->GetRandomReachablePointInRadius(location,Radius,navLocation);

	if(!isFound)
		return EBTNodeResult::Failed;

	//将随机到的位置赋值给黑板键
	blackboard->SetValueAsVector(MovePositionKey.SelectedKeyName, navLocation.Location);
	return EBTNodeResult::Succeeded;
}
