// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"//�ƶ����
#include "EnemyBaseCharacter.h"
#include "KismetAnimationLibrary.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	//ǿ��ת����basecharactor
	ownerCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
}

void UCharacterAnimInstance::NativeUpdateAnimation(float deltaTime)
{
	//ownercharacter���ڲ��ܽ��н������Ĳ�����������쳣��ֹ
	if (ownerCharacter)
	{
		//ȡ���ڽ�ɫ���ٶ�
		FVector velocity = ownerCharacter->GetVelocity();

		//speed���ٶȵ�ģ��
		speed = velocity.Length();

		//��ȡ��ǰ��ɫ����תֵ
		FRotator rotation = ownerCharacter->GetActorRotation();
		
		//���㵱ǰ�ķ���
		direction = UKismetAnimationLibrary::CalculateDirection(velocity, rotation);

		//�������ƶ����
		UCharacterMovementComponent* movement = ownerCharacter->GetCharacterMovement();
		isFalling = movement->IsFalling();

		if (ownerCharacter->GetHealthProgress() <= 0)
			isDeath = true;
	}

}

//���˹�������
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

