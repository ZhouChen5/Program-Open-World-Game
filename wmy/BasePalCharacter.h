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
	//����������
	int count = 0;


//��������
public:
	//���캯��
	ABasePalCharacter();

	//��֡����
	virtual void Tick(float DeltaTime) override;

	//�����ƶ�
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	//��ʼ
	virtual void BeginPlay() override;





	//����Ϊ�����ڲ��߼��йصĿ��Ʊ���
protected:
	//�Ƿ��յ�����
	UPROPERTY(BlueprintReadWrite, Category = "MyVariable")
	bool IsDamage = false;
	//������������̫��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	UAnimMontage* AttackMontage;
	//���˶�������̫��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> HitMontage;
	//������������̫��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> DeathMontage;
	//��Ϣ��������̫��
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> Rest0Montage;
	//��Ϣ��������̫��
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> Rest1Montage;
	//�ܾ���������̫��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> EncounterMontage;

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void MyDelay(float delayTime, FName functionName);

public:
	//��Ϊ���й���Ϊ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;
	//�Ƿ��ڹ���״̬
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	bool IsAttacking = false;
	//�Ƿ�����Ϣ״̬
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	bool IsResting = false;
	//�Ƿ�������״̬
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	bool IsDeath = false;

	//ʵ�ֹ�������
	UFUNCTION()
	void PlayAttack();
	//ʵ����Ϣ����
	UFUNCTION()
	void PlayRest0();
	UFUNCTION()
	void PlayRest1();
	//ʵ��ż������
	UFUNCTION()
	void PlayEncounter();

	//ʵ�ֹ����˺�����
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void AttackDamage();
	//���˿ۼ�Ѫ��
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void TakeHitDamage(const float DamageValue);

	//�����³�˺���Ӧ
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void HitResponse();
	//�����³������Ӧ
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void DeathResponse();

	UFUNCTION()
	void DestroyPal();








//һЩ�������Ե�ʵ��
private:

protected:

	//��Ѫ���йص�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthValue")
	float MaxHealth = 100.0f;                   //Ѫ������
	float Health = 100.0f;                      //ʵ��Ѫ��

	//��Ӫ��ǩ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Faction");
	Faction faction = Faction::ENEMY;
	//���Ա�ǩ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Property");
	PersonProperty ownerProperty;

public:
	//��ȡ��ǰ����ֵ
	FORCEINLINE float GetHealthProgress() { return Health / MaxHealth; }
	//��ȡ����ֵ�������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIWidget")
	TSubclassOf<UHealthBarWidget> HealthBarWidgetClass;
	//��ȡ���ӻ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WidgetComp")
	UWidgetComponent* HealthBar;
	//��ȡ��³����Ӫ
	UFUNCTION()
	Faction GetFaction();
	//��ȡ��ǰ��³������
	UFUNCTION()
	PersonProperty GetPersonalProperty();
	//��ȡ������
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	APlayerBaseCharacter* GetAttackRangePlayer(const TArray<FHitResult> hit);
	//������Χ������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttackRange")
	float AttackRange;
	//�����˺�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttackRange")
	float AttackValue;


};
