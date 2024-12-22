// Fill out your copyright notice in the Description page of Project Settings.

#include "toos_axe.h"
#include"DebugHelper.h"
#include"PlayerBaseCharacter.h"
#include "Kismet/GameplayStatics.h"

Atoos_axe::Atoos_axe()
{
	
	// �趨Ĭ��ֵ
	PrimaryActorTick.bCanEverTick = true;

	// �����������������
	AxeMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AxeMesh"));
	RootComponent = AxeMeshComponent;
	AxeMeshComponent->SetVisibility(false);

	// ���� Box ������ײ���
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollisionComponent->SetupAttachment(RootComponent);  // �� Box ������ӵ��������

	// ������ײ
	BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);  // ��������Ͳ�ѯ��ײ
	BoxCollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);  // Ĭ�Ϻ���������ײ

	// ���� Damage ������ײ���
	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageCollision"));
	DamageCollision->SetupAttachment(RootComponent);  // �� Box ������ӵ��������

	// ������ײ
	DamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);  // ��������Ͳ�ѯ��ײ
	DamageCollision->SetCollisionResponseToAllChannels(ECR_Ignore);  // Ĭ�Ϻ���������ײ

}




void Atoos_axe::BeginPlay()
{
	Super::BeginPlay();

}

void Atoos_axe::EnableHit(bool enable)
{

	weaponowner = Cast<APlayerBaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (weaponowner)
	{
		enable = weaponowner->bIsAttack;
	}
	//���enableΪ�棬����Ϊ����������ѯ��ײ
	if(1)
	{
		BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	//��Ϊ�٣�������ײ
	else
	{
		BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void Atoos_axe::Chop_Implementation(float damage, FVector hitlocation)
{
}
