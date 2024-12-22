// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePalCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PalAIController.h"
#include "DebugHelper.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "PlayerBaseCharacter.h"
#include "CombatGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "BaseCharacter.h"



// Sets default values
ABasePalCharacter::ABasePalCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//����ת��Ĳ�������Ϊfalse
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	//��ӿ�������������
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABasePalCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//�йس�ʼֵ���趨
	Health = MaxHealth;

	//��ʼ��������
	ownerProperty = PersonProperty::NONE;

	//�����ؼ�
	UHealthBarWidget* HealthBarWidget = CreateWidget<UHealthBarWidget>(GetWorld(), HealthBarWidgetClass);
	HealthBar->SetWidget(HealthBarWidget); //��Ѫ����ֵ���������

	//��Ѫ���Ĺ��ض������ø��Լ�
	HealthBarWidget->ownerCharacter = this;    
}

// Called every frame
void ABasePalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasePalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ABasePalCharacter::MyDelay(float delayTime, FName functionName)
{
	FLatentActionInfo ActionInfo;
	ActionInfo.Linkage = 1;
	ActionInfo.UUID = 100;
	ActionInfo.CallbackTarget = this;
	ActionInfo.ExecutionFunction = functionName;

	UKismetSystemLibrary::Delay(GetWorld(), delayTime, ActionInfo);
}

//�Լ�������ʵ��
void ABasePalCharacter::PlayAttack()
{
	IsAttacking = true;
	//���Ź�������
	GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
}

//ʵ����Ϣ�����Ĳ���
void ABasePalCharacter::PlayRest0()
{
	if (Rest0Montage) {
		//������Ϣ����
		Debug::Print("Rest0");
		GetMesh()->GetAnimInstance()->Montage_Play(Rest0Montage);
	}
}
void ABasePalCharacter::PlayRest1()
{
	if (Rest1Montage) {
		//������Ϣ����
		Debug::Print("Rest1");
		GetMesh()->GetAnimInstance()->Montage_Play(Rest1Montage);
	}
}

void ABasePalCharacter::PlayEncounter()
{
	if (EncounterMontage) {
		//����ż������
		GetMesh()->GetAnimInstance()->Montage_Play(EncounterMontage);
	}
}

//��³�Ĺ�������˺�
void ABasePalCharacter::AttackDamage()
{
	FVector StartPos = GetActorLocation();
	FVector EndPos = GetActorForwardVector() * 200 + StartPos;

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);

	TArray<AActor*> ignoreActors;
	TArray<FHitResult> hitList;
	bool isResult = UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), StartPos, EndPos, AttackRange, FName(""), true, ignoreActors, EDrawDebugTrace::ForDuration, hitList, true);
	if (isResult) {
		for (FHitResult Hit : hitList) {
			ABasePalCharacter* pal = Cast<ABasePalCharacter>(Hit.GetActor());
			if (pal && pal != this) {
				//ͨ����Ӫ�ж��Ƿ�Ϊ����
				Faction enemyFaction = pal->GetFaction();
				bool isEnemy = IsEnemy(faction, enemyFaction);
				if (isEnemy) {
					pal->TakeHitDamage(AttackValue);
				}
			}
			else {
				APlayerBaseCharacter* pal0 = Cast<APlayerBaseCharacter>(Hit.GetActor());
				if (pal) {
					Faction enemyFaction = pal->GetFaction();
					bool isEnemy = IsEnemy(faction, enemyFaction);
					if (isEnemy) {
						pal0->DiaoXie(AttackValue);
					}
				}
			}
		}
	}
}
//��³�����˼�Ѫ
void ABasePalCharacter::TakeHitDamage(const float DamageValue)
{
	Health -= DamageValue;
	if (Health <= 0) {
		Health = 0;
		//Ѫ���������������Ӧ����
		DeathResponse();
	}
	else {
		//ʵ��������Ӧ
		HitResponse();
	}
}

//��³���˺���Ӧ
void ABasePalCharacter::HitResponse()
{
	//�������˵���̫�涯��
	GetMesh()->GetAnimInstance()->Montage_Play(HitMontage);
	if (!IsDamage) {
		//����������
		IsDamage = true;

		APalAIController* controller = Cast<APalAIController>(GetController());

		if (controller) {
			controller->SetAIDamage();
		}
	}
}
//��³��������Ӧ
void ABasePalCharacter::DeathResponse()
{
	//��ȡAI���
	AAIController* controller = Cast<AAIController>(GetController());
	//����Ƿ��ȡ���
	if (controller) {
		//ֹͣһ���߼�
		controller->BrainComponent->StopLogic("");
		//����˶�״̬
		controller->ClearFocus(EAIFocusPriority::Default);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		//ȥ����ײ��
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//ȥ����ײ��ĸ���
		SetActorEnableCollision(false);
		//��ֹ��һ������
		SetActorTickEnabled(false);
	}
	//������������
	GetMesh()->GetAnimInstance()->Montage_Play(DeathMontage);
	//����Ѫ�����
	HealthBar->DestroyComponent();

	ACombatGameMode* gameMode = Cast<ACombatGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->RemovePal(this);

	MyDelay(3.f, FName("DestroyPal"));
}
void ABasePalCharacter::DestroyPal()
{
	Destroy();
}
//��ȡ��ǰ�Լ����������
PersonProperty ABasePalCharacter::GetPersonalProperty()
{
	return ownerProperty;
}

APlayerBaseCharacter* ABasePalCharacter::GetAttackRangePlayer(const TArray<FHitResult> hit)
{
	APlayerBaseCharacter* player;
	//����ʶ�𵽵�Actor�����ǲ������
	for (FHitResult Hit : hit) {
		player = Cast<APlayerBaseCharacter>(Hit.GetActor());
		if (player) {
			return player;
		}
	}

	return nullptr;
}
Faction ABasePalCharacter::GetFaction()
{
	return faction;
}
