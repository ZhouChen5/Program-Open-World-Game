// Fill out your copyright notice in the Description page of Project Settings.


#include "PalSupermaleAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "PlayerBaseCharacter.h"
#include "AboutScreen.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Factions.h"
#include "BasePalCharacter.h"

//��ʼ����
float mindistance = 10000000.f;

APalSupermaleAIController::APalSupermaleAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");
}

void APalSupermaleAIController::BeginPlay()
{
	Super::BeginPlay();
	//��ʼ�������úܴ�
	mindistance = 10000000.f;
	//������
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &APalSupermaleAIController::OnPerceptionUpdate);
}


void APalSupermaleAIController::OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	//�Լ�
	APawn* own = GetPawn();
	//�з�
	APawn* player = Cast<APawn>(Actor);

	//����Ƿ�ɹ�ת��
	if (player && own) {
		//��ʶ���Լ�
		if (player == own) {
			return;
		}

		//ͨ����Ӫ�ж��Ƿ�Ϊ����
		Faction ownFaction = Cast<ABasePalCharacter>(own)->GetFaction();
		ABasePalCharacter* player1 = Cast<ABasePalCharacter>(player);
		APlayerBaseCharacter* player2 = Cast<APlayerBaseCharacter>(player);
		if (player1) {    //Ϊ��³
			Faction enemyFaction = player1->GetFaction();
			bool isEnemy = IsEnemy(ownFaction, enemyFaction);
			if (!isEnemy) {
				return;
			}
		}
		else if (player2) {   //Ϊ���
			Faction enemyFaction = player2->GetFaction();
			bool isEnemy = IsEnemy(ownFaction, enemyFaction);
			if (!isEnemy) {
				return;
			}

		}

		//����Ƿ�Ϊ�������ĵ���
		float distance = (own->GetActorLocation() - own->GetActorLocation()).Size();
		if (distance >= mindistance) {
			return;
		}

		//��ȡ�ڰ����
		UBlackboardComponent* blackboard = GetBlackboardComponent();
		//���ڰ��Ƿ����
		if (blackboard) {
			blackboard->SetValueAsObject(FName("PlayerActor"), player);
		}
	}

}
