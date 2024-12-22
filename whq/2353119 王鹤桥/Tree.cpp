// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DebugHelper.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerBaseCharacter.h"

ATree::ATree()
{
	// 创建 StaticMeshComponent，但实际上它是由蓝图中创建和配置的
	tree = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("tree"));
	RootComponent = tree;  // 设置为根组件

	// 创建 Box 盒体碰撞组件
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollisionComponent->SetupAttachment(RootComponent);  // 将 Box 组件附加到根组件下

	// 启用碰撞
	BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);  // 启用物理和查询碰撞

	treeup = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("treeup"));
	treeup->SetupAttachment(RootComponent);  // 将树干组件附加到根组件下

	treedown = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("treedown"));
	treedown->SetupAttachment(RootComponent);  // 将树根组件附加到根组件下


	// 创建并附加Niagara特效组件
	NiagaraEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
	NiagaraEffectComponent->SetupAttachment(treeup);

}

void ATree::BeginPlay()
{
	Super::BeginPlay();

}

void ATree::Tick(float DealtaTime)
{


}

void ATree::AttackDamage()
{

}


//受到伤害后的扣血处理
void ATree::TakeHitDamage(const float damageValue)
{
	if (health > 0)
	{
		health -= damageValue;
		Debug::print("hit111111111111111111");
		if (health <= 0)
		{
			health = 0;
			DeathResponse();   //没血了死亡
		}
		else
			HitResponse();     //还有血伤害
	}
	else
	{
		//让原有的树不再可见
		tree->SetVisibility(false);
		//设置原有碰撞消失
		tree->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//设置新树可见
		treeup->SetVisibility(true);
		treedown->SetVisibility(true);
		//设置新碰撞启用
		treeup->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		treedown->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		treeup->SetSimulatePhysics(true);

		//设置倒下效果
		FVector impulse(50.0f, 10.0f, 0.0f);
		treeup->AddImpulse(impulse, NAME_None, true);
		if (GetWorld())
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ATree::OnCutting, 20, false, 3);

	}
}



void ATree::HitResponse()
{

}

void ATree::DeathResponse()
{

}

void ATree::Chop_Implementation(float damage, FVector hitlocation)
{
	damage = 80;
	Debug::print("kan");
	damage = getdamage;
	//更改血量
	health = FMath::Clamp(health - damage, -100, maxhealth);

	float middlehealth = health / maxhealth;


	TrunkMaterial->SetScalarParameterValue("Strength", 1 - middlehealth);

	//计算旋转起始位置
	FVector niagaralocation = NiagaraEffectComponent->GetComponentLocation();

	// 计算角色到目标位置的旋转
	FRotator LookAtRotation = FRotationMatrix::MakeFromX(hitlocation - niagaralocation).Rotator();

	float xnum = NiagaraEffectComponent->GetComponentRotation().Pitch;
	float ynum = NiagaraEffectComponent->GetComponentRotation().Roll;
	//对x,y的旋转均置0；
	LookAtRotation.Pitch = xnum;
	LookAtRotation.Roll = ynum;
	LookAtRotation.Yaw -= 20.0f;
	//执行旋转
	NiagaraEffectComponent->SetWorldRotation(LookAtRotation);

	NiagaraEffectComponent->Activate(true);
	//检查当前血量
	if (health <= 0)
	{
		CutResponse();
		hasfallen = true;
		if (!hasbeencut)
		{
			Debug::Print("kandaole");
			hasbeencut = true;
		}
		else
		{
			Debug::Print("kansile");
			if (health <= (-1 * trunkhealth))
				OnCutting();
		}
	}

}

void ATree::CutResponse()
{
	//让原有的树不再可见
	tree->SetVisibility(false);
	//设置原有碰撞消失
	tree->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//设置新树可见
	treeup->SetVisibility(true);
	treedown->SetVisibility(true);
	//设置新碰撞启用
	treeup->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	treedown->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	treeup->SetSimulatePhysics(true);

	//设置倒下效果
	FVector impulse(50.0f, 10.0f, 0.0f);
	treeup->AddImpulse(impulse, NAME_None, true);
}

void ATree::OnCutting()
{
	//让倒地的树不可见不碰撞
	treeup->SetVisibility(false);
	treeup->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ATree::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	// 进行动态属性修改、组件初始化等操作
	
}


