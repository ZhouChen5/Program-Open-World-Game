


#include "ArchitectureComponent.h"

// ���캯������
UArchitectureComponent::UArchitectureComponent()
{
	// ��ʼ�����캯��
	PrimaryComponentTick.bCanEverTick = true;
	// ��ʼ����������
	camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	// ...
}


// ��Ϸ��ʼʱ�Ĵ���
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


//�ı���ɫ����������ɫ��ɫ���أ��ɽ���Ϊ��ɫ�����ɽ���Ϊ��ɫ
void UArchitectureComponent::GiveBuildColor(bool isGreen)
{
	//������ɫ����
	UMaterialInterface* greenmaterial = Cast<UMaterialInterface>(StaticLoadObject(
		UMaterialInterface::StaticClass(),
		nullptr,
		TEXT("/Script/Engine.Material'/Game/ItsMeBroBaseBuildingAssets/GreenColor.GreenColor'")
	));

	//�����ɫ����
	UMaterialInterface* redmaterial = Cast<UMaterialInterface>(StaticLoadObject(
		UMaterialInterface::StaticClass(),
		nullptr,
		TEXT("/Script/Engine.Material'/Game/ItsMeBroBaseBuildingAssets/RedColor.RedColor'")
	));

	//���Ҫ�������ݵ��ܲ�������
	int num = buildGhost->GetNumMaterials();
	for (int i = 0; i < num; i++)
	{

		//��ɫ��ɫ���óɶ�Ӧ��ɫ
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

	// ȷ�� forward vector �ǵ�λ����
	secondPosition.Normalize();

	int firstFollowDistance = 1000;
	int secondFollowDistance = 350;

	// ���������յ�
	FVector thirdPosition = secondPosition * firstFollowDistance;
	FVector forthPosition = firstPosition + thirdPosition;

	// �����������
	FVector fifthPosition = secondPosition * secondFollowDistance;
	FVector sixthPosition = firstPosition + fifthPosition;

	// ��ȡ����ָ��
	UWorld* world = GetWorld();
	if (!world)
	{
		UE_LOG(LogTemp, Warning, TEXT("World pointer is null."));
		return 0;
	}

	// ʹ�� TArray �洢������н��
	FHitResult hitResults;

	// �������߼��ͨ�������� ECC_Visibility��
	ECollisionChannel TraceChannel{};

	// ִ�����߼��
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
	//�ı�����ʲ�
	buildGhost->SetStaticMesh(Buildables[buildID].Mesh);

}

void UArchitectureComponent::DetectBuildBoxes()
{

}



//�����߸�����Ϊ�ӿڼ̳к�����cpp��������
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
//����������������������������������������������������������������������������������������������������������������������������