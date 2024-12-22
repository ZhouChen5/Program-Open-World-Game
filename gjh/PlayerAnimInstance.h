// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"PlayerBaseCharacter.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PALUWORLD_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float deltatime) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "AnimValue")
	class APlayerBaseCharacter* ownercharacter;

	UPROPERTY(BlueprintReadWrite, Category = "AnimValue")
	float speed;

	UPROPERTY(BlueprintReadOnly, Category = "AnimValue")
	bool isfalling;

public:
	UPROPERTY(BlueprintReadOnly, Category = "AnimValue");
	bool isSprinting;
	UPROPERTY(BlueprintReadOnly, Category = "AnimValue")
	float direction;
	UPROPERTY(BlueprintReadOnly, Category = "AnimValue");
	int weapon;
	//投掷的动画通知
	UFUNCTION(BlueprintCallable, Category = "AnimFunc")
	void AnimNotify_throwend(UAnimNotify* Notify);
	UFUNCTION(BlueprintCallable, Category = "AnimFunc")
	void AnimNotify_throwstart(UAnimNotify* Notify);
	UFUNCTION(BlueprintCallable, Category = "AnimFunc")
	void AnimNotify_offhand(UAnimNotify* Notify);
	//空手攻击的动画通知
	UFUNCTION(BlueprintCallable, Category = "AnimFunc")
	void AnimNotify_noneattack1(UAnimNotify* Notify);
	UFUNCTION(BlueprintCallable, Category = "AnimFunc")
	void AnimNotify_noneattack2(UAnimNotify* Notify);
	UFUNCTION(BlueprintCallable, Category = "AnimFunc")
	void AnimNotify_melee_attack1(UAnimNotify* Notify);
	UFUNCTION(BlueprintCallable, Category = "AnimFunc")
	void AnimNotify_melee_attack2(UAnimNotify* Notify);
};
