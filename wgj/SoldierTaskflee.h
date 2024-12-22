// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SoldierTaskflee.generated.h"

/**
 * 
 */
UCLASS()
class COMBETPROJECTS_CPP_API USoldierTaskFleeAway : public UBTTaskNode
{
	GENERATED_BODY()
public:
	USoldierTaskFleeAway();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float speed = 350;
};
