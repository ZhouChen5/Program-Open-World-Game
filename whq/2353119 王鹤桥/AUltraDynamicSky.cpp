#include "UltraDynamicSky.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"



// ����̫���������ĸ������ת
void AUltraDynamicSky::SetSunAndMoonRootRotation(FVector realSunPosition, FVector realMoonPosition, double realMoonPhase, FVector moonAlignment, bool updatingSun, bool updatingMoon)
{
    // ����̫��λ�ã�������ʵλ�ü���̫����������ת
    if (updatingSun)
    {
        FRotator SunRotation = FRotator::MakeFromEuler(realSunPosition);
        GetRootComponent()->SetWorldRotation(SunRotation);
    }
    // ��������λ�ã�������ʵλ�ú��������������������ת
    if (updatingMoon)
    {
        FRotator MoonRotation = FRotator::MakeFromEuler(realMoonPosition);
        MoonRotation.Pitch += realMoonPhase * 10.0f; // ģ�������Ӱ��
        GetRootComponent()->SetWorldRotation(MoonRotation);
    }
    // ������������������һ������
    if (moonAlignment != FVector::ZeroVector)
    {
        FRotator AlignmentRotation = FRotator::MakeFromEuler(moonAlignment);
        GetRootComponent()->AddWorldRotation(AlignmentRotation);
    }
}

// UDS��ÿһ֡���õĸ��º���
void AUltraDynamicSky::TickFunction()
{
    // ���¶�̬����������ֵ
    UpdateCommonDerivatives();
    // ��̬�����Ʋ�λ��
    UpdateCloudMovement(0.1f); // ÿ֡�����ƶ���������
    // ���¹��շ���
    UpdateDirectionalLightRotations();
    // ��������ղ�׽����������²�׽
    RecaptureSkyLight();
    // ȷ��ʱ���ǺϷ���
    ForceValidDay();
    // �����Ҫ��������ƫ��
    OffsetDate(1);
}

// ���¶�̬����������ֵ�������ظ�����
void AUltraDynamicSky::UpdateCommonDerivatives()
{
    // ����̫����ʵʱ�������ɫ
    FVector SunDirection = FVector(1.0f, 0.0f, 0.0f); // ʾ������
    SunDirection.Normalize();
    // ģ��̫�����ߵ���ɢ��Ӱ�������ɫ
    FLinearColor SunColor = FLinearColor::MakeRandomColor();
    SunColor *= FMath::Clamp(SunDirection.Z, 0.0f, 1.0f); // �߶�Ӱ����ɫ
    // ������������ɫ����̬����
    FLinearColor FogColor = FLinearColor(0.5f, 0.5f, 0.6f, 1.0f);
    FogColor.R += 0.01f; // ÿ����΢���Ӻ�ɫƫ��
}

// ����ϵͳ����ʱ��Ҫ�����ж�̬����
void AUltraDynamicSky::UpdateActiveVariables()
{
    // ����̫����������λ��
    FVector SunPosition = FVector(100.0f, 200.0f, 300.0f);
    FVector MoonPosition = FVector(-100.0f, -200.0f, -300.0f);
    double MoonPhase = 0.5f; // ����ʾ��
    FVector MoonAlignment = FVector(0.0f, 1.0f, 0.0f);
    SetSunAndMoonRootRotation(SunPosition, MoonPosition, MoonPhase, MoonAlignment, true, true);
    // ������չ��ղ���
    UpdateDirectionalLightRotations();
    // �����Ʋ�Ķ�̬�ٶ�
    FVector CloudVelocity = CloudTextureVelocity();
    CloudVelocity.X += 10.0f; // ģ�������ƶ��ٶ�
}

// ��ȡʱ����
void AUltraDynamicSky::GetTimeCode(FTimecode& timeCode)
{
    // ��ǰʱ����ģ��ʱ���ĳ��ֵ
    timeCode.Hours = 12; // ʾ��ʱ�䣬����Ϊ12Сʱ
    timeCode.Minutes = 30;
    timeCode.Seconds = 15;
    timeCode.Frames = 25; // ÿ��25֡
    // ���������Ϣ
    UE_LOG(LogTemp, Warning, TEXT("TimeCode: %02d:%02d:%02d:%02d"), timeCode.Hours, timeCode.Minutes, timeCode.Seconds, timeCode.Frames);
}

