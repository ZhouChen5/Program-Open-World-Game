// Fill out your copyright notice in the Description page of Project Settings.

#include "TaskGetRandomPositionNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PalAIController.h"
#include "NavigationSystem.h"
#include "DebugHelper.h"

UTaskGetRandomPositionNode::UTaskGetRandomPositionNode()
{
	NodeName = FString("GetRandomPosition");
}

EBTNodeResult::Type UTaskGetRandomPositionNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* controller = OwnerComp.GetAIOwner();
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	if (!(controller) || !(blackboard)) {
		return EBTNodeResult::Failed;
	}

	APawn* pawn = controller->GetPawn();
	if (!(pawn)) {
		return EBTNodeResult::Failed;
	}
	
	FVector location = pawn->GetActorLocation();

	UNavigationSystemV1* Navisystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	if (!(Navisystem)) {
		return EBTNodeResult::Failed;
	}

	FNavLocation navLocation;
	bool isFound = Navisystem->GetRandomReachablePointInRadius(location, Radium, navLocation);

	if (!(isFound)) return EBTNodeResult::Failed;

	Debug::Print("Get here!");
	blackboard->SetValueAsVector(MovePositionKey.SelectedKeyName, navLocation.Location);

	return EBTNodeResult::Succeeded;
}
