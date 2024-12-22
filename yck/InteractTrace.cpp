#include "UAC_Inventory.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// ����ʵ�ֿ�ʼ���˺������ڸ��ݸ����Ķ�������������н���׷����ز���
void UAC_Inventory::InteractTrace(TArray<TEnumAsByte<EObjectTypeQuery> > TraceObject) {
    if (!GetWorld()) {
        return;  // ���û�л�ȡ����ǰ���磬ֱ�ӷ��أ��޷����к�������׷�ٲ���
    }

    ACharacter* ownerCharacter = OwningCharacterReference.Get();
    if (!ownerCharacter) {
        return;  // ���û��ӵ���߽�ɫ��Ҳ�޷�������Ч�Ľ���׷�٣�ֱ�ӷ���
    }

    FVector startLocation = ownerCharacter->GetActorLocation();
    FVector endLocation = startLocation + (ownerCharacter->GetActorForwardVector() * TraceLengthPic kUpItems);  // �������Ž�ɫǰ��һ������Ϊ׷���յ㣬����ʹ�������еı���TraceLengthPic kUpItems

    // ������ײ��ѯ����
    FCollisionQueryParams collisionParams;
    collisionParams.AddIgnoredActor(ownerCharacter);  // ����ӵ���߽�ɫ������������
    collisionParams.bTraceComplex = true;  // ���и�����״����ײ��⣨����ʵ������ɵ�����

    // ���������Ķ����������飬���������ײ�����ҿɽ�������
    for (int32 i = 0; i < TraceObject.Num(); ++i) {
        TEnumAsByte<EObjectTypeQuery> currentObjectType = TraceObject[i];

        // ������ײ�������Ͳ���
        FCollisionObjectQueryParams objectQueryParams;
        objectQueryParams.AddObjectTypesToQuery(currentObjectType);

        TArray<FHitResult> hitResults;
        // �������߼�⣬���ҷ��϶������͵���ײ����
        bool bHit = GetWorld()->LineTraceMultiByObjectType(hitResults, startLocation, endLocation, objectQueryParams, collisionParams);

        if (bHit) {
            // �����⵽�˷������͵Ķ��󣬽�����Ӧ�Ľ����߼���������򵥴�ӡ�����Ϣʾ��
            for (const FHitResult& hitResult : hitResults) {
                AActor* hitActor = hitResult.GetActor();
                if (hitActor) {
                    UE_LOG(LogTemp, Warning, TEXT("InteractTrace: Found interactable actor of type %d: %s"), (int32)currentObjectType, *hitActor->GetName());
                    // ������������Ӹ������Ľ����߼������紥��UI��ʾ��ִ���ض���Ϊ��
                    // ���磬����һ��������ڵĺ�����������ö���Ľ���
                    // HandleInteractionWithActor(hitActor);
                }
            }
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("InteractTrace: No interactable actor found for type %d"), (int32)currentObjectType);
        }

        // �����Ǽ򵥵ĵ��Ի������ߣ�����鿴��ⷶΧ���ɸ���ʵ����������Ƿ�����
#if WITH_EDITOR
        FColor debugColor = FColor::Green;
        if (!bHit) {
            debugColor = FColor::Red;
        }
        DrawDebugLine(GetWorld(), startLocation, endLocation, debugColor, false, 1.0f);
#endif
    }
}