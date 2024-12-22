#pragma once

#include "Components/StaticMeshComponent.h"
#include "PlayerBaseCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Engine/DataTable.h" 
#include "UObject/StructOnScope.h"
#include "UObject/Class.h"
#include "Architecture.generated.h"




USTRUCT(BlueprintType)
struct FSBuildables : public FTableRowBase
{
	GENERATED_BODY()
public:
	/** The mesh for the buildable */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Mesh", MakeStructureDefaultValue = "None"))
	TObjectPtr<UStaticMesh> Mesh;

	/** The trace channel used for placement validation */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "TraceChannel", MakeStructureDefaultValue = "TraceTypeQuery1"))
	TEnumAsByte<ETraceTypeQuery> TraceChannel;

	/** The actor to spawn when the buildable is placed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Actor"))
	TSubclassOf<AActor> Actor;


};


UCLASS(Blueprintable)
class PALUWORLD_API AArchitecture : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArchitecture();
	// TArray 应直接使用 SBuildables 类型
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Buildable");
	TArray<FSBuildables> Buildables;


	UFUNCTION(BlueprintImplementableEvent,  BlueprintCallable, Category = "BuildDelay")
	void BuildDelay();
protected:

	// 摄像机组件
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera");
	UCameraComponent* camera;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BuildMode")
	bool isBuildModeOn;          // 是否开启建造模式

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BuildMode")
	bool canBuild;              // 是否允许建造

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BuildMode")
	int buildID;                // 建造物品的ID

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BuildMode")
	FTransform buildTransform;  // 建造物品的位置信息（位置、旋转、比例）

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BuildMode")
	UStaticMeshComponent* buildGhost;

	//监测碰撞结果，知道碰到的是什么
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerRef")
	AActor* hitactor;   

	//监测碰撞结果，知道碰到的是什么
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerRef")
	UPrimitiveComponent* hitcomponent;


	// 存储 PlayerBaseCharacter 的引用
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerRef")
	APlayerBaseCharacter* playerRef;   // 使用指针以便动态绑定

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void GiveBuildColor(bool isGreen);//监测材质

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	bool BuildCycle();//创建构建周期

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void ChangeMesh();//改变材质

};