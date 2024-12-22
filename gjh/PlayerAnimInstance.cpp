// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerAnimInstance.h"
#include "DebugHelper.h"
#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BasePalCharacter.h"
#include "MeleeWeapon.h"
#include "ThrowWeapon.h"
#include "Kismet/KismetSystemLibrary.h"
#include <string>

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	ownercharacter = Cast<APlayerBaseCharacter>(TryGetPawnOwner());
	ownercharacter->CurrentMovementSpeed;
}

void UPlayerAnimInstance::NativeUpdateAnimation(float deltatime)
{
	if (ownercharacter) {
		//��ȡ�ٶ�
		FVector velocity = ownercharacter->GetVelocity();
		speed = velocity.Length();

		//Debug::print("getspeed");
		int speedd = speed;
		/*for (int i = 0; i < speed; i++) {
			Debug::print("i");
		}*/
		//��ȡ�ƶ�����
		FRotator rotation = ownercharacter->GetActorRotation();
		if (!velocity.Z)
			direction = CalculateDirection(velocity, rotation);
		else {
			if (velocity.X)
				direction = CalculateDirection(velocity, rotation);
			else {
				if (velocity.Z > 0)
					direction = 0;
				else {
					direction = -180;
				}
			}
			/*direction = FMath::Atan(velocity.X/velocity.Z);
			direction = direction * 180 / PI;*/
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::FromInt(velocity.Z));
			/*Debug::print("ppp");
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::FromInt(velocity.Z));
			Debug::print("xxx");
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::FromInt(velocity.X));*/

		}
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::FromInt(direction * 10));
		UCharacterMovementComponent* movement = ownercharacter->GetCharacterMovement();
		isfalling = movement->IsFalling();

		//��ȡ��ǰ����������
		weapon = ownercharacter->GetWeaponClass();

		isSprinting = 0;
	}

}

void UPlayerAnimInstance::AnimNotify_throwend(UAnimNotify* Notify)
{
	Debug::Print("throwend");

	ownercharacter->setThrowEnd();

}
void UPlayerAnimInstance::AnimNotify_throwstart(UAnimNotify* Notify)
{
	Debug::Print("throwstart");
	ownercharacter->setThrowStart();
}

void UPlayerAnimInstance::AnimNotify_offhand(UAnimNotify* Notify)
{

	ownercharacter->setOffHand();

}

void UPlayerAnimInstance::AnimNotify_noneattack1(UAnimNotify* Notify)
{
	auto world = GetWorld();
	if (world) {
		//������ʼλ��
		auto startpos = ownercharacter->GetActorLocation();
		//���߷���
		auto hitdirection = ownercharacter->GetActorForwardVector();
		//��ֹλ��
		auto endpos = startpos + hitdirection * 200;
		//Ҫ���ԵĶ��󣨶������壩
		TArray<AActor*> ignoreactor;
		ignoreactor.Add(ownercharacter);
		//���ܽ��������
		TArray<FHitResult> Result;
		bool isResult = UKismetSystemLibrary::SphereTraceMultiByProfile(world,
			startpos,
			endpos,
			20,
			FName(""),
			true,
			ignoreactor,
			EDrawDebugTrace::ForDuration,
			Result,
			true);
		if (isResult) {
			for (FHitResult result : Result) {
				auto hit = result.GetActor();
				if (hit->IsA(ABasePalCharacter::StaticClass())) {
					Debug::Print("hit a Palu");
					auto hitpalu = Cast<ABasePalCharacter>(hit);
					if (hitpalu) {
						hitpalu->TakeHitDamage(10);
					}
				}
				if (hit->IsA(ABaseCharacter::StaticClass())) {
					auto hitpalu = Cast<ABaseCharacter>(hit);
					if (hitpalu) {
						hitpalu->TakeHitDamage(10);
					}
				}
			}
		}
	}
}

