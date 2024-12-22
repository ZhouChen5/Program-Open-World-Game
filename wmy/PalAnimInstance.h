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
	UPROPERTY(BlueprintReadOnly, Category = "AnimValue")         //�����ٿ���³
	class ABasePalCharacter* ownPalCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "AnimValue")         //�����ٶ�ֵ
	float speed;

	UPROPERTY(BlueprintReadOnly, Category = "AnimValue")         //��������ֵ
	float direction;

	UPROPERTY(BlueprintReadOnly, Category = "AnimValue")         //�����Ƿ񸡿�
	bool isFalling;

	UPROPERTY(BlueprintReadOnly, Category = "AnimValue")         //�����Ƿ�����
	bool isDeath;


public://�ж��Ƿ񹥻�����
	UFUNCTION(BlueprintCallable, Category = "AnimNotify")
	void AnimNotify_AttackEnd(UAnimNotify* Notify);
	//�ж��Ƿ��յ�����
	UFUNCTION(BlueprintCallable, Category = "AnimNotify")
	void AnimNotify_AttackDamage(UAnimNotify* Notify);
	//�ж��Ƿ���Ϣ����
	UFUNCTION(BlueprintCallable, Category = "AnimNotify")
	void AnimNotify_RestEnd(UAnimNotify* Notify);


};
