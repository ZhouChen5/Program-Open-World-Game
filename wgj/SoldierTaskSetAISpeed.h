// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SoldierTaskSetAISpeed.generated.h"

/**
 * 
 */
UCLASS()
class PALUWORLD_API USoldierTaskSetAISpeed : public UBTTaskNode
{
	GENERATED_BODY()
public:
	USoldierTaskSetAISpeed();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
	float speed = 175;
};
