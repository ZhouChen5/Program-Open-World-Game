


#include "ArchitectureComponent.h"

// 构造函数处理
UArchitectureComponent::UArchitectureComponent()
{
	// 初始化构造函数
	PrimaryComponentTick.bCanEverTick = true;
	// 初始化摄像机组件
	camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	// ...
}


// 游戏开始时的处理
void UArchitectureComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UArchitectureComponent::BuildDelay()
{

}


// Called every frame
void UArchitectureComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


//改变颜色，总体由绿色红色调控，可建造为绿色，不可建造为红色
void UArchitectureComponent::GiveBuildColor(bool isGreen)
{
	//引入绿色材质
	UMaterialInterface* greenmaterial = Cast<UMaterialInterface>(StaticLoadObject(
		UMaterialInterface::StaticClass(),
		nullptr,
		TEXT("/Script/Engine.Material'/Game/ItsMeBroBaseBuildingAssets/GreenColor.GreenColor'")
	));

	//引入红色材质
	UMaterialInterface* redmaterial = Cast<UMaterialInterface>(StaticLoadObject(
		UMaterialInterface::StaticClass(),
		nullptr,
		TEXT("/Script/Engine.Material'/Game/ItsMeBroBaseBuildingAssets/RedColor.RedColor'")
	));

	//获得要建造内容的总材质数量
	int num = buildGhost->GetNumMaterials();
	for (int i = 0; i < num; i++)
	{

		//红色绿色设置成对应颜色
		if (isGreen)
		{
			buildGhost->SetMaterial(i, greenmaterial);
		}
		else
		{
			buildGhost->SetMaterial(i, redmaterial);
		}
	}
	buildGhost->SetWorldTransform(buildTransform);

}

bool UArchitectureComponent::BuildCycle()
{
	FVector firstPosition = camera->GetComponentLocation();
	FVector secondPosition = camera->GetForwardVector();

	// 确保 forward vector 是单位向量
	secondPosition.Normalize();

	int firstFollowDistance = 1000;
	int secondFollowDistance = 350;

	// 计算射线终点
	FVector thirdPosition = secondPosition * firstFollowDistance;
	FVector forthPosition = firstPosition + thirdPosition;

	// 计算射线起点
	FVector fifthPosition = secondPosition * secondFollowDistance;
	FVector sixthPosition = firstPosition + fifthPosition;

	// 获取世界指针
	UWorld* world = GetWorld();
	if (!world)
	{
		UE_LOG(LogTemp, Warning, TEXT("World pointer is null."));
		return 0;
	}

	// 使用 TArray 存储多个命中结果
	FHitResult hitResults;

	// 定义射线检测通道（例如 ECC_Visibility）
	ECollisionChannel TraceChannel{};

	// 执行射线检测
	bool bHit = world->LineTraceSingleByChannel(hitResults, sixthPosition, forthPosition, TraceChannel);
	FQuat rotation = buildTransform.GetRotation();
	FVector scale = buildTransform.GetScale3D();
	buildTransform.SetRotation(rotation);
	buildTransform.SetScale3D(scale);
	if (bHit)
	{
		buildTransform.SetLocation(hitResults.ImpactPoint);
		hitactor = hitResults.GetActor();
		hitcomponent = hitResults.GetComponent();
		if (IsValid(buildGhost))
		{

		}
		else
			return true;
	}
	else
	{
		buildTransform.SetLocation(hitResults.TraceEnd);
		if (IsValid(buildGhost))
		{
			GiveBuildColor(1);
			BuildDelay();
		}
		else
			return true;
	}
	return false;
}


void UArchitectureComponent::ChangeMesh()
{
	//改变材质资产
	buildGhost->SetStaticMesh(Buildables[buildID].Mesh);

}

void UArchitectureComponent::DetectBuildBoxes()
{

}



//以下七个函数为接口继承函数，cpp不做操作
void UArchitectureComponent::ReturnBoxes_Implementation(TArray<UBoxComponent*>& Boxes)
{
}

void UArchitectureComponent::SetMesh_Implementation(UStaticMesh* Mesh)
{
}

void UArchitectureComponent::InteractWithBuild_Implementation()
{
}

void UArchitectureComponent::SetBuildCost_Implementation(FSBuildables buildable, int32 buildid)
{
}

void UArchitectureComponent::ReturnbuildID_Implementation(int buildid)
{
}

void UArchitectureComponent::isfinished_Implementation(bool isbuildfinished)
{
}

void UArchitectureComponent::DealDamage_Implementation(double damage)
{
}
//——————————————————————————————————————————————————————————————