// Fill out your copyright notice in the Description page of Project Settings.


#include "PalAnimInstance.h"
#include "BasePalCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

void UPalAnimInstance::NativeInitializeAnimation()
{
	ownPalCharacter = Cast<ABasePalCharacter>(TryGetPawnOwner());    //获取己方对象的信息
}

void UPalAnimInstance::NativeUpdateAnimation(float deltaTime)
{
	if (ownPalCharacter) {

		//获取当前帕鲁速度信息
		FVector velocity = ownPalCharacter->GetVelocity();
		speed = velocity.Length();

		//获取当前帕鲁移动方向信息
		FRotator rotation = ownPalCharacter->GetActorRotation();
		direction = UKismetAnimationLibrary::CalculateDirection(velocity, rotation);

		//判断帕鲁是否正在掉落
		UCharacterMovementComponent* movement = ownPalCharacter->GetCharacterMovement();
		isFalling = movement->IsFalling();

		//检测是否死亡
		if (ownPalCharacter->GetHealthProgress() <= 0) {
			isDeath = true;
		}
	}
}

//寻找动画结束的点
void UPalAnimInstance::AnimNotify_AttackEnd(UAnimNotify* Notify)
{
	ABasePalCharacter* PalCharacter = Cast<ABasePalCharacter>(ownPalCharacter);
	//如果指针有效
	if (PalCharacter) {
		PalCharacter->IsAttacking = false;
	}
}

//寻找动画结束的点
void UPalAnimInstance::AnimNotify_RestEnd(UAnimNotify* Notify)
{
	ABasePalCharacter* PalCharacter = Cast<ABasePalCharacter>(ownPalCharacter);
	//如果指针有效
	if (PalCharacter) {
		PalCharacter->IsResting = false;
	}
}


void UPalAnimInstance::AnimNotify_AttackDamage(UAnimNotify* Notify)
{
	//如果指针有效
	if (ownPalCharacter) {
		ownPalCharacter->AttackDamage();
	}
}
