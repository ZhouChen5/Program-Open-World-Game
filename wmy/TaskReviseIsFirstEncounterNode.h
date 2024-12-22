// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TaskReviseIsFirstEncounterNode.generated.h"

/**
 * 
 */
UCLASS()
class PALUWORLD_API UTaskReviseIsFirstEncounterNode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UTaskReviseIsFirstEncounterNode();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value")
	bool Encounter = true;
};
