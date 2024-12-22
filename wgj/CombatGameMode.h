// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CombatGameMode.generated.h"

/**
 * 
 */
class UUserWidget;
class AEnemyBaseCharacter;
class ASoldier1Character;
class ABasePalCharacter;
class AAudioActor;
UCLASS()
class PALUWORLD_API ACombatGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ACombatGameMode();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="UI")
	TSubclassOf<UUserWidget> MainUIClass;

	UPROPERTY(BlueprintReadOnly,Category="CreatePoint")
	TArray<AActor*> CreatePointList;


	UPROPERTY(EditAnywhere,Category="EnemyClass")
	TArray<TSubclassOf<AEnemyBaseCharacter>> EnemyClassList;

	UPROPERTY(EditAnywhere, Category = "EnemyClass")
	TArray<AEnemyBaseCharacter*> EnemyList;

	UPROPERTY(BlueprintReadOnly,Category="CreateNumber")
	int32 CreateNum=0;

	UPROPERTY(BlueprintReadOnly, Category = "CreateNumber")
	bool IsCreateBoss=false;

	UFUNCTION(BlueprintCallable,Category="MyFunction")
	void LoopCreateEnemy();

	//UFUNCTION(BlueprintCallable, Category = "MyFunction")
	//void CreateBoss();

	//以上是enemy的动态创建
	//下面是soldier的动态创建
	UPROPERTY(BlueprintReadOnly, Category = "CreatePoint")
	TArray<AActor*> SoldierCreatePointList;

	UPROPERTY(EditAnywhere, Category = "EnemyClass")
	TArray<TSubclassOf<ASoldier1Character>> SoldierClassList;

	UPROPERTY(EditAnywhere, Category = "EnemyClass")
	TArray<ASoldier1Character*> SoldierList;

	UPROPERTY(BlueprintReadOnly, Category = "CreateNumber")
	int32 SoldierNum = 0;
	//音乐组件
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Audio")
	TSubclassOf<AAudioActor> AudioActorClass;

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void LoopCreateSoldier();

	//以下是pal的处理策略
	UPROPERTY(BlueprintReadOnly, Category = "CreatePoint")
	TArray<AActor*> PalCreatePointList;

	UPROPERTY(EditAnywhere, Category = "EnemyClass")
	TArray<TSubclassOf<ABasePalCharacter>> PalClassList;

	UPROPERTY(EditAnywhere, Category = "EnemyClass")
	TArray<ABasePalCharacter*> PalList;

	UPROPERTY(BlueprintReadOnly, Category = "CreateNumber")
	int32 PalNum = 0;

	UPROPERTY(BlueprintReadOnly, Category = "CreatePoint")
	TObjectPtr<AAudioActor> AudioActor;

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void LoopCreatePal();

public:
	UPROPERTY(BlueprintReadOnly, Category = "CreatePoint")
	AActor* BossCreatePoint;

	UFUNCTION(BlueprintCallable,Category="MyFunction")
	void AddPointToList(AActor* point);

	UFUNCTION(BlueprintCallable,Category="MyFunction")
	void BeginCreateEnemy();

	UFUNCTION(BlueprintCallable,Category="MyFunction")
	void RemoveEnemy(AEnemyBaseCharacter* enemy);

	//以下是soldier的处理策略
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void AddPointToSoldierList(AActor* point);

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void BeginCreateSoldier();

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void RemoveSoldier(ASoldier1Character* enemy);

	//以下是Pal的处理策略
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void AddPointToPalList(AActor* point);

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void BeginCreatePal();

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void RemovePal(ABasePalCharacter* enemy);

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void OnVolumeValueChange(float value);

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void OnMusicPaused(bool ison);
};
