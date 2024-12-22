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
    //���μ���Ƿ����³����
    {
        
        UWorld* World = GetWorld();
        if (World == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to get the world pointer."));
            return;
        }


        // ���������λ�ã�����ʹ�õ�ǰ Actor ��λ��
        FVector Center = GetActorLocation();
        // ����뾶
        float Radius = 30.0f;
        TArray<AActor*> ignoreactor;
        if (GetOwner())
            ignoreactor.Add(GetOwner());

        TArray<FHitResult> Result;


        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQueries;
        // �������Ҫ�Ķ������Ͳ�ѯ�����磺
        ObjectTypeQueries.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));


        // �������μ��
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

        // �����⵽�� Actor �б�
       
        if (isResult) {
            auto hit=Result[0].GetActor();
            if (hit->IsA(ABasePalCharacter::StaticClass())) {
                Debug::Print("hit a Palu");
                auto hitpalu = Cast<ABasePalCharacter>(hit);
                if (hitpalu) {
                    // ��������棬ʹ������豸��Ϊ����           
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
                    // ��������棬ʹ������豸��Ϊ����           
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
    // ��������ʼ�� ProjectileMovementComponent
    ProjectileMovementComponent = this->CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    // ���ó�ʼ����
   
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
    // �����δ���� ProjectileMovementComponent���򴴽���
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
            // �� speed ת��Ϊ FVector ����
            FVector launchVelocity = speed*launchDirection;
            // ʹ�� ProjectileMovementComponent �����ǲ�����
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

