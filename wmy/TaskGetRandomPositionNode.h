// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TaskGetRandomPositionNode.generated.h"

/**
 * 
 */
UCLASS()
class PALUWORLD_API UTaskGetRandomPositionNode : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTaskGetRandomPositionNode();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);


	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "AI")
	FBlackboardKeySelector MovePositionKey;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "AI")
	float Radium = 1000.0f;

};
