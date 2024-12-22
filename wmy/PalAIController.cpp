// Fill out your copyright notice in the Description page of Project Settings.


#include "PalAIController.h"
#include "Kismet/GameplayStatics.h"
#include "DebugHelper.h"
#include "BasePalCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void APalAIController::BeginPlay()
{
	Super::BeginPlay();

	ABasePalCharacter* Pal = Cast<ABasePalCharacter>(GetPawn());
	if (Pal) {
		RunBehaviorTree(Pal->BehaviorTree);
	}

	//Debug::Print("in ai successfully");
	////拿到玩家的标记值
	//APawn* player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	//if (player) {
	//	//SetFocus(player); //朝向玩家

	//	//MoveToActor(player,150.0f);

	//	ABasePalCharacter* Pal = Cast<ABasePalCharacter>(GetPawn());

	//	RunBehaviorTree(Pal->BehaviorTree);
	//	//获取黑板值
	//	UBlackboardComponent* blackboard = GetBlackboardComponent();
	//	Debug::Print("find player successfully");
	//	blackboard->SetValueAsObject(FName("PlayerActor"), player);

	//}
}

void APalAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ABasePalCharacter* pal = Cast<ABasePalCharacter>(GetPawn());

	if (pal)
	{
		RunBehaviorTree(pal->BehaviorTree);
	}
}

void APalAIController::SetAIDamage()
{
	UBlackboardComponent* blackboard = GetBlackboardComponent();

	if (blackboard) {
		blackboard->SetValueAsBool(FName("IsDamage"), true);

		//Debug::Print("in ai successfully");
	////拿到玩家的标记值
	// 
	// 
	APawn* player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	// 
	// 
	//if (player) {
	//	//SetFocus(player); //朝向玩家

	//	//MoveToActor(player,150.0f);

	//	ABasePalCharacter* Pal = Cast<ABasePalCharacter>(GetPawn());

	//	RunBehaviorTree(Pal->BehaviorTree);
	//	//获取黑板值
	//	UBlackboardComponent* blackboard = GetBlackboardComponent();
	//	Debug::Print("find player successfully");
	// 
	// 
		blackboard->SetValueAsObject(FName("PlayerActor"), player);

	//}
		//TArray<APawn*> AllPawns;
		//UWorld* world = GetWorld();
		//if (world) {
		//	TArray<AActor*> AllActors;
		//	//获取所有的Actor
		//	UGameplayStatics::GetAllActorsOfClass(world, AActor::StaticClass(), AllActors);
		//	for (AActor* actor : AllActors) {
		//		APawn* pawn = Cast<APawn>(actor);
		//		if (pawn) {
		//			AllPawns.Add(pawn);
		//			break;
		//		}
		//	}
		//	if(AllPawns[1]){
		//		if (AllActors.Num() != 0) {
		//			APawn* ClosestDistance = AllPawns[1];
		//			blackboard->SetValueAsObject(FName("PlayerActor"), ClosestDistance);
		//		}
		//	}
		
	}
}
