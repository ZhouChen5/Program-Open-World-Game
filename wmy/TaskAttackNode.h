// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TaskAttackNode.generated.h"

/**
 * 
 */
UCLASS()
class PALUWORLD_API UTaskAttackNode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UTaskAttackNode();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
