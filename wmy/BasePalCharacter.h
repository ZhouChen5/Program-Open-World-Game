// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Factions.h"
#include "PersonalProperty.h"
#include "BasePalCharacter.generated.h"

class UHealthBarWidget;
class UWidgetComponent;
class APlayerCharacter;


UCLASS()
class PALUWORLD_API ABasePalCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	//公共计数器
	int count = 0;


//基础功能
public:
	//构造函数
	ABasePalCharacter();

	//逐帧更新
	virtual void Tick(float DeltaTime) override;

	//控制移动
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	//起始
	virtual void BeginPlay() override;





	//与行为控制内部逻辑有关的控制变量
protected:
	//是否收到攻击
	UPROPERTY(BlueprintReadWrite, Category = "MyVariable")
	bool IsDamage = false;
	//攻击动画的蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	UAnimMontage* AttackMontage;
	//受伤动画的蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> HitMontage;
	//死亡动画的蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> DeathMontage;
	//休息动画的蒙太奇
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> Rest0Montage;
	//休息动画的蒙太奇
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> Rest1Montage;
	//受惊动画的蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> EncounterMontage;

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void MyDelay(float delayTime, FName functionName);

public:
	//行为树有关行为
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;
	//是否处于攻击状态
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	bool IsAttacking = false;
	//是否处于休息状态
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	bool IsResting = false;
	//是否处于死亡状态
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	bool IsDeath = false;

	//实现攻击操作
	UFUNCTION()
	void PlayAttack();
	//实现休息操作
	UFUNCTION()
	void PlayRest0();
	UFUNCTION()
	void PlayRest1();
	//实现偶遇操作
	UFUNCTION()
	void PlayEncounter();

	//实现攻击伤害操作
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void AttackDamage();
	//受伤扣减血量
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void TakeHitDamage(const float DamageValue);

	//添加帕鲁伤害响应
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void HitResponse();
	//添加帕鲁死亡响应
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void DeathResponse();

	UFUNCTION()
	void DestroyPal();








//一些基础属性的实现
private:

protected:

	//与血量有关的属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthValue")
	float MaxHealth = 100.0f;                   //血量上限
	float Health = 100.0f;                      //实际血量

	//阵营标签
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Faction");
	Faction faction = Faction::ENEMY;
	//属性标签
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Property");
	PersonProperty ownerProperty;

public:
	//获取当前生命值
	FORCEINLINE float GetHealthProgress() { return Health / MaxHealth; }
	//获取生命值挂载组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIWidget")
	TSubclassOf<UHealthBarWidget> HealthBarWidgetClass;
	//获取可视化组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WidgetComp")
	UWidgetComponent* HealthBar;
	//获取帕鲁的阵营
	UFUNCTION()
	Faction GetFaction();
	//获取当前帕鲁的属性
	UFUNCTION()
	PersonProperty GetPersonalProperty();
	//获取玩家组件
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	APlayerBaseCharacter* GetAttackRangePlayer(const TArray<FHitResult> hit);
	//攻击范围检测组件
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttackRange")
	float AttackRange;
	//攻击伤害
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttackRange")
	float AttackValue;


};
