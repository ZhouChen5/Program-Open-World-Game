// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAIController.h"
#include "Enemy2AIController.generated.h"

/**
 * 
 */
UCLASS()
class PALUWORLD_API AEnemy2AIController : public AEnemyAIController
{
	GENERATED_BODY()
public:
	AEnemy2AIController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="MyComponents")
	class UAIPerceptionComponent* AIPerceptionComponent;

	UFUNCTION()
	void OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);
};
