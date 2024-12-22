// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossCharacter.generated.h"

class UBehaviorTree;
class UAnimMontage;
/**
*
*/
UCLASS()
class COMBETPROJECTS_CPP_API ABossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float health = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthValue")
	float maxHealth = 100.0f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> HitMontage;

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void MyDelay(float delayTime, FName functionName);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	bool isAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TArray<TObjectPtr<UAnimMontage>> AttackAnimList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> BigAttackAnim;

	int32 attackNum = 0;

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	virtual void AttackDamage();

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	virtual void TakeHitDamage(const float damageValue);

	UFUNCTION()
	void PlayAttack();
	//角色的伤害相应
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	virtual void HitResponse();

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	virtual void DeathResponse();

	FORCEINLINE float GetHealthProgress() { return health / maxHealth; };
};
