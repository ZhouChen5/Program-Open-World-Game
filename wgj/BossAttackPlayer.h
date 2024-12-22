// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BossAttackPlayer.generated.h"

/**
 * 
 */
UCLASS()
class COMBETPROJECTS_CPP_API UBossAttackPlayer : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBossAttackPlayer();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
