// Fill out your copyright notice in the Description page of Project Settings.


#include "PalAnimInstance.h"
#include "BasePalCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

void UPalAnimInstance::NativeInitializeAnimation()
{
	ownPalCharacter = Cast<ABasePalCharacter>(TryGetPawnOwner());    //��ȡ�����������Ϣ
}

void UPalAnimInstance::NativeUpdateAnimation(float deltaTime)
{
	if (ownPalCharacter) {

		//��ȡ��ǰ��³�ٶ���Ϣ
		FVector velocity = ownPalCharacter->GetVelocity();
		speed = velocity.Length();

		//��ȡ��ǰ��³�ƶ�������Ϣ
		FRotator rotation = ownPalCharacter->GetActorRotation();
		direction = UKismetAnimationLibrary::CalculateDirection(velocity, rotation);

		//�ж���³�Ƿ����ڵ���
		UCharacterMovementComponent* movement = ownPalCharacter->GetCharacterMovement();
		isFalling = movement->IsFalling();

		//����Ƿ�����
		if (ownPalCharacter->GetHealthProgress() <= 0) {
			isDeath = true;
		}
	}
}

//Ѱ�Ҷ��������ĵ�
void UPalAnimInstance::AnimNotify_AttackEnd(UAnimNotify* Notify)
{
	ABasePalCharacter* PalCharacter = Cast<ABasePalCharacter>(ownPalCharacter);
	//���ָ����Ч
	if (PalCharacter) {
		PalCharacter->IsAttacking = false;
	}
}

//Ѱ�Ҷ��������ĵ�
void UPalAnimInstance::AnimNotify_RestEnd(UAnimNotify* Notify)
{
	ABasePalCharacter* PalCharacter = Cast<ABasePalCharacter>(ownPalCharacter);
	//���ָ����Ч
	if (PalCharacter) {
		PalCharacter->IsResting = false;
	}
}


void UPalAnimInstance::AnimNotify_AttackDamage(UAnimNotify* Notify)
{
	//���ָ����Ч
	if (ownPalCharacter) {
		ownPalCharacter->AttackDamage();
	}
}
