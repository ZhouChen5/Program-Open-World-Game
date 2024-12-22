// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIController.h"
#include "BossCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DebugHelper.h"

void ABossAIController::BeginPlay()
{
	Super::BeginPlay();
	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (playerPawn)
	{
		SetFocus(playerPawn);
		ABossCharacter* enemy = Cast<ABossCharacter>(GetPawn());
		
		Debug::print("Enemy Behavior Tree Run!");
		RunBehaviorTree(enemy->BehaviorTree);

		UBlackboardComponent* blackboard = GetBlackboardComponent();

		blackboard->SetValueAsObject(FName("PlayerActor"), playerPawn);
	}
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ABossCharacter* boss = Cast<ABossCharacter>(InPawn);

	if (boss)
	{
		RunBehaviorTree(boss->BehaviorTree);
		APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		if (playerPawn)
		{
			SetFocus(playerPawn);

			UBlackboardComponent* blackboard= GetBlackboardComponent();
			if (blackboard)
			{
				blackboard->SetValueAsObject(FName("PlayerActor"), playerPawn);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Behavior tree could not find player in blackboard"));
		}
	}
}
