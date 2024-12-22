// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PalAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PALUWORLD_API UPalAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float deltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "AnimValue")         //声明操控帕鲁
	class ABasePalCharacter* ownPalCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "AnimValue")         //声明速度值
	float speed;

	UPROPERTY(BlueprintReadOnly, Category = "AnimValue")         //声明方向值
	float direction;

	UPROPERTY(BlueprintReadOnly, Category = "AnimValue")         //声明是否浮空
	bool isFalling;

	UPROPERTY(BlueprintReadOnly, Category = "AnimValue")         //声明是否死亡
	bool isDeath;


public://判断是否攻击结束
	UFUNCTION(BlueprintCallable, Category = "AnimNotify")
	void AnimNotify_AttackEnd(UAnimNotify* Notify);
	//判断是否收到攻击
	UFUNCTION(BlueprintCallable, Category = "AnimNotify")
	void AnimNotify_AttackDamage(UAnimNotify* Notify);
	//判断是否休息结束
	UFUNCTION(BlueprintCallable, Category = "AnimNotify")
	void AnimNotify_RestEnd(UAnimNotify* Notify);


};
