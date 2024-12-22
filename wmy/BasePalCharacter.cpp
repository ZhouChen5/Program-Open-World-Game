// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePalCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PalAIController.h"
#include "DebugHelper.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "PlayerBaseCharacter.h"
#include "CombatGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "BaseCharacter.h"



// Sets default values
ABasePalCharacter::ABasePalCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//控制转向的操作设置为false
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	//添加控制组件到根组件
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABasePalCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//有关初始值的设定
	Health = MaxHealth;

	//初始属性设置
	ownerProperty = PersonProperty::NONE;

	//创建控件
	UHealthBarWidget* HealthBarWidget = CreateWidget<UHealthBarWidget>(GetWorld(), HealthBarWidgetClass);
	HealthBar->SetWidget(HealthBarWidget); //将血条赋值给控制组件

	//将血条的挂载对象设置给自己
	HealthBarWidget->ownerCharacter = this;    
}

// Called every frame
void ABasePalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasePalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ABasePalCharacter::MyDelay(float delayTime, FName functionName)
{
	FLatentActionInfo ActionInfo;
	ActionInfo.Linkage = 1;
	ActionInfo.UUID = 100;
	ActionInfo.CallbackTarget = this;
	ActionInfo.ExecutionFunction = functionName;

	UKismetSystemLibrary::Delay(GetWorld(), delayTime, ActionInfo);
}

//自己函数的实现
void ABasePalCharacter::PlayAttack()
{
	IsAttacking = true;
	//播放攻击动画
	GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
}

//实现休息动作的播放
void ABasePalCharacter::PlayRest0()
{
	if (Rest0Montage) {
		//播放休息动画
		Debug::Print("Rest0");
		GetMesh()->GetAnimInstance()->Montage_Play(Rest0Montage);
	}
}
void ABasePalCharacter::PlayRest1()
{
	if (Rest1Montage) {
		//播放休息动画
		Debug::Print("Rest1");
		GetMesh()->GetAnimInstance()->Montage_Play(Rest1Montage);
	}
}

void ABasePalCharacter::PlayEncounter()
{
	if (EncounterMontage) {
		//播放偶遇动画
		GetMesh()->GetAnimInstance()->Montage_Play(EncounterMontage);
	}
}

//帕鲁的攻击造成伤害
void ABasePalCharacter::AttackDamage()
{
	FVector StartPos = GetActorLocation();
	FVector EndPos = GetActorForwardVector() * 200 + StartPos;

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);

	TArray<AActor*> ignoreActors;
	TArray<FHitResult> hitList;
	bool isResult = UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), StartPos, EndPos, AttackRange, FName(""), true, ignoreActors, EDrawDebugTrace::ForDuration, hitList, true);
	if (isResult) {
		for (FHitResult Hit : hitList) {
			ABasePalCharacter* pal = Cast<ABasePalCharacter>(Hit.GetActor());
			if (pal && pal != this) {
				//通过阵营判断是否为敌人
				Faction enemyFaction = pal->GetFaction();
				bool isEnemy = IsEnemy(faction, enemyFaction);
				if (isEnemy) {
					pal->TakeHitDamage(AttackValue);
				}
			}
			else {
				APlayerBaseCharacter* pal0 = Cast<APlayerBaseCharacter>(Hit.GetActor());
				if (pal) {
					Faction enemyFaction = pal->GetFaction();
					bool isEnemy = IsEnemy(faction, enemyFaction);
					if (isEnemy) {
						pal0->DiaoXie(AttackValue);
					}
				}
			}
		}
	}
}
//帕鲁的受伤减血
void ABasePalCharacter::TakeHitDamage(const float DamageValue)
{
	Health -= DamageValue;
	if (Health <= 0) {
		Health = 0;
		//血量清零调用死亡响应函数
		DeathResponse();
	}
	else {
		//实现受伤响应
		HitResponse();
	}
}

//帕鲁的伤害响应
void ABasePalCharacter::HitResponse()
{
	//播放受伤的蒙太奇动画
	GetMesh()->GetAnimInstance()->Montage_Play(HitMontage);
	if (!IsDamage) {
		//攻击调试用
		IsDamage = true;

		APalAIController* controller = Cast<APalAIController>(GetController());

		if (controller) {
			controller->SetAIDamage();
		}
	}
}
//帕鲁的死亡响应
void ABasePalCharacter::DeathResponse()
{
	//获取AI组件
	AAIController* controller = Cast<AAIController>(GetController());
	//检测是否获取组件
	if (controller) {
		//停止一切逻辑
		controller->BrainComponent->StopLogic("");
		//清除运动状态
		controller->ClearFocus(EAIFocusPriority::Default);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		//去除碰撞体
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//去除碰撞体的干扰
		SetActorEnableCollision(false);
		//防止进一步交互
		SetActorTickEnabled(false);
	}
	//播放死亡动画
	GetMesh()->GetAnimInstance()->Montage_Play(DeathMontage);
	//销毁血条组件
	HealthBar->DestroyComponent();

	ACombatGameMode* gameMode = Cast<ACombatGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->RemovePal(this);

	MyDelay(3.f, FName("DestroyPal"));
}
void ABasePalCharacter::DestroyPal()
{
	Destroy();
}
//获取当前自己的属性组件
PersonProperty ABasePalCharacter::GetPersonalProperty()
{
	return ownerProperty;
}

APlayerBaseCharacter* ABasePalCharacter::GetAttackRangePlayer(const TArray<FHitResult> hit)
{
	APlayerBaseCharacter* player;
	//遍历识别到的Actor检验是不是玩家
	for (FHitResult Hit : hit) {
		player = Cast<APlayerBaseCharacter>(Hit.GetActor());
		if (player) {
			return player;
		}
	}

	return nullptr;
}
Faction ABasePalCharacter::GetFaction()
{
	return faction;
}