void UPlayerAnimInstance::AnimNotify_noneattack2(UAnimNotify* Notify)
{
	auto world = GetWorld();
	if (world) {
		//������ʼλ��
		auto startpos = ownercharacter->GetActorLocation();
		//���߷���
		auto hitdirection = ownercharacter->GetActorForwardVector();
		//��ֹλ��
		auto endpos = startpos + hitdirection * 200;
		//Ҫ���ԵĶ��󣨶������壩
		TArray<AActor*> ignoreactor;
		ignoreactor.Add(ownercharacter);
		//���ܽ��������
		TArray<FHitResult> Result;
		bool isResult = UKismetSystemLibrary::SphereTraceMultiByProfile(world,
			startpos,
			endpos,
			20,
			FName(""),
			true,
			ignoreactor,
			EDrawDebugTrace::ForDuration,
			Result,
			true);
		if (isResult) {
			for (FHitResult result : Result) {
				auto hit = result.GetActor();
				if (hit->IsA(ABasePalCharacter::StaticClass())) {
					Debug::Print("hit a Palu");
					auto hitpalu = Cast<ABasePalCharacter>(hit);
					if (hitpalu) {
						hitpalu->TakeHitDamage(10);
					}
				}
				if (hit->IsA(ABaseCharacter::StaticClass())) {
					auto hitpalu = Cast<ABaseCharacter>(hit);
					if (hitpalu) {
						hitpalu->TakeHitDamage(10);
					}
				}
			}
		}
	}
}

void UPlayerAnimInstance::AnimNotify_melee_attack1(UAnimNotify* Notify)
{
	auto world = GetWorld();
	if (world) {
		//������ʼλ��
		auto startpos = ownercharacter->GetActorLocation();
		//���߷���
		auto hitdirection = ownercharacter->GetActorForwardVector();
		//��ֹλ��
		auto endpos = startpos + hitdirection * 200;
		//Ҫ���ԵĶ��󣨶������壩
		TArray<AActor*> ignoreactor;
		ignoreactor.Add(ownercharacter);
		//���ܽ��������
		TArray<FHitResult> Result;
		bool isResult = UKismetSystemLibrary::SphereTraceMultiByProfile(world,
			startpos,
			endpos,
			20,
			FName(""),
			true,
			ignoreactor,
			EDrawDebugTrace::ForDuration,
			Result,
			true);
		if (isResult) {
			for (FHitResult result : Result) {
				auto hit = result.GetActor();
				auto meleeweapon = Cast<AMeleeWeapon>(ownercharacter->weapon);
				if (hit->IsA(ABasePalCharacter::StaticClass())) {
					Debug::Print("hit a Palu");
					auto hitpalu = Cast<ABasePalCharacter>(hit);
					if (hitpalu) {
						if (!hitpalu->IsDeath)
							if(meleeweapon)
							hitpalu->TakeHitDamage(meleeweapon->hurtvalue);
					}
				}
				if (hit->IsA(ABaseCharacter::StaticClass())) {
					auto hitpalu = Cast<ABaseCharacter>(hit);
					if (hitpalu) {
						if (meleeweapon) {
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("ABaseWeapon Pointer Address: %d"), meleeweapon->hurtvalue));

							hitpalu->TakeHitDamage(meleeweapon->hurtvalue);
						}
					}
				}
			}
		}
	}
}

void UPlayerAnimInstance::AnimNotify_melee_attack2(UAnimNotify* Notify)
{
	auto world = GetWorld();
	if (world) {
		//������ʼλ��
		auto startpos = ownercharacter->GetActorLocation();
		//���߷���
		auto hitdirection = ownercharacter->GetActorForwardVector();
		//��ֹλ��
		auto endpos = startpos + hitdirection * 200;
		//Ҫ���ԵĶ��󣨶������壩
		TArray<AActor*> ignoreactor;
		ignoreactor.Add(ownercharacter);
		//���ܽ��������
		TArray<FHitResult> Result;
		bool isResult = UKismetSystemLibrary::SphereTraceMultiByProfile(world,
			startpos,
			endpos,
			20,
			FName(""),
			true,
			ignoreactor,
			EDrawDebugTrace::ForDuration,
			Result,
			true);
		if (isResult) {
			for (FHitResult result : Result) {
				auto hit = result.GetActor();
				auto meleeweapon = Cast<AMeleeWeapon>(ownercharacter->weapon);
				if (hit->IsA(ABasePalCharacter::StaticClass())) {
					
					Debug::Print("hit a Palu");
					auto hitpalu = Cast<ABasePalCharacter>(hit);
					if (hitpalu) {
						
						if (meleeweapon) {
							meleeweapon->hurtvalue;
							hitpalu->TakeHitDamage(meleeweapon->hurtvalue);
						}
					}			
				}
				if (hit->IsA(ABaseCharacter::StaticClass())) {
					auto hitpalu = Cast<ABaseCharacter>(hit);
					if (hitpalu) {
						if(meleeweapon)
						hitpalu->TakeHitDamage(meleeweapon->hurtvalue);
					}
				}
			}
		}
	}
}

