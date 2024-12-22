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

	//��ǰѪ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health")
	float health = 200;


	//���Ѫ��
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


	//�ܵ��˺���Ŀ�Ѫ����
	UFUNCTION(BlueprintCallable,Category = "function")
	virtual void TakeHitDamage(const float damageValue);

	///�˺���Ӧ
	UFUNCTION(BlueprintCallable, Category = "function")
	virtual void HitResponse();

	//������Ӧ
	UFUNCTION(BlueprintCallable, Category = "function")
	virtual void DeathResponse();

	FORCEINLINE float GetHealthProgress() { return health / maxhealth; };

	//������Ӧ
	UFUNCTION(BlueprintCallable, Category = "function")
	virtual void CutResponse();

	//������Ӧ
	UFUNCTION(BlueprintCallable, Category = "function")
	virtual void OnCutting();

	//��ӿ���ϵ
	virtual void Chop_Implementation(float damage, FVector hitlocation) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	bool isAttacking = false;


	// ����ͼ�е� StaticMeshComponent ��ÿɷ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* tree;  // ������ͼ�д����� StaticMeshComponent

	// Niagara ��Ч���
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* NiagaraEffectComponent;

	// ����ʵ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInstanceDynamic* TrunkMaterial; // �������ʵ����̬����


	// Box ������ײ���
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxCollisionComponent;

	// ����ʵ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* treedown;  // ������ͼ�д����� StaticMeshComponent

	// ����ʵ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* treeup;  // ������ͼ�д����� StaticMeshComponent


};