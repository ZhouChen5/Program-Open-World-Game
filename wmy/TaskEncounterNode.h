// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TaskEncounterNode.generated.h"

/**
 * 
 */
UCLASS()
class PALUWORLD_API UTaskEncounterNode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UTaskEncounterNode();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

};
