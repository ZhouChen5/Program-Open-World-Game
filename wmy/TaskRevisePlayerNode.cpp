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
    // 获取黑板组件
    UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
    if (!blackboard)
    {
        return EBTNodeResult::Failed;
    }

    // 获取行为树所属的AI角色（假设AI角色是一个Pawn，这里获取其所在位置作为参考点）
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

    // 遍历识别到的Actor检验是不是玩家并筛选合适的Pawn
    for (FHitResult Hit : hitList)
    {
        APawn* actor = Cast<APawn>(Hit.GetActor());
        //未找到actor
        if (!actor) {
            return nullptr;
        }
        // 首先排除自己
        if (actor == own)
        {
            continue;
        }

        // 计算当前Pawn与AI角色的距离
        float distance = (actor->GetActorLocation() - own->GetActorLocation()).Size();
        //输出错误信息
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




