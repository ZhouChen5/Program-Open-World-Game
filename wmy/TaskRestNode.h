// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TaskRestNode.generated.h"

/**
 * 
 */
UCLASS()
class PALUWORLD_API UTaskRestNode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UTaskRestNode();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
