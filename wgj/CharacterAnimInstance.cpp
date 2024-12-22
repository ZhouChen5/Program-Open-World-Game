// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"//移动组件
#include "EnemyBaseCharacter.h"
#include "KismetAnimationLibrary.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	//强制转换到basecharactor
	ownerCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
}

void UCharacterAnimInstance::NativeUpdateAnimation(float deltaTime)
{
	//ownercharacter存在才能进行接下来的操作，否则会异常终止
	if (ownerCharacter)
	{
		//取现在角色的速度
		FVector velocity = ownerCharacter->GetVelocity();

		//speed是速度的模长
		speed = velocity.Length();

		//获取当前角色的旋转值
		FRotator rotation = ownerCharacter->GetActorRotation();
		
		//计算当前的方向
		direction = UKismetAnimationLibrary::CalculateDirection(velocity, rotation);

		//侦测玩家移动组件
		UCharacterMovementComponent* movement = ownerCharacter->GetCharacterMovement();
		isFalling = movement->IsFalling();

		if (ownerCharacter->GetHealthProgress() <= 0)
			isDeath = true;
	}

}

//敌人攻击结束
void UCharacterAnimInstance::AnimNotify_AttackEnd(UAnimNotify* Notify)
{
	ABaseCharacter* enemyCharacter = Cast<ABaseCharacter>(ownerCharacter);

	if (enemyCharacter)
	{
		enemyCharacter->isAttacking = false;
	}
}

//void UCharacterAnimInstance::AnimNotify_BossAttackEnd(UAnimNotify* Notify)
//{
//	ABossCharacter* bossCharacter = Cast<ABossCharacter>(ownerCharacter);
//
//	if (bossCharacter)
//	{
//		bossCharacter->isAttacking = false;
//	}
//}

void UCharacterAnimInstance::AnimNotify_AttackDamage(UAnimNotify* Notify)
{
	if (ownerCharacter)
	{
		ownerCharacter->AttackDamage();
	}
}

