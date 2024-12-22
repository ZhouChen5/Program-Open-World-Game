// ThrowWeapon.h
#pragma once

#include "PlayerBaseCharacter.h"
#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ThrowWeapon.generated.h"

/**
 *
 */
UCLASS()
class PALUWORLD_API AThrowWeapon : public ABaseWeapon
{
    GENERATED_BODY()
private:
    UProjectileMovementComponent* ProjectileMovementComponent;

    virtual void Tick(float DeltaTime) override;
    
public:
    AThrowWeapon();
    // 可以添加一些方便的函数来操作这个组件
    UProjectileMovementComponent* GetProjectileMovementComponent() const;

    // 发射投掷物
    void Launch(int32 speed);

    //设置回复为模拟物理的时间
    int timer = 10;
    //碰撞箱
    UPROPERTY(VisibleAnywhere);
    UBoxComponent* ballbox;

    UPROPERTY(EditAnywhere);
    UStaticMeshComponent* StaticMeshComponent;

    FTimerHandle TimerHandle;

    void AfterThrow();
  
    virtual void  ReceiveActorBeginOverlap(AActor* OtherActor);
 };