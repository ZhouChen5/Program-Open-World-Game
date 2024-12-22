// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SoldierTaskGetRandomPosition.generated.h"

/**
 * 
 */
UCLASS()
class PALUWORLD_API USoldierTaskGetRandomPosition : public UBTTaskNode
{
	GENERATED_BODY()

public:
	USoldierTaskGetRandomPosition();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
protected:
	UPROPERTY(EditAnywhere,BluePrintReadOnly,Category="AI")
	FBlackboardKeySelector MovePositionKey;

	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = "AI")
	float Radius = 1000.0f;
};