// ʹ�õ�ǰʱ����Ϣ��������ʱ��ṹ��
void AUltraDynamicSky::GetDateTime(FDateTime& currentDateTime)
{
    // ��ǰ����Ϊ2024��12��21��
    currentDateTime = FDateTime(2024, 12, 21, 12, 0, 0);
    // ��ӡ������Ϣ
    UE_LOG(LogTemp, Warning, TEXT("Current DateTime: %s"), *currentDateTime.ToString());
}

// ���̫���ڵ�ƽ����
void AUltraDynamicSky::IsDaytime(bool& yes)
{
    // ����̫���ĸ߶�ֵ�ж�Ϊ����
    FVector SunPosition = FVector(0.0f, 0.0f, 1.0f); // ʾ��̫��λ��
    yes = SunPosition.Z > 0.0f; // Z�����0��ʾ�ڵ�ƽ���Ϸ�
    if (yes)
    {
        UE_LOG(LogTemp, Warning, TEXT("It is daytime."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("It is nighttime."));
    }
}

// ����ʱ����
void AUltraDynamicSky::SetTimeWithTimeCode(FTimecode timeCode)
{
    // ��ʱ����ת��Ϊ�ڲ�ʱ��
    double TotalSeconds = timeCode.Hours * 3600.0 + timeCode.Minutes * 60.0 + timeCode.Seconds + timeCode.Frames / 25.0; // 25֡ÿ��
    UE_LOG(LogTemp, Warning, TEXT("Set Time: %f seconds since midnight."), TotalSeconds);
    // ���¶�̬����
    UpdateActiveVariables();
}

// ���¾�̬����
void AUltraDynamicSky::UpdateStaticVariables()
{
    // �����ɫΪ��ɫ
    FLinearColor SkyColor = FLinearColor(0.2f, 0.4f, 0.8f, 1.0f);
    UE_LOG(LogTemp, Warning, TEXT("Updated static sky color to %s"), *SkyColor.ToString());
}

// ��ȡ�Ʋ��ƶ��ٶ�
FVector AUltraDynamicSky::CloudTextureVelocity()
{
    // ����һ�����Ʋ��ƶ��ٶ�
    FVector Velocity = FVector(10.0f, 5.0f, 0.0f); // �Ʋ��ƶ��ٶ�����
    UE_LOG(LogTemp, Warning, TEXT("Cloud Velocity: %s"), *Velocity.ToString());
    return Velocity;
}

// �����Ʋ�λ��
void AUltraDynamicSky::UpdateCloudMovement(double cachedMovementAlpha)
{
    // ģ���Ʋ���ƶ�
    FVector CloudPosition = FVector(100.0f, 200.0f, 300.0f);
    CloudPosition.X += cachedMovementAlpha * 50.0f; // ����ֵӰ���ƶ�
    UE_LOG(LogTemp, Warning, TEXT("Updated Cloud Position to %s"), *CloudPosition.ToString());
}

// �ڹ���ű��е��õĺ���
void AUltraDynamicSky::ConstructionScriptFunction()
{
    // ��ʼ����̬��ձ���
    UpdateStaticVariables();
    // ���ö�̬��������
    UpdateActiveVariables();
    // ��ӡ������־
    UE_LOG(LogTemp, Warning, TEXT("Construction script executed."));
}

// ���²�׽��չ���
void AUltraDynamicSky::RecaptureSkyLight()
{
    // ģ�����²�׽��չ���
    UE_LOG(LogTemp, Warning, TEXT("Recapturing skylight."));
}
// ȷ�����ںϷ��ԣ������˷Ƿ����ڣ��Զ���������Ч��Χ
void AUltraDynamicSky::ForceValidDay()
{
    // ��鵱ǰ������ʱ��Ϸ�
    FDateTime CurrentDate = FDateTime::Now();
    if (!CurrentDate.IsValid())
    {
        // ��Ч������ΪĬ��ֵ
        CurrentDate = FDateTime(2024, 1, 1, 0, 0, 0); // Ĭ������Ϊ2024��1��1��
        UE_LOG(LogTemp, Warning, TEXT("Invalid date detected. Resetting to default date: %s"), *CurrentDate.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Date is valid: %s"), *CurrentDate.ToString());
    }
}

// ����ƫ�ƺ�����������������������ǰ����
void AUltraDynamicSky::OffsetDate(int32 DaysOffset)
{
    // ��ȡ��ǰ����
    FDateTime CurrentDate = FDateTime::Now();
    // ����ƫ�ƺ������
    FDateTime OffsetDate = CurrentDate + FTimespan(DaysOffset, 0, 0, 0);
    UE_LOG(LogTemp, Warning, TEXT("Offsetting date by %d days. New date: %s"), DaysOffset, *OffsetDate.ToString());
    // ������յĶ�̬����
    UpdateStaticVariables();
}

// ����̫����������������ת
void AUltraDynamicSky::UpdateDirectionalLightRotations()
{
    // ģ��̫������ת����
    FVector SunVector = FVector(1.0f, 0.0f, 1.0f).GetSafeNormal();
    FRotator SunRotation = FRotator::MakeFromEuler(SunVector);
    GetRootComponent()->SetWorldRotation(SunRotation);

    // ģ����������ת����
    FVector MoonVector = FVector(-1.0f, 0.0f, 0.5f).GetSafeNormal();
    FRotator MoonRotation = FRotator::MakeFromEuler(MoonVector);
    MoonRotation.Pitch += 5.0f; // ��΢С����
    GetRootComponent()->AddWorldRotation(MoonRotation);

    UE_LOG(LogTemp, Warning, TEXT("Updated sun and moon directional light rotations."));
}

// ����̫������������ʵλ�ã�������������
void AUltraDynamicSky::ApproximateRealSunMoonAndStars(double timeOfDay, bool onlyCalculateSun, FVector& sunVector, FVector& moonVector, double& realPhase, FVector& phaseAlignment, FLinearColor& celestialYaw, double& celestialOrbit, double utcTimeOfDay, double utcTimeOfYear, int32 utcMonth, int32 utcDay, int32 dayOfYearIndex, double seasonalTime, FRotator localGeocoordinate, FVector viewerPositionOnEarth, double axialTilt, FRotator zenithDirection, int32 utcYear, FVector sunDirection, FLinearColor starsYaw, double starsOrbit, FVector celestialNorthPole, FVector moonDirection, FVector moonAlignment, double moonRealPhase, FVector earthCenter, double lunarTime, FVector moonCenter, double lunarInclinationCycle, UCurveFloat equationOfTimeCurve, double equationOfTimeOffset)
{
    // ����̫��λ��
    sunVector = FVector(FMath::Cos(timeOfDay), FMath::Sin(timeOfDay), FMath::Cos(axialTilt));
    sunVector.Normalize();

    // ��Ҫ���������������ģ��
    if (!onlyCalculateSun)
    {
        moonVector = FVector(FMath::Cos(timeOfDay * 0.5), FMath::Sin(timeOfDay * 0.5), FMath::Cos(axialTilt + 10.0f));
        moonVector.Normalize();
        realPhase = FMath::Abs(FMath::Sin(timeOfDay)); // ģ������
    }

    // �������峯��
    celestialYaw = FLinearColor(1.0f, 0.0f, 0.0f); // ʾ����ɫ
    celestialOrbit = FMath::Fmod(timeOfDay * 360.0f, 360.0f); // �򵥵�������ģ��

    UE_LOG(LogTemp, Warning, TEXT("SunVector: %s, MoonVector: %s, RealPhase: %f"), *sunVector.ToString(), *moonVector.ToString(), realPhase);
}

// �������ں�ʱ��
void AUltraDynamicSky::SetDateAndTime(FDateTime dateTime)
{
    // ���µ�ǰʱ��
    FDateTime CurrentDateTime = dateTime;
    UE_LOG(LogTemp, Warning, TEXT("DateTime set to: %s"), *CurrentDateTime.ToString());

    // ���¶�̬��ղ���
    UpdateActiveVariables();
}

// ���ڿ���չ�Ե�������
void AUltraDynamicSky::UpdateSettingsBasedOnScalability(int32 effectsQuality, bool reconstruct)
{
    // ����Ч������������̬�Ʋ�ֱ���
    if (effectsQuality >= 3)
    {
        UE_LOG(LogTemp, Warning, TEXT("High scalability settings applied. Maximum quality enabled."));
    }
    else if (effectsQuality == 2)
    {
        UE_LOG(LogTemp, Warning, TEXT("Medium scalability settings applied."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Low scalability settings applied. Reducing effects quality."));
    }

    // ��Ҫ���¹����������
    if (reconstruct)
    {
        UE_LOG(LogTemp, Warning, TEXT("Reconstructing sky with updated scalability settings."));
        ConstructionScriptFunction();
    }
}

// ģ�����ǵĹ켣����
void AUltraDynamicSky::UpdateStarMovements()
{
    // �����Թ̶��ٶ�Χ�������ת
    double StarOrbitSpeed = 0.1f; // ÿ����ת�ٶ�
    FRotator StarRotation = FRotator(0.0f, StarOrbitSpeed, 0.0f);
    GetRootComponent()->AddWorldRotation(StarRotation);

    UE_LOG(LogTemp, Warning, TEXT("Updated star movements with speed: %f"), StarOrbitSpeed);
}

// ��̬��������
void AUltraDynamicSky::UpdateFogColor()
{
    // ���㵱ǰ������ɫ
    FLinearColor CurrentFogColor = FLinearColor(0.8f, 0.7f, 0.6f);
    CurrentFogColor.G -= 0.01f; // ��̬������ɫ������ģ�⽥��Ч��
    CurrentFogColor.R += 0.02f; // ��ǿ��ɫ������ģ������
    UE_LOG(LogTemp, Warning, TEXT("Fog color updated to: %s"), *CurrentFogColor.ToString());
}

// ���񲢸��·�����պ�
void AUltraDynamicSky::CaptureReflectionSkybox()
{
    // ģ�Ⲷ����շ���
    UE_LOG(LogTemp, Warning, TEXT("Capturing reflection skybox."));
    UpdateStaticVariables(); // ͬʱ���¾�̬����
}

// ��̬���ٱ仯������Ӱ���Ʋ������
void AUltraDynamicSky::UpdateWindSpeed()
{
    // ģ����ٵĶ�̬�仯
    FVector WindVelocity = FVector(FMath::RandRange(-10.0f, 10.0f), FMath::RandRange(-5.0f, 5.0f), 0.0f);
    UE_LOG(LogTemp, Warning, TEXT("Wind velocity updated to: %s"), *WindVelocity.ToString());
}


// ��ȡ�����ܶ�
double CurrentFogDensity()
{
    UWorld* World = GetWorld();
    if (!World) return 0.0;
    EnvParams Params;
    Params.WeatherFactor = FMath::RandRange(0.0f, 1.0f);
    Params.TimeFactor = FMath::RandRange(0.0f, 1.0f);
    Params.ExposureFactor = FMath::RandRange(0.0f, 1.0f);
    double density = Params.WeatherFactor * 0.3 + Params.TimeFactor * 0.3 + Params.ExposureFactor * 0.4;
    density = FMath::Clamp(density, 0.0, 1.0);
    return density;
}

// ��ȡ�߶�˥��ֵ
double AUltraDynamicSky::FogHeightFalloff()
{
    UWorld* World = GetWorld();
    if (!World) return 0.0;
    EnvParams Params;
    Params.CloudCoverage = FMath::RandRange(0.0f, 1.0f);
    Params.DustFactor = FMath::RandRange(0.0f, 1.0f);
    double fogDensity = CurrentFogDensity();
    double falloff = fogDensity * 0.4 + Params.CloudCoverage * 0.3 + Params.DustFactor * 0.3;
    falloff = FMath::Clamp(falloff, 0.0, 1.0);
    return falloff;
}

// ������ʼ����
double AUltraDynamicSky::FogStartDistance()
{
    double density = CurrentFogDensity();
    double distance = 1000.0 / (density + 0.1);
    distance = FMath::Clamp(distance, 100.0, 2000.0);
    return distance;
}

// ��ȡ����ǿ��
double AUltraDynamicSky::CurrentLitIntensity()
{
    UMaterialInstanceDynamic* SkyMat = UGameplayStatics::CreateAndGetDynamicMaterialInstance(this, TEXT("SkyMaterial"));
    if (!SkyMat) return 0.0;
    float base = SkyMat->GetScalarParameterValue("BaseColorIntensity");
    float cloud = SkyMat->GetScalarParameterValue("CloudIntensity");
    float dynCloud = SkyMat->GetScalarParameterValue("DynamicCloudIntensity");
    double intensity = base * 0.4 + cloud * 0.3 + dynCloud * 0.3;
    return intensity;
}

// ��ȡ����ǿ��
UFUNCTION(BlueprintPure, Category = "Sky Material")
double AUltraDynamicSky::CurrentOverallIntensity()
{
    UMaterialInstanceDynamic* SkyMat = UGameplayStatics::CreateAndGetDynamicMaterialInstance(this, TEXT("SkyMaterial"));
    if (!SkyMat) return 0.0;
    double intensity = SkyMat->GetScalarParameterValue("OverallIntensity");
    return intensity;
}

// ��ȡ�Ʋ�͸����
UFUNCTION(BlueprintPure, Category = "Sky Material")
double AUltraDynamicSky::CurrentWispsOpacity(double Base)
{
    UMaterialInstanceDynamic* SkyMat = UGameplayStatics::CreateAndGetDynamicMaterialInstance(this, TEXT("SkyMaterial"));
    if (!SkyMat) return 0.0;
    double coeff = SkyMat->GetScalarParameterValue("WispsOpacityCoefficient");
    double opacity = Base * coeff;
    opacity = FMath::Clamp(opacity, 0.0, 1.0);
    return opacity;
}

// �����¼��ɷ���
void AUltraDynamicSky::FireEditorDispatchers()
{
    // ��������¼��������ȡʵ��
    UMyEventManager* Manager = UMyEventManager::GetInstance();
    if (Manager)
    {
        Manager->DispatchSunsetEvent();
        Manager->DispatchSunriseEvent();
        TArray<AActor*> Actors;
        Manager->GetBoundActors(Actors);
        for (AActor* Actor : Actors)
        {
            Actor->OnSunsetOrSunriseEvent();
        }
    }
}

// �����Ʋ�������
void AUltraDynamicSky::ScaleSampleCount(double in, double& out)
{
    int32 QualityLevel = GetGlobalMaterialQualityLevel();
    if (QualityLevel == 0)
    {
        out = in * 0.5;
    }
    else if (QualityLevel == 1)
    {
        out = in * 0.8;
    }
    else
    {
        out = in;
    }
    out = FMath::Max(out, 10.0);
}

// ��������������ɫ
void AUltraDynamicSky::SetCurrentFogBaseColors(double DesaturationAmount, FLinearColor LocalDustFogColor, FLinearColor LocalFogBrightnessMultiplier, FLinearColor LocalBaseDirectionalScatteringFogColor, double SunTwilightBrightnessFalloff, double MoonTwilightBrightnessFalloff)
{
    UStaticMeshComponent* FogComp = FindComponentByClass<UStaticMeshComponent>();
    if (!FogComp) return;
    FLinearColor adjColor = LocalDustFogColor;
    adjColor.Desaturate(DesaturationAmount);
    adjColor *= LocalFogBrightnessMultiplier;
    FogComp->SetVectorParameterValueOnMaterials("FogBaseColor", adjColor);
    FLinearColor dirColor = LocalBaseDirectionalScatteringFogColor;
    dirColor *= SunTwilightBrightnessFalloff;
    dirColor *= MoonTwilightBrightnessFalloff;
    FogComp->SetVectorParameterValueOnMaterials("DirectionalScatteringFogColor", dirColor);
}

// ��ȡ����ɢ����ɫ
void AUltraDynamicSky::CurrentFogInscatteringColor(FLinearColor& InscatteringColor, FLinearColor BaseColor)
{
    InscatteringColor = BaseColor;
    InscatteringColor += FLinearColor(0.1, 0.1, 0.1, 0.0);
    ULightComponent* MainLight = FindComponentByClass<ULightComponent>();
    if (MainLight)
    {
        float intensity = MainLight->Intensity;
        InscatteringColor *= intensity;
    }
    InscatteringColor.R = FMath::Clamp(InscatteringColor.R, 0.0f, 1.0f);
    InscatteringColor.G = FMath::Clamp(InscatteringColor.G, 0.0f, 1.0f);
    InscatteringColor.B = FMath::Clamp(InscatteringColor.B, 0.0f, 1.0f);
}

// ��ȡ��������ɢ����ɫ
void AUltraDynamicSky::CurrentFogDirectionalInscatteringColor(FLinearColor& DirectionalInscatteringColor, double BaseIntensity)
{
    FVector Coeffs(0.8, 0.6, 0.4);
    DirectionalInscatteringColor.R = BaseIntensity * Coeffs.X;
    DirectionalInscatteringColor.G = BaseIntensity * Coeffs.Y;
    DirectionalInscatteringColor.B = BaseIntensity * Coeffs.Z;
    DirectionalInscatteringColor.R = FMath::Clamp(DirectionalInscatteringColor.R, 0.0f, 1.0f);
    DirectionalInscatteringColor.G = FMath::Clamp(DirectionalInscatteringColor.G, 0.0f, 1.0f);
    DirectionalInscatteringColor.B = FMath::Clamp(DirectionalInscatteringColor.B, 0.0f, 1.0f);
}

// ��ȡ��ջ�����ɫ

void AUltraDynamicSky::CurrentSkyAmbientColor(FLinearColor& SkyAmbientColor)
{
    UMaterialInstanceDynamic* SkyMat = UGameplayStatics::CreateAndGetDynamicMaterialInstance(this, TEXT("SkyMaterial"));
    if (!SkyMat)
    {
        SkyAmbientColor = FLinearColor::Black;
        return;
    }
    SkyAmbientColor = SkyMat->GetVectorParameterValue("SkyAmbientColor");
}

// ��ȡ��ɢ����λ
double AUltraDynamicSky::CurrentVolumetricMultiscatteringPhase1()
{
    double phase = FMath::RandRange(0.0, 1.0);
    return phase;
}

// ��ȡ�Ʋ�����߶�
void AUltraDynamicSky::VolumetricCloudBaseCloudHeight(double& BaseCloudHeight)
{
    float coverage = FMath::RandRange(0.0f, 1.0f);
    BaseCloudHeight = 1000.0 + coverage * 500.0;
    BaseCloudHeight = FMath::Clamp(BaseCloudHeight, 500.0, 2000.0);
}

// ��ȡʵ���Ʋ�߶�
void AUltraDynamicSky::VolumetricCloudLayerHeight(double BaseCloudHeight, double& LayerHeight)
{
    LayerHeight = BaseCloudHeight * 0.8;
    LayerHeight = FMath::Clamp(LayerHeight, 400.0, 1600.0);
}

// ��ȡ��һ�㶥���θ߶�
void AUltraDynamicSky::VolumetricCloudFirstLayerTopAltitude(double& CloudTopAltitude)
{
    double baseHeight;
    VolumetricCloudBaseCloudHeight(baseHeight);
    CloudTopAltitude = baseHeight + 200.0;
    CloudTopAltitude = FMath::Clamp(CloudTopAltitude, 700.0, 2200.0);
}

void AUltraDynamicSky::VolumetricCloudLayerScale(double& LayerScale)
{
    LayerScale = 0.5;
}


void AUltraDynamicSky::SkyAtmosphereFogContribution(double& Output, double OvercastBrightnessMultiplier)
{
    Output = 0.3 * OvercastBrightnessMultiplier;
}

void AUltraDynamicSky::QueryProjectSettingsAndUDSVersion()
{
    FString Settings = GetProjectSettings();
    int32 UDSVersion = GetUDSVersion();
}