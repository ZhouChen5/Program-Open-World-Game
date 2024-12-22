// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "SoldierBaseWidget.h"
#include "Soldier1Character.generated.h"

class USoldierBaseWidget;
class UWidgetComponent;
class APlayerBaseCharacter;
class UAnimMontage;
/**
 * 
 */
UCLASS()
class PALUWORLD_API ASoldier1Character : public ABaseCharacter
{
	GENERATED_BODY()
public:
	ASoldier1Character();
	virtual void BeginPlay() override;
protected:
	UPROPERTY(BlueprintReadWrite, Category = "MyVariable")
	bool isDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TArray<TObjectPtr<UAnimMontage>> AttackAnimList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> BigAttackAnim;

	int32 attackNum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIWidget")
	TSubclassOf<USoldierBaseWidget> HealthBarWidgetClass;

	//每个敌人的身高不一样，血条位置显示也应该不同
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIWidget")
	float HealthBarHeight = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WidgetComp")
	UWidgetComponent* HealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HealthValue")
	bool isboss=false;

	UFUNCTION()
	void DestroyEnemy();

	virtual void DeathResponse() override;
	virtual void HitResponse() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	APlayerBaseCharacter* GetAttackRangePlayer(const TArray<FHitResult> hitList);

	virtual void AttackDamage() override;

	virtual void PlayAttack() override;
};
