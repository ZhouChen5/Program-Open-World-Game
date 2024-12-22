// Fill out your copyright notice in the Description page of Project Settings.


#include "Trunk.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DebugHelper.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerBaseCharacter.h"
// Sets default values
ATrunk::ATrunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 创建 StaticMeshComponent，但实际上它是由蓝图中创建和配置的
	tree = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("trunk"));
	RootComponent = tree;  // 设置为根组件

	// 创建 Box 盒体碰撞组件
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollisionComponent->SetupAttachment(RootComponent);  // 将 Box 组件附加到根组件下

	// 启用碰撞
	BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);  // 启用物理和查询碰撞
	BoxCollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);  // 默认忽略所有碰撞


	// 创建并附加Niagara特效组件
	NiagaraEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
	NiagaraEffectComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATrunk::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrunk::AttackDamage()
{
}

void ATrunk::TakeHitDamage(const float damageValue)
{
	if (health > 0)
	{
		health -= damageValue;
		if (health <= 0)
		{
			health = 0;
			DeathResponse();   //没血了死亡
		}
		else
			HitResponse();     //还有血伤害
	}
}

void ATrunk::HitResponse()
{
}

void ATrunk::DeathResponse()
{
}

void ATrunk::Chop_Implementation(float damage, FVector hitlocation)
{
	damage = getdamage;
	//更改血量
	health = FMath::Clamp(health - damage, 0, maxhealth);

	float middlehealth = health / maxhealth;


	TrunkMaterial->SetScalarParameterValue("Strength", 1 - middlehealth);

	//计算旋转起始位置
	FVector niagaralocation = NiagaraEffectComponent->GetComponentLocation();

	// 计算角色到目标位置的旋转
	FRotator LookAtRotation = FRotationMatrix::MakeFromX(hitlocation - niagaralocation).Rotator();

	//对x,y的旋转均置0；
	LookAtRotation.Pitch = 0.0f;
	LookAtRotation.Roll = 0.0f;
	LookAtRotation.Yaw -= 20.0f;
	//执行旋转
	NiagaraEffectComponent->SetWorldRotation(LookAtRotation);

	NiagaraEffectComponent->Activate(true);
}



void ATrunk::OnConstruction(const FTransform& Transform)
{
}

