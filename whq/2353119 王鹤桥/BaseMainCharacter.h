// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "JoinSessionCallbackProxy.h"
#include "FindSessionsCallbackProxy.h"
#include "OnlineSubsystem.h"
#include "GameFramework/Character.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "BaseMainCharacter.generated.h"


UCLASS()
class PALUWORLD_API ABaseMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseMainCharacter();

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	bool isAttacking = false;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	virtual void AttackDamage();
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	virtual void TakeHitDamage(const float damageValue);

	//角色的伤害相应
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	virtual void HitResponse();

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	virtual void DeathResponse();

	UFUNCTION()
	virtual void PlayAttack();

	FORCEINLINE float GetHealthProgress() { return health / maxHealth; };


	//联机操作
	UFUNCTION(BlueprintCallable, Category = "Open")
	void OpenLobby();

	UFUNCTION(BlueprintCallable, Category = "Open")
	void CallOpenLevel(const FString& address);

	UFUNCTION(BlueprintCallable, Category = "Open")
	void CallClientTravel(const FString& address);

	//这里专门控制多人游戏模块
public:
	//指向多人游戏的指针(保存会话接口)
	IOnlineSessionPtr OnlineSessionInterface;

protected:
	//创建一个会话
	UFUNCTION(BlueprintCallable, Category = "Open")
	void CreateGameSession();

	UFUNCTION(BlueprintCallable, Category = "Open")
	//创建会话调用是否成功
	void OnCreateSessionComplete(FName session, bool issuccessful);

	//创建连接键盘的函数，两个键位
	UFUNCTION(BlueprintCallable, Category = "Open")
	void JoinGameSession();
	//找到会话的函数
	UFUNCTION(BlueprintCallable, Category = "Open")
	void OnFindSessionComplete(bool success);
	//成功加入
	void OnJoinSessionComplete(FName name, EOnJoinSessionCompleteResult::Type result);

private:
	//会话
	FOnCreateSessionCompleteDelegate delegate;
	FOnFindSessionsCompleteDelegate findsession;//查找会话
	TSharedPtr<FOnlineSessionSearch> search;
	FOnJoinSessionCompleteDelegate join;
};

