// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnemyBaseCharacter.generated.h"

class USoldierBaseWidget;
class UWidgetComponent;
class APlayerBaseCharacter;
/**
 * 
 */

UCLASS()
class PALUWORLD_API AEnemyBaseCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AEnemyBaseCharacter();
	virtual void BeginPlay() override;
protected:
	UPROPERTY(BlueprintReadWrite,Category="MyVariable")
	bool isDamage = false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Anim")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UIWidget")
	TSubclassOf<USoldierBaseWidget> HealthBarWidgetClass;

	//每个敌人的身高不一样，血条位置显示也应该不同
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIWidget")
	float HealthBarHeight=100.0f;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="WidgetComp")
	UWidgetComponent* HealthBar;

	UFUNCTION()
	void DestroyEnemy();

	virtual void DeathResponse() override;
	virtual void HitResponse() override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="AI")
	class UBehaviorTree* BehaviorTree;

	UFUNCTION(BlueprintCallable,Category="MyFunction")
	APlayerBaseCharacter* GetAttackRangePlayer(const TArray<FHitResult> hitList);

	virtual void AttackDamage() override;
	virtual void PlayAttack() override;
};
