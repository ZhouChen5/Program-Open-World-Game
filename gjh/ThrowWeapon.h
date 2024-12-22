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
    // �������һЩ����ĺ���������������
    UProjectileMovementComponent* GetProjectileMovementComponent() const;

    // ����Ͷ����
    void Launch(int32 speed);

    //���ûظ�Ϊģ�������ʱ��
    int timer = 10;
    //��ײ��
    UPROPERTY(VisibleAnywhere);
    UBoxComponent* ballbox;

    UPROPERTY(EditAnywhere);
    UStaticMeshComponent* StaticMeshComponent;

    FTimerHandle TimerHandle;

    void AfterThrow();
  
    virtual void  ReceiveActorBeginOverlap(AActor* OtherActor);
 };