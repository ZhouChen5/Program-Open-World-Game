#include "UAC_Inventory.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// 函数实现开始，此函数用于根据给定的对象类型数组进行交互追踪相关操作
void UAC_Inventory::InteractTrace(TArray<TEnumAsByte<EObjectTypeQuery> > TraceObject) {
    if (!GetWorld()) {
        return;  // 如果没有获取到当前世界，直接返回，无法进行后续交互追踪操作
    }

    ACharacter* ownerCharacter = OwningCharacterReference.Get();
    if (!ownerCharacter) {
        return;  // 如果没有拥有者角色，也无法进行有效的交互追踪，直接返回
    }

    FVector startLocation = ownerCharacter->GetActorLocation();
    FVector endLocation = startLocation + (ownerCharacter->GetActorForwardVector() * TraceLengthPic kUpItems);  // 假设沿着角色前方一定距离为追踪终点，这里使用了类中的变量TraceLengthPic kUpItems

    // 设置碰撞查询参数
    FCollisionQueryParams collisionParams;
    collisionParams.AddIgnoredActor(ownerCharacter);  // 忽略拥有者角色自身，避免误检测
    collisionParams.bTraceComplex = true;  // 进行复杂形状的碰撞检测（根据实际需求可调整）

    // 遍历给定的对象类型数组，逐个进行碰撞检测查找可交互对象
    for (int32 i = 0; i < TraceObject.Num(); ++i) {
        TEnumAsByte<EObjectTypeQuery> currentObjectType = TraceObject[i];

        // 设置碰撞对象类型参数
        FCollisionObjectQueryParams objectQueryParams;
        objectQueryParams.AddObjectTypesToQuery(currentObjectType);

        TArray<FHitResult> hitResults;
        // 进行射线检测，查找符合对象类型的碰撞对象
        bool bHit = GetWorld()->LineTraceMultiByObjectType(hitResults, startLocation, endLocation, objectQueryParams, collisionParams);

        if (bHit) {
            // 如果检测到了符合类型的对象，进行相应的交互逻辑处理，这里简单打印相关信息示例
            for (const FHitResult& hitResult : hitResults) {
                AActor* hitActor = hitResult.GetActor();
                if (hitActor) {
                    UE_LOG(LogTemp, Warning, TEXT("InteractTrace: Found interactable actor of type %d: %s"), (int32)currentObjectType, *hitActor->GetName());
                    // 可以在这里添加更多具体的交互逻辑，比如触发UI显示、执行特定行为等
                    // 例如，调用一个假设存在的函数来处理与该对象的交互
                    // HandleInteractionWithActor(hitActor);
                }
            }
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("InteractTrace: No interactable actor found for type %d"), (int32)currentObjectType);
        }

        // 以下是简单的调试绘制射线，方便查看检测范围（可根据实际需求决定是否保留）
#if WITH_EDITOR
        FColor debugColor = FColor::Green;
        if (!bHit) {
            debugColor = FColor::Red;
        }
        DrawDebugLine(GetWorld(), startLocation, endLocation, debugColor, false, 1.0f);
#endif
    }
}