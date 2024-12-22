// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SoldierTaskAttackPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PALUWORLD_API USoldierTaskAttackPlayer : public UBTTaskNode
{
	GENERATED_BODY()
public:
	USoldierTaskAttackPlayer();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
