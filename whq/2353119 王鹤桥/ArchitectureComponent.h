// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "PlayerBaseCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Engine/DataTable.h" 
#include "UObject/StructOnScope.h"
#include "BuildInterface.h"
#include "UObject/Class.h"
#include "Components/ActorComponent.h"
#include "ArchitectureComponent.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PALUWORLD_API UArchitectureComponent : public UActorComponent,public IBuildInterface
{
	GENERATED_BODY()

public:	
	//���캯��
	UArchitectureComponent();

	//����buildable�ṹ��ͳ��滮����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Buildable");
	TArray<FSBuildables> Buildables;

protected:
	//��ʼ��Ϸʱִ���߼�
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "BuildDelay")
	void BuildDelay();

	//����ʱ����������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera");
	UCameraComponent* camera;



public:	
	// ÿ֡����ִ�еĺ���
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//�����Ƿ����ڿ�ʼ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BuildMode")
	bool isBuildModeOn;         

	//���鵱ǰλ���Ƿ���Խ���
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BuildMode")
	bool canBuild;            

	//��ǰ�Ľ�����ID
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BuildMode")
	int buildID;            

	//��������ת����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BuildMode")
	FTransform buildTransform;  

	//����������
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BuildMode")
	UStaticMeshComponent* buildGhost;

	// ��������ײactor
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerRef")
	AActor* hitactor;

	//��������ײcomponent
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerRef")
	UPrimitiveComponent* hitcomponent;


	//��ϵ PlayerBaseCharacter       
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerRef")
	APlayerBaseCharacter* playerRef; 

	//ȷ��������ɫ
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void GiveBuildColor(bool isGreen);      

	//�������ڿ���
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	bool BuildCycle();          

	//�ı佨������
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void ChangeMesh();

	//�����ײ��
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void DetectBuildBoxes();

	//�����ǶԽӿڵĶԽ���ϵ
	virtual void ReturnBoxes_Implementation(TArray<UBoxComponent*>& Boxes) override;
	virtual void SetMesh_Implementation(UStaticMesh* Mesh) override;
	virtual void InteractWithBuild_Implementation() override;
	virtual void SetBuildCost_Implementation(FSBuildables buildable, int32 buildid) override;
	virtual void ReturnbuildID_Implementation(int buildid) override;
	virtual void isfinished_Implementation(bool isbuildfinished) override;
	virtual void DealDamage_Implementation(double damage) override;
		
};
