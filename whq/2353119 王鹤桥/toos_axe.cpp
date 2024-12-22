// Fill out your copyright notice in the Description page of Project Settings.

#include "toos_axe.h"
#include"DebugHelper.h"
#include"PlayerBaseCharacter.h"
#include "Kismet/GameplayStatics.h"

Atoos_axe::Atoos_axe()
{
	
	// 设定默认值
	PrimaryActorTick.bCanEverTick = true;

	// 创建骨骼网格体组件
	AxeMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AxeMesh"));
	RootComponent = AxeMeshComponent;
	AxeMeshComponent->SetVisibility(false);

	// 创建 Box 盒体碰撞组件
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollisionComponent->SetupAttachment(RootComponent);  // 将 Box 组件附加到根组件下

	// 启用碰撞
	BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);  // 启用物理和查询碰撞
	BoxCollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);  // 默认忽略所有碰撞

	// 创建 Damage 盒体碰撞组件
	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageCollision"));
	DamageCollision->SetupAttachment(RootComponent);  // 将 Box 组件附加到根组件下

	// 启用碰撞
	DamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);  // 启用物理和查询碰撞
	DamageCollision->SetCollisionResponseToAllChannels(ECR_Ignore);  // 默认忽略所有碰撞

}




void Atoos_axe::BeginPlay()
{
	Super::BeginPlay();

}

void Atoos_axe::EnableHit(bool enable)
{

	weaponowner = Cast<APlayerBaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (weaponowner)
	{
		enable = weaponowner->bIsAttack;
	}
	//如果enable为真，设置为纯物理，不查询碰撞
	if(1)
	{
		BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	//若为假，则无碰撞
	else
	{
		BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void Atoos_axe::Chop_Implementation(float damage, FVector hitlocation)
{
}
