// Fill out your copyright notice in the Description page of Project Settings.


#include "Soldier1Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "SoldierBaseWidget.h"
#include "Components/CapsuleComponent.h"
#include "PlayerBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "CombatGameMode.h"
#include "AIController.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"

ASoldier1Character::ASoldier1Character()
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

void ASoldier1Character::BeginPlay()
{
	Super::BeginPlay();


	//创建控件，并将控件赋值给控件逻辑
	USoldierBaseWidget* healthBarWidget = CreateWidget<USoldierBaseWidget>(GetWorld(), HealthBarWidgetClass);

	HealthBar->SetWidget(healthBarWidget);//将血条赋值给控件组件

	healthBarWidget->ownerCharacter = this;//设置ownerCharacter

}

void ASoldier1Character::DestroyEnemy()
{
	Destroy();

}

void ASoldier1Character::DeathResponse()
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
		gameMode->RemoveSoldier(this);

		MyDelay(3.0f, FName("DestroyEnemy"));
	}
}

void ASoldier1Character::HitResponse()
{
	Super::HitResponse();

	if (!isDamage)
	{
		isDamage = true;

		AEnemyAIController* controller = Cast<AEnemyAIController>(GetController());

		if (controller)
		{
			controller->SetAIDamage();
		}
	}

}

void ASoldier1Character::PlayAttack()
{
	attackNum++;

	isAttacking = true;

	int32 rIndex = FMath::RandRange(0, AttackAnimList.Num() - 1);
	if (attackNum < 4)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(AttackAnimList[rIndex]);
	}
	else
	{
		GetMesh()->GetAnimInstance()->Montage_Play(BigAttackAnim);
		attackNum = 0;
	}
}

APlayerBaseCharacter* ASoldier1Character::GetAttackRangePlayer(const TArray<FHitResult> hitList)
{
	APlayerBaseCharacter* player = nullptr;

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

void ASoldier1Character::AttackDamage()
{
	FVector start = GetActorLocation();
	FVector end = start + GetActorForwardVector() * 200;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);

	TArray<AActor*> ignoreList;

	TArray<FHitResult> hitList;
	bool isResult = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), start, end, 50.0f, ObjectTypes, true,
		ignoreList, EDrawDebugTrace::ForDuration, hitList, true);

	if (isResult)
	{
		APlayerBaseCharacter* player = GetAttackRangePlayer(hitList);
		if (player)
		{
			player->DiaoXie(50.0f);
		}
	}
}
