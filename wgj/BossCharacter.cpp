// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "DebugHelper.h"
#include "BossAIController.h"
#include "Animation/AnimMontage.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	health = maxHealth;
}

void ABossCharacter::MyDelay(float delayTime, FName functionName)
{
	FLatentActionInfo ActionInfo;
	ActionInfo.Linkage = 1;
	ActionInfo.UUID = 100;
	ActionInfo.CallbackTarget = this;
	ActionInfo.ExecutionFunction = functionName;

	UKismetSystemLibrary::Delay(GetWorld(), delayTime, ActionInfo);
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABossCharacter::AttackDamage()
{

}

void ABossCharacter::TakeHitDamage(const float damageValue)
{
	if (health > 0)
	{
		health -= damageValue;
		if (health <= 0)
		{
			health = 0;
			DeathResponse();//执行死亡相应
		}
		else
			HitResponse();//执行伤害相应
	}
}

void ABossCharacter::PlayAttack()
{
	attackNum++;
	
	isAttacking = true;

	int32 rIndex = FMath::RandRange(0, AttackAnimList.Num() - 1);
	if(attackNum<4)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(AttackAnimList[rIndex]);
	}
	else
	{
		GetMesh()->GetAnimInstance()->Montage_Play(BigAttackAnim);
		attackNum = 0;
	}

}

void ABossCharacter::HitResponse()
{
	GetMesh()->GetAnimInstance()->Montage_Play(HitMontage);
}

void ABossCharacter::DeathResponse()
{

}

