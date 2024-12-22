// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PalAIController.h"
#include "PalSupermaleAIController.generated.h"

/**
 * 
 */
UCLASS()
class PALUWORLD_API APalSupermaleAIController : public APalAIController
{
	GENERATED_BODY()

public:
	APalSupermaleAIController();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyComponents")
	class UAIPerceptionComponent* AIPerceptionComponent;

	UFUNCTION()
	void OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);
	
};
