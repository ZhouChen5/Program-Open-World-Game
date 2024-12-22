// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DebugHelper.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerBaseCharacter.h"

ATree::ATree()
{
	// ���� StaticMeshComponent����ʵ������������ͼ�д��������õ�
	tree = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("tree"));
	RootComponent = tree;  // ����Ϊ�����

	// ���� Box ������ײ���
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollisionComponent->SetupAttachment(RootComponent);  // �� Box ������ӵ��������

	// ������ײ
	BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);  // ��������Ͳ�ѯ��ײ

	treeup = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("treeup"));
	treeup->SetupAttachment(RootComponent);  // ������������ӵ��������

	treedown = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("treedown"));
	treedown->SetupAttachment(RootComponent);  // ������������ӵ��������


	// ����������Niagara��Ч���
	NiagaraEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
	NiagaraEffectComponent->SetupAttachment(treeup);

}

void ATree::BeginPlay()
{
	Super::BeginPlay();

}

void ATree::Tick(float DealtaTime)
{


}

void ATree::AttackDamage()
{

}


//�ܵ��˺���Ŀ�Ѫ����
void ATree::TakeHitDamage(const float damageValue)
{
	if (health > 0)
	{
		health -= damageValue;
		Debug::print("hit111111111111111111");
		if (health <= 0)
		{
			health = 0;
			DeathResponse();   //ûѪ������
		}
		else
			HitResponse();     //����Ѫ�˺�
	}
	else
	{
		//��ԭ�е������ٿɼ�
		tree->SetVisibility(false);
		//����ԭ����ײ��ʧ
		tree->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//���������ɼ�
		treeup->SetVisibility(true);
		treedown->SetVisibility(true);
		//��������ײ����
		treeup->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		treedown->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		treeup->SetSimulatePhysics(true);

		//���õ���Ч��
		FVector impulse(50.0f, 10.0f, 0.0f);
		treeup->AddImpulse(impulse, NAME_None, true);
		if (GetWorld())
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ATree::OnCutting, 20, false, 3);

	}
}



void ATree::HitResponse()
{

}

void ATree::DeathResponse()
{

}

void ATree::Chop_Implementation(float damage, FVector hitlocation)
{
	damage = 80;
	Debug::print("kan");
	damage = getdamage;
	//����Ѫ��
	health = FMath::Clamp(health - damage, -100, maxhealth);

	float middlehealth = health / maxhealth;


	TrunkMaterial->SetScalarParameterValue("Strength", 1 - middlehealth);

	//������ת��ʼλ��
	FVector niagaralocation = NiagaraEffectComponent->GetComponentLocation();

	// �����ɫ��Ŀ��λ�õ���ת
	FRotator LookAtRotation = FRotationMatrix::MakeFromX(hitlocation - niagaralocation).Rotator();

	float xnum = NiagaraEffectComponent->GetComponentRotation().Pitch;
	float ynum = NiagaraEffectComponent->GetComponentRotation().Roll;
	//��x,y����ת����0��
	LookAtRotation.Pitch = xnum;
	LookAtRotation.Roll = ynum;
	LookAtRotation.Yaw -= 20.0f;
	//ִ����ת
	NiagaraEffectComponent->SetWorldRotation(LookAtRotation);

	NiagaraEffectComponent->Activate(true);
	//��鵱ǰѪ��
	if (health <= 0)
	{
		CutResponse();
		hasfallen = true;
		if (!hasbeencut)
		{
			Debug::Print("kandaole");
			hasbeencut = true;
		}
		else
		{
			Debug::Print("kansile");
			if (health <= (-1 * trunkhealth))
				OnCutting();
		}
	}

}

void ATree::CutResponse()
{
	//��ԭ�е������ٿɼ�
	tree->SetVisibility(false);
	//����ԭ����ײ��ʧ
	tree->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//���������ɼ�
	treeup->SetVisibility(true);
	treedown->SetVisibility(true);
	//��������ײ����
	treeup->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	treedown->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	treeup->SetSimulatePhysics(true);

	//���õ���Ч��
	FVector impulse(50.0f, 10.0f, 0.0f);
	treeup->AddImpulse(impulse, NAME_None, true);
}

void ATree::OnCutting()
{
	//�õ��ص������ɼ�����ײ
	treeup->SetVisibility(false);
	treeup->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ATree::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	// ���ж�̬�����޸ġ������ʼ���Ȳ���
	
}


