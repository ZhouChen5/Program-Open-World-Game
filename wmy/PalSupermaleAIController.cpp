// Fill out your copyright notice in the Description page of Project Settings.


#include "PalSupermaleAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "PlayerBaseCharacter.h"
#include "AboutScreen.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Factions.h"
#include "BasePalCharacter.h"

//初始距离
float mindistance = 10000000.f;

APalSupermaleAIController::APalSupermaleAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");
}

void APalSupermaleAIController::BeginPlay()
{
	Super::BeginPlay();
	//初始距离设置很大
	mindistance = 10000000.f;
	//检测敌人
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &APalSupermaleAIController::OnPerceptionUpdate);
}


void APalSupermaleAIController::OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	//自己
	APawn* own = GetPawn();
	//敌方
	APawn* player = Cast<APawn>(Actor);

	//检测是否成功转换
	if (player && own) {
		//不识别自己
		if (player == own) {
			return;
		}

		//通过阵营判断是否为敌人
		Faction ownFaction = Cast<ABasePalCharacter>(own)->GetFaction();
		ABasePalCharacter* player1 = Cast<ABasePalCharacter>(player);
		APlayerBaseCharacter* player2 = Cast<APlayerBaseCharacter>(player);
		if (player1) {    //为帕鲁
			Faction enemyFaction = player1->GetFaction();
			bool isEnemy = IsEnemy(ownFaction, enemyFaction);
			if (!isEnemy) {
				return;
			}
		}
		else if (player2) {   //为玩家
			Faction enemyFaction = player2->GetFaction();
			bool isEnemy = IsEnemy(ownFaction, enemyFaction);
			if (!isEnemy) {
				return;
			}

		}

		//检测是否为最近距离的敌人
		float distance = (own->GetActorLocation() - own->GetActorLocation()).Size();
		if (distance >= mindistance) {
			return;
		}

		//获取黑板组件
		UBlackboardComponent* blackboard = GetBlackboardComponent();
		//检测黑板是否存在
		if (blackboard) {
			blackboard->SetValueAsObject(FName("PlayerActor"), player);
		}
	}

}
