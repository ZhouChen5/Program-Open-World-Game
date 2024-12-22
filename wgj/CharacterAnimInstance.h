// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PALUWORLD_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float deltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly,Category="AnimValue")
	class ABaseCharacter* ownerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "AnimValue")
	float speed;

	UPROPERTY(BlueprintReadOnly, Category = "AnimValue")
	float direction;

	UPROPERTY(BlueprintReadOnly, Category = "AnimValue")
	bool isFalling;

	UPROPERTY(BlueprintReadOnly, Category = "AnimValue")
	bool isDeath;

	UFUNCTION(BlueprintCallable,Category="AnimNotify")
	void AnimNotify_AttackEnd(UAnimNotify* Notify);

	//UFUNCTION(BlueprintCallable, Category = "AnimNotify")
	//void AnimNotify_BossAttackEnd(UAnimNotify* Notify);

	UFUNCTION(BlueprintCallable, Category = "AnimNotify")
	void AnimNotify_AttackDamage(UAnimNotify* Notify);
};
