// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy2AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "PlayerBaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemy2AIController::AEnemy2AIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");
}

void AEnemy2AIController::BeginPlay()
{
	Super::BeginPlay();

	//AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this,&AEnemy2AIController::OnPerceptionUpdate);
}

void AEnemy2AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemy2AIController::OnPerceptionUpdate);
}

void AEnemy2AIController::OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	APlayerBaseCharacter* player= Cast<APlayerBaseCharacter>(Actor);

	if (player)
	{
		UBlackboardComponent* blackboard = GetBlackboardComponent();

		if (blackboard)
		{
			blackboard->SetValueAsObject(FName("PlayerActor"), player);
		}
	}
}
