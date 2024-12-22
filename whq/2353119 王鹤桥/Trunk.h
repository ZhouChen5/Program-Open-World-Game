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
#include "Trunk.generated.h"

UCLASS()
class PALUWORLD_API ATrunk : public AActor,public IChopInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//��ǰѪ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health")
	float health = 100;


	//���Ѫ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health")
	float maxhealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health")
	float getdamage = 10;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, Category = "function")
	virtual void AttackDamage();


	//�ܵ��˺���Ŀ�Ѫ����
	UFUNCTION(BlueprintCallable, Category = "function")
	virtual void TakeHitDamage(const float damageValue);

	///�˺���Ӧ
	UFUNCTION(BlueprintCallable, Category = "function")
	virtual void HitResponse();

	//������Ӧ
	UFUNCTION(BlueprintCallable, Category = "function")
	virtual void DeathResponse();

	FORCEINLINE float GetHealthProgress() { return health / maxhealth; };


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
};
