// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChopInterface.h"
#include "Components/BoxComponent.h"
#include "Math/UnrealMathUtility.h"
#include "NiagaraSystem.h"
#include "Components/SceneComponent.h"
#include "NiagaraComponent.h"
#include "Tree.generated.h"

class APlayerBaseCharacter;

UCLASS()
class PALUWORLD_API ATree : public AActor,public IChopInterface
{
	GENERATED_BODY()
public:
	ATree();
protected:
	virtual void BeginPlay() override;

	//当前血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health")
	float health = 200;


	//最大血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health")
	float maxhealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health")
	float getdamage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health")
	float trunkhealth = 45;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "cut")
	bool hasbeencut = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "cut")
	bool hasfallen = false;

public:
	virtual void Tick(float DealtaTime) override;

	FTimerHandle TimerHandle;

/// <summary>
///   
///    virtual void SetupPlayerInputComponent(cluss UInputComponent* PlayerInputComponent) override;
/// 
/// </summary>
/// 




	UFUNCTION(BlueprintCallable, Category = "function")
	virtual void AttackDamage();


	//受到伤害后的扣血处理
	UFUNCTION(BlueprintCallable,Category = "function")
	virtual void TakeHitDamage(const float damageValue);

	///伤害响应
	UFUNCTION(BlueprintCallable, Category = "function")
	virtual void HitResponse();

	//死亡响应
	UFUNCTION(BlueprintCallable, Category = "function")
	virtual void DeathResponse();

	FORCEINLINE float GetHealthProgress() { return health / maxhealth; };

	//倒地响应
	UFUNCTION(BlueprintCallable, Category = "function")
	virtual void CutResponse();

	//倒地响应
	UFUNCTION(BlueprintCallable, Category = "function")
	virtual void OnCutting();

	//与接口联系
	virtual void Chop_Implementation(float damage, FVector hitlocation) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	bool isAttacking = false;


	// 让蓝图中的 StaticMeshComponent 变得可访问
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* tree;  // 你在蓝图中创建的 StaticMeshComponent

	// Niagara 特效组件
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* NiagaraEffectComponent;

	// 材质实例
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInstanceDynamic* TrunkMaterial; // 定义材质实例动态变量


	// Box 盒体碰撞组件
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxCollisionComponent;

	// 树根实例
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* treedown;  // 你在蓝图中创建的 StaticMeshComponent

	// 树干实例
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* treeup;  // 你在蓝图中创建的 StaticMeshComponent


};