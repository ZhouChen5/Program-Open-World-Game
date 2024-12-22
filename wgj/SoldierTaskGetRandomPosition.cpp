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
	//��ȡ�ڰ��AIcontroller
	AAIController* Controller = OwnerComp.GetAIOwner();
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	if (!Controller || !blackboard)
		return EBTNodeResult::Failed;

	//��ȡAIcontroller��Ӧ��Pawn
	APawn* pawn = Controller->GetPawn();
	if(!pawn)
		return EBTNodeResult::Failed;

	FVector location = pawn->GetActorLocation();

	//��ȡ����ϵͳ
	UNavigationSystemV1* naviSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

	if(!naviSystem)
		return EBTNodeResult::Failed;

	//���ݵ����������λ��
	FNavLocation navLocation;
	bool isFound=naviSystem->GetRandomReachablePointInRadius(location,Radius,navLocation);

	if(!isFound)
		return EBTNodeResult::Failed;

	//���������λ�ø�ֵ���ڰ��
	blackboard->SetValueAsVector(MovePositionKey.SelectedKeyName, navLocation.Location);
	return EBTNodeResult::Succeeded;
}
