// Fill out your copyright notice in the Description page of Project Settings.


#include "Trunk.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DebugHelper.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerBaseCharacter.h"
// Sets default values
ATrunk::ATrunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// ���� StaticMeshComponent����ʵ������������ͼ�д��������õ�
	tree = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("trunk"));
	RootComponent = tree;  // ����Ϊ�����

	// ���� Box ������ײ���
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollisionComponent->SetupAttachment(RootComponent);  // �� Box ������ӵ��������

	// ������ײ
	BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);  // ��������Ͳ�ѯ��ײ
	BoxCollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);  // Ĭ�Ϻ���������ײ


	// ����������Niagara��Ч���
	NiagaraEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
	NiagaraEffectComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATrunk::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrunk::AttackDamage()
{
}

void ATrunk::TakeHitDamage(const float damageValue)
{
	if (health > 0)
	{
		health -= damageValue;
		if (health <= 0)
		{
			health = 0;
			DeathResponse();   //ûѪ������
		}
		else
			HitResponse();     //����Ѫ�˺�
	}
}

void ATrunk::HitResponse()
{
}

void ATrunk::DeathResponse()
{
}

void ATrunk::Chop_Implementation(float damage, FVector hitlocation)
{
	damage = getdamage;
	//����Ѫ��
	health = FMath::Clamp(health - damage, 0, maxhealth);

	float middlehealth = health / maxhealth;


	TrunkMaterial->SetScalarParameterValue("Strength", 1 - middlehealth);

	//������ת��ʼλ��
	FVector niagaralocation = NiagaraEffectComponent->GetComponentLocation();

	// �����ɫ��Ŀ��λ�õ���ת
	FRotator LookAtRotation = FRotationMatrix::MakeFromX(hitlocation - niagaralocation).Rotator();

	//��x,y����ת����0��
	LookAtRotation.Pitch = 0.0f;
	LookAtRotation.Roll = 0.0f;
	LookAtRotation.Yaw -= 20.0f;
	//ִ����ת
	NiagaraEffectComponent->SetWorldRotation(LookAtRotation);

	NiagaraEffectComponent->Activate(true);
}



void ATrunk::OnConstruction(const FTransform& Transform)
{
}

