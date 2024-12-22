// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyAIController.h"
#include "DebugHelper.h"
//血条所需控件组件
#include "Components/WidgetComponent.h"
#include "SoldierBaseWidget.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "CombatGameMode.h"

AEnemyBaseCharacter::AEnemyBaseCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	//添加血条控件组件
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(RootComponent);

	// 调整血条高度，Z 轴通常表示高度
	HealthBar->SetRelativeLocation(FVector(0.f, 0.f, HealthBarHeight)); // Z = 100 是示例值
}

void AEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();


	//创建控件，并将控件赋值给控件逻辑
	USoldierBaseWidget* healthBarWidget = CreateWidget<USoldierBaseWidget>(GetWorld(),HealthBarWidgetClass);

	HealthBar->SetWidget(healthBarWidget);//将血条赋值给控件组件

	healthBarWidget->ownerCharacter = this;//设置ownerCharacter
}

void AEnemyBaseCharacter::DeathResponse()
{
	HealthBar->DestroyComponent();
	
	AAIController* controller = Cast<AAIController>(GetController());

	if (controller)
	{
		controller->BrainComponent->StopLogic("");
		controller->ClearFocus(EAIFocusPriority::Default);

		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		ACombatGameMode* gameMode = Cast<ACombatGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		gameMode->RemoveEnemy(this);

		MyDelay(0.5f,FName("DestroyEnemy"));
	}
}

void AEnemyBaseCharacter::DestroyEnemy()
{
	Destroy();
}

void AEnemyBaseCharacter::HitResponse()
{
	Super::HitResponse();

	if(!isDamage)
	{
		isDamage = true;

		AEnemyAIController* controller = Cast<AEnemyAIController>(GetController());

		if (controller)
		{
			controller->SetAIDamage();
		}
	}
}

void AEnemyBaseCharacter::PlayAttack()
{

	Debug::print("Attack Player");
	isAttacking = true;

	GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);

}


void AEnemyBaseCharacter::AttackDamage()
{
	FVector start = GetActorLocation();
	FVector end = start + GetActorForwardVector() * 200;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);

	TArray<AActor*> ignoreList;

	TArray<FHitResult> hitList;
	bool isResult = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), start, end, 50.0f,ObjectTypes,true,
		ignoreList,EDrawDebugTrace::ForOneFrame,hitList,true);

	if (isResult)
	{
		APlayerBaseCharacter* player = GetAttackRangePlayer(hitList);
		if (player)
		{
			player->DiaoXie(10.0f);
		}
	}

}

APlayerBaseCharacter* AEnemyBaseCharacter::GetAttackRangePlayer(const TArray<FHitResult> hitList)
{
	APlayerBaseCharacter* player=nullptr;

	for (FHitResult hitRes : hitList)
	{
		player = Cast<APlayerBaseCharacter>(hitRes.GetActor());
		if (player)
		{
			break;
		}
	}
	return player;
}
