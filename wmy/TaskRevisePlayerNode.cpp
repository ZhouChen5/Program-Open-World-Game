// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskRevisePlayerNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BasePalCharacter.h"
#include "PlayerCharacter.h"
#include "DebugHelper.h"






UTaskRevisePlayerNode::UTaskRevisePlayerNode()
{
	NodeName = FString("RevisePlayer");
}

EBTNodeResult::Type UTaskRevisePlayerNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // ��ȡ�ڰ����
    UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
    if (!blackboard)
    {
        return EBTNodeResult::Failed;
    }

    // ��ȡ��Ϊ��������AI��ɫ������AI��ɫ��һ��Pawn�������ȡ������λ����Ϊ�ο��㣩
    AActor* OwnerActor = OwnerComp.GetOwner();
    if (!OwnerActor)
    {
        return EBTNodeResult::Failed;
    }
    FVector StartPos = OwnerActor->GetActorLocation();
    FVector EndPos = OwnerActor->GetActorForwardVector() * 1 + StartPos;

    TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
    objectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);
   
    TArray<AActor*> ignoreActors;
    TArray<FHitResult> hitList;
    bool isResult = UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), StartPos, EndPos, SearchRadium, FName(""), true, ignoreActors, EDrawDebugTrace::ForDuration, hitList, true);
    if (isResult) {

        APawn* pal =(GetAttackRangePlayer(hitList, OwnerActor));
        if (!pal) {
            return EBTNodeResult::Succeeded;
        }

        blackboard->SetValueAsObject(FName("PlayerActor"), pal);
        return EBTNodeResult::Succeeded;
    }

	return EBTNodeResult::Failed;
}



APawn* UTaskRevisePlayerNode::GetAttackRangePlayer(const TArray<FHitResult> hitList, AActor* own)
{
    APawn* closestPawn = nullptr;
    float closestDistance = FLT_MAX;

    // ����ʶ�𵽵�Actor�����ǲ�����Ҳ�ɸѡ���ʵ�Pawn
    for (FHitResult Hit : hitList)
    {
        APawn* actor = Cast<APawn>(Hit.GetActor());
        //δ�ҵ�actor
        if (!actor) {
            return nullptr;
        }
        // �����ų��Լ�
        if (actor == own)
        {
            continue;
        }

        // ���㵱ǰPawn��AI��ɫ�ľ���
        float distance = (actor->GetActorLocation() - own->GetActorLocation()).Size();
        //���������Ϣ
        FString str = "Distance";
        str += LexToString(distance);
        Debug::Print(str);

        if (distance < closestDistance)
        {
            closestDistance = distance;
            closestPawn = actor;
        }
    }

    return closestPawn;
}




