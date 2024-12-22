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
	// TArray Ӧֱ��ʹ�� SBuildables ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Buildable");
	TArray<FSBuildables> Buildables;


	UFUNCTION(BlueprintImplementableEvent,  BlueprintCallable, Category = "BuildDelay")
	void BuildDelay();
protected:

	// ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera");
	UCameraComponent* camera;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BuildMode")
	bool isBuildModeOn;          // �Ƿ�������ģʽ

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BuildMode")
	bool canBuild;              // �Ƿ�������

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BuildMode")
	int buildID;                // ������Ʒ��ID

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BuildMode")
	FTransform buildTransform;  // ������Ʒ��λ����Ϣ��λ�á���ת��������

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BuildMode")
	UStaticMeshComponent* buildGhost;

	//�����ײ�����֪����������ʲô
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerRef")
	AActor* hitactor;   

	//�����ײ�����֪����������ʲô
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerRef")
	UPrimitiveComponent* hitcomponent;


	// �洢 PlayerBaseCharacter ������
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerRef")
	APlayerBaseCharacter* playerRef;   // ʹ��ָ���Ա㶯̬��

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void GiveBuildColor(bool isGreen);//������

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	bool BuildCycle();//������������

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void ChangeMesh();//�ı����

};