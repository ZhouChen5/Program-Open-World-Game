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
	//构造函数
	UArchitectureComponent();

	//引入buildable结构，统筹规划建筑
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Buildable");
	TArray<FSBuildables> Buildables;

protected:
	//开始游戏时执行逻辑
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "BuildDelay")
	void BuildDelay();

	//建筑时的摄像机组件
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera");
	UCameraComponent* camera;



public:	
	// 每帧都在执行的函数
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//检验是否正在开始建造
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BuildMode")
	bool isBuildModeOn;         

	//检验当前位置是否可以建造
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BuildMode")
	bool canBuild;            

	//当前的建筑物ID
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BuildMode")
	int buildID;            

	//建筑物旋转操作
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BuildMode")
	FTransform buildTransform;  

	//建筑物生成
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BuildMode")
	UStaticMeshComponent* buildGhost;

	// 建筑物碰撞actor
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerRef")
	AActor* hitactor;

	//建筑物碰撞component
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerRef")
	UPrimitiveComponent* hitcomponent;


	//联系 PlayerBaseCharacter       
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerRef")
	APlayerBaseCharacter* playerRef; 

	//确定建筑颜色
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void GiveBuildColor(bool isGreen);      

	//建筑周期控制
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	bool BuildCycle();          

	//改变建筑材质
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void ChangeMesh();

	//监测碰撞箱
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void DetectBuildBoxes();

	//以下是对接口的对接联系
	virtual void ReturnBoxes_Implementation(TArray<UBoxComponent*>& Boxes) override;
	virtual void SetMesh_Implementation(UStaticMesh* Mesh) override;
	virtual void InteractWithBuild_Implementation() override;
	virtual void SetBuildCost_Implementation(FSBuildables buildable, int32 buildid) override;
	virtual void ReturnbuildID_Implementation(int buildid) override;
	virtual void isfinished_Implementation(bool isbuildfinished) override;
	virtual void DealDamage_Implementation(double damage) override;
		
};
