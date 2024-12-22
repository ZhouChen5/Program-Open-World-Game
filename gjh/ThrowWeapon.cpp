// ThrowWeapon.cpp
#include "ThrowWeapon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BasePalCharacter.h"
#include "BaseCharacter.h"
#include "DebugHelper.h"

void AThrowWeapon::Tick(float deltatime)
{
    Super::Tick(deltatime);
    if (timer > 0) {
        timer--;
    }
    else if (timer == 0) {
        StaticMeshComponent->SetSimulatePhysics(true);
        timer--;
    }
    //球形检测是否打到帕鲁身上
    {
        
        UWorld* World = GetWorld();
        if (World == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to get the world pointer."));
            return;
        }


        // 球体的中心位置，这里使用当前 Actor 的位置
        FVector Center = GetActorLocation();
        // 球体半径
        float Radius = 30.0f;
        TArray<AActor*> ignoreactor;
        if (GetOwner())
            ignoreactor.Add(GetOwner());

        TArray<FHitResult> Result;


        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQueries;
        // 添加你需要的对象类型查询，例如：
        ObjectTypeQueries.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));


        // 进行球形检测
        bool isResult=UKismetSystemLibrary::SphereTraceMultiByProfile(
            World, 
            Center, 
            Center, 
            Radius, 
            FName(""), 
            true, 
            ignoreactor, 
            EDrawDebugTrace::None, 
            Result, 
            true);

        // 处理检测到的 Actor 列表
       
        if (isResult) {
            auto hit=Result[0].GetActor();
            if (hit->IsA(ABasePalCharacter::StaticClass())) {
                Debug::Print("hit a Palu");
                auto hitpalu = Cast<ABasePalCharacter>(hit);
                if (hitpalu) {
                    // 随机数引擎，使用随机设备作为种子           
                    double randomValue = FMath::Rand();
                    if (randomValue >= 0.5) {
                        hitpalu->TakeHitDamage(100000000);
                    }
                    else {
                        hitpalu->TakeHitDamage(0);
                    }
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("ABaseWeapon Pointer Address: %p"), this));

                    if (this)
                        this->Destroy();
                }

            }
            if (hit->IsA(ABaseCharacter::StaticClass())) {
                Debug::Print("hit a Enemy");
                auto hitEnemy = Cast<ABaseCharacter>(hit);
                if (hitEnemy) {
                    // 随机数引擎，使用随机设备作为种子           
                    double randomValue = FMath::Rand();
                    if (randomValue >= 0.5) {
                        hitEnemy->TakeHitDamage(100000000);
                    }
                    else {
                        hitEnemy->TakeHitDamage(0);
                    }
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("ABaseWeapon Pointer Address: %p"), this));

                    if (this)
                        this->Destroy();
                }

            }

        }

    }
}
AThrowWeapon::AThrowWeapon()
{
    // 创建并初始化 ProjectileMovementComponent
    ProjectileMovementComponent = this->CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    // 设置初始属性
   
    ProjectileMovementComponent->InitialSpeed = 0;
    
    ProjectileMovementComponent->MaxSpeed = 0;
    
    ProjectileMovementComponent->ProjectileGravityScale = 1.0f;
    
    ProjectileMovementComponent->SetVelocityInLocalSpace(FVector(0, 0, 0));

    ballbox = CreateDefaultSubobject<UBoxComponent>(TEXT("ballbox"));

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

    RootComp->SetSimulatePhysics(true);

    //
    
    MainMesh->SetSimulatePhysics(false);


    Debug::Print("starttimer");
    if(GetWorld())
    GetWorldTimerManager().SetTimer(TimerHandle, this, &AThrowWeapon::AfterThrow, 3, false, 3);

}


UProjectileMovementComponent* AThrowWeapon::GetProjectileMovementComponent() const
{
    return ProjectileMovementComponent;
}


void AThrowWeapon::Launch(int32 speed)
{
    // 如果尚未创建 ProjectileMovementComponent，则创建它
    if (!ProjectileMovementComponent)
    {
        ProjectileMovementComponent = this->CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    }

    if (GetOwner())
    {
        auto ownercharacter = Cast<APlayerBaseCharacter>(GetOwner());
        if (ownercharacter)
        {
            auto launchDirection = ownercharacter->GetActorForwardVector();
            // 将 speed 转换为 FVector 类型
            FVector launchVelocity = speed*launchDirection;
            // 使用 ProjectileMovementComponent 而不是查找它
            ProjectileMovementComponent->SetVelocityInLocalSpace(launchVelocity * launchDirection);

        }
    }
    else {
        Debug::Print("fail");
    }
}

void AThrowWeapon::AfterThrow()
{
    if (this) {
        Debug::Print("DESTORY");


        this->Destroy();
    }


}

void AThrowWeapon::ReceiveActorBeginOverlap(AActor* OtherActor)
{
    Super::ReceiveActorBeginOverlap(OtherActor);
    if (OtherActor) {
        Debug::Print(OtherActor->GetName());
    }
}

