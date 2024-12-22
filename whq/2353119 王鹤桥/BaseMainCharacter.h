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

	//��ɫ���˺���Ӧ
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	virtual void HitResponse();

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	virtual void DeathResponse();

	UFUNCTION()
	virtual void PlayAttack();

	FORCEINLINE float GetHealthProgress() { return health / maxHealth; };


	//��������
	UFUNCTION(BlueprintCallable, Category = "Open")
	void OpenLobby();

	UFUNCTION(BlueprintCallable, Category = "Open")
	void CallOpenLevel(const FString& address);

	UFUNCTION(BlueprintCallable, Category = "Open")
	void CallClientTravel(const FString& address);

	//����ר�ſ��ƶ�����Ϸģ��
public:
	//ָ�������Ϸ��ָ��(����Ự�ӿ�)
	IOnlineSessionPtr OnlineSessionInterface;

protected:
	//����һ���Ự
	UFUNCTION(BlueprintCallable, Category = "Open")
	void CreateGameSession();

	UFUNCTION(BlueprintCallable, Category = "Open")
	//�����Ự�����Ƿ�ɹ�
	void OnCreateSessionComplete(FName session, bool issuccessful);

	//�������Ӽ��̵ĺ�����������λ
	UFUNCTION(BlueprintCallable, Category = "Open")
	void JoinGameSession();
	//�ҵ��Ự�ĺ���
	UFUNCTION(BlueprintCallable, Category = "Open")
	void OnFindSessionComplete(bool success);
	//�ɹ�����
	void OnJoinSessionComplete(FName name, EOnJoinSessionCompleteResult::Type result);

private:
	//�Ự
	FOnCreateSessionCompleteDelegate delegate;
	FOnFindSessionsCompleteDelegate findsession;//���һỰ
	TSharedPtr<FOnlineSessionSearch> search;
	FOnJoinSessionCompleteDelegate join;
};

