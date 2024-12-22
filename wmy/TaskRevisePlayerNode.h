// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TaskRevisePlayerNode.generated.h"

/**
 *
 */
UCLASS()
class PALWORLD_API UTaskRevisePlayerNode : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTaskRevisePlayerNode();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	APawn* GetAttackRangePlayer(const TArray<FHitResult> hitList, AActor* own);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Range")
	float SearchRadium;
};
