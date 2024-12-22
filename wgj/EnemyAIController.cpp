// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "DebugHelper.h"
#include "EnemyBaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Soldier1Character.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	////ÄÃµ½Íæ¼ÒµÄPawn
	//APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	//if (playerPawn)
	//{
	//	/*Debug::print("enemy go!");
	//	SetFocus(playerPawn);

	//	MoveToActor(playerPawn);*/
	//	AEnemyBaseCharacter* enemy = Cast<AEnemyBaseCharacter>(GetPawn());

	//	Debug::print("Enemy Behavior Tree Run!");
	//	RunBehaviorTree(enemy->BehaviorTree);

	//	UBlackboardComponent* blackboard = GetBlackboardComponent();

	//	//blackboard->SetValueAsObject(FName("PlayerActor"), playerPawn);
	//}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AEnemyBaseCharacter* enemy = Cast<AEnemyBaseCharacter>(GetPawn());

	if (enemy)
	{
		RunBehaviorTree(enemy->BehaviorTree);
	}
	else
	{
		ASoldier1Character* soldier = Cast<ASoldier1Character>(GetPawn());
		if (soldier)
		{
			RunBehaviorTree(soldier->BehaviorTree);
		}
		else
		{
			Debug::print("no soldier behavior!");
		}
	}
}

void AEnemyAIController::SetAIDamage()
{
	UBlackboardComponent* blackboard = GetBlackboardComponent();

	if (blackboard)
	{
		blackboard->SetValueAsBool(FName("IsDamage"), true);

		APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		
		if (playerPawn)
		{
			SetFocus(playerPawn);
			blackboard->SetValueAsObject(FName("PlayerActor"), playerPawn);
		}
	}
}
