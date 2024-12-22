#include "UltraDynamicSky.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"



// 设置太阳和月亮的根组件旋转
void AUltraDynamicSky::SetSunAndMoonRootRotation(FVector realSunPosition, FVector realMoonPosition, double realMoonPhase, FVector moonAlignment, bool updatingSun, bool updatingMoon)
{
    // 设置太阳位置，基于真实位置计算太阳方向光的旋转
    if (updatingSun)
    {
        FRotator SunRotation = FRotator::MakeFromEuler(realSunPosition);
        GetRootComponent()->SetWorldRotation(SunRotation);
    }
    // 设置月亮位置，基于真实位置和月相计算月亮方向光的旋转
    if (updatingMoon)
    {
        FRotator MoonRotation = FRotator::MakeFromEuler(realMoonPosition);
        MoonRotation.Pitch += realMoonPhase * 10.0f; // 模拟月相的影响
        GetRootComponent()->SetWorldRotation(MoonRotation);
    }
    // 根据月亮对齐向量进一步调整
    if (moonAlignment != FVector::ZeroVector)
    {
        FRotator AlignmentRotation = FRotator::MakeFromEuler(moonAlignment);
        GetRootComponent()->AddWorldRotation(AlignmentRotation);
    }
}

// UDS在每一帧调用的更新函数
void AUltraDynamicSky::TickFunction()
{
    // 更新动态变量的派生值
    UpdateCommonDerivatives();
    // 动态更新云层位置
    UpdateCloudMovement(0.1f); // 每帧让云移动少量距离
    // 更新光照方向
    UpdateDirectionalLightRotations();
    // 启用了天空捕捉，则进行重新捕捉
    RecaptureSkyLight();
    // 确保时间是合法的
    ForceValidDay();
    // 检查需要更新日期偏移
    OffsetDate(1);
}

// 更新动态变量的派生值，减少重复计算
void AUltraDynamicSky::UpdateCommonDerivatives()
{
    // 计算太阳的实时方向和颜色
    FVector SunDirection = FVector(1.0f, 0.0f, 0.0f); // 示例向量
    SunDirection.Normalize();
    // 模拟太阳光线的扩散，影响天空颜色
    FLinearColor SunColor = FLinearColor::MakeRandomColor();
    SunColor *= FMath::Clamp(SunDirection.Z, 0.0f, 1.0f); // 高度影响颜色
    // 计算雾霾的颜色，动态调整
    FLinearColor FogColor = FLinearColor(0.5f, 0.5f, 0.6f, 1.0f);
    FogColor.R += 0.01f; // 每次稍微增加红色偏移
}

// 更新系统运行时需要的所有动态参数
void AUltraDynamicSky::UpdateActiveVariables()
{
    // 更新太阳和月亮的位置
    FVector SunPosition = FVector(100.0f, 200.0f, 300.0f);
    FVector MoonPosition = FVector(-100.0f, -200.0f, -300.0f);
    double MoonPhase = 0.5f; // 半月示例
    FVector MoonAlignment = FVector(0.0f, 1.0f, 0.0f);
    SetSunAndMoonRootRotation(SunPosition, MoonPosition, MoonPhase, MoonAlignment, true, true);
    // 更新天空光照参数
    UpdateDirectionalLightRotations();
    // 调整云层的动态速度
    FVector CloudVelocity = CloudTextureVelocity();
    CloudVelocity.X += 10.0f; // 模拟更快的移动速度
}

// 获取时间码
void AUltraDynamicSky::GetTimeCode(FTimecode& timeCode)
{
    // 当前时间是模拟时间的某个值
    timeCode.Hours = 12; // 示例时间，设置为12小时
    timeCode.Minutes = 30;
    timeCode.Seconds = 15;
    timeCode.Frames = 25; // 每秒25帧
    // 输出调试信息
    UE_LOG(LogTemp, Warning, TEXT("TimeCode: %02d:%02d:%02d:%02d"), timeCode.Hours, timeCode.Minutes, timeCode.Seconds, timeCode.Frames);
}

// 使用当前时间信息构造日期时间结构体
void AUltraDynamicSky::GetDateTime(FDateTime& currentDateTime)
{
    // 当前日期为2024年12月21日
    currentDateTime = FDateTime(2024, 12, 21, 12, 0, 0);
    // 打印调试信息
    UE_LOG(LogTemp, Warning, TEXT("Current DateTime: %s"), *currentDateTime.ToString());
}

// 检查太阳在地平线上
void AUltraDynamicSky::IsDaytime(bool& yes)
{
    // 根据太阳的高度值判断为白昼
    FVector SunPosition = FVector(0.0f, 0.0f, 1.0f); // 示例太阳位置
    yes = SunPosition.Z > 0.0f; // Z轴大于0表示在地平线上方
    if (yes)
    {
        UE_LOG(LogTemp, Warning, TEXT("It is daytime."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("It is nighttime."));
    }
}

// 设置时间码
void AUltraDynamicSky::SetTimeWithTimeCode(FTimecode timeCode)
{
    // 将时间码转换为内部时间
    double TotalSeconds = timeCode.Hours * 3600.0 + timeCode.Minutes * 60.0 + timeCode.Seconds + timeCode.Frames / 25.0; // 25帧每秒
    UE_LOG(LogTemp, Warning, TEXT("Set Time: %f seconds since midnight."), TotalSeconds);
    // 更新动态参数
    UpdateActiveVariables();
}

// 更新静态变量
void AUltraDynamicSky::UpdateStaticVariables()
{
    // 天空颜色为蓝色
    FLinearColor SkyColor = FLinearColor(0.2f, 0.4f, 0.8f, 1.0f);
    UE_LOG(LogTemp, Warning, TEXT("Updated static sky color to %s"), *SkyColor.ToString());
}

// 获取云层移动速度
FVector AUltraDynamicSky::CloudTextureVelocity()
{
    // 返回一个的云层移动速度
    FVector Velocity = FVector(10.0f, 5.0f, 0.0f); // 云层移动速度向量
    UE_LOG(LogTemp, Warning, TEXT("Cloud Velocity: %s"), *Velocity.ToString());
    return Velocity;
}

// 更新云层位置
void AUltraDynamicSky::UpdateCloudMovement(double cachedMovementAlpha)
{
    // 模拟云层的移动
    FVector CloudPosition = FVector(100.0f, 200.0f, 300.0f);
    CloudPosition.X += cachedMovementAlpha * 50.0f; // 缓存值影响移动
    UE_LOG(LogTemp, Warning, TEXT("Updated Cloud Position to %s"), *CloudPosition.ToString());
}

// 在构造脚本中调用的函数
void AUltraDynamicSky::ConstructionScriptFunction()
{
    // 初始化静态天空变量
    UpdateStaticVariables();
    // 调用动态参数更新
    UpdateActiveVariables();
    // 打印调试日志
    UE_LOG(LogTemp, Warning, TEXT("Construction script executed."));
}

// 重新捕捉天空光照
void AUltraDynamicSky::RecaptureSkyLight()
{
    // 模拟重新捕捉天空光照
    UE_LOG(LogTemp, Warning, TEXT("Recapturing skylight."));
}
// 确保日期合法性，设置了非法日期，自动修正到有效范围
void AUltraDynamicSky::ForceValidDay()
{
    // 检查当前的日期时间合法
    FDateTime CurrentDate = FDateTime::Now();
    if (!CurrentDate.IsValid())
    {
        // 无效，重置为默认值
        CurrentDate = FDateTime(2024, 1, 1, 0, 0, 0); // 默认设置为2024年1月1日
        UE_LOG(LogTemp, Warning, TEXT("Invalid date detected. Resetting to default date: %s"), *CurrentDate.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Date is valid: %s"), *CurrentDate.ToString());
    }
}

// 日期偏移函数，根据输入天数调整当前日期
void AUltraDynamicSky::OffsetDate(int32 DaysOffset)
{
    // 获取当前日期
    FDateTime CurrentDate = FDateTime::Now();
    // 计算偏移后的日期
    FDateTime OffsetDate = CurrentDate + FTimespan(DaysOffset, 0, 0, 0);
    UE_LOG(LogTemp, Warning, TEXT("Offsetting date by %d days. New date: %s"), DaysOffset, *OffsetDate.ToString());
    // 更新天空的动态参数
    UpdateStaticVariables();
}

// 更新太阳和月亮方向光的旋转
void AUltraDynamicSky::UpdateDirectionalLightRotations()
{
    // 模拟太阳的旋转更新
    FVector SunVector = FVector(1.0f, 0.0f, 1.0f).GetSafeNormal();
    FRotator SunRotation = FRotator::MakeFromEuler(SunVector);
    GetRootComponent()->SetWorldRotation(SunRotation);

    // 模拟月亮的旋转更新
    FVector MoonVector = FVector(-1.0f, 0.0f, 0.5f).GetSafeNormal();
    FRotator MoonRotation = FRotator::MakeFromEuler(MoonVector);
    MoonRotation.Pitch += 5.0f; // 有微小调整
    GetRootComponent()->AddWorldRotation(MoonRotation);

    UE_LOG(LogTemp, Warning, TEXT("Updated sun and moon directional light rotations."));
}

// 计算太阳和月亮的真实位置，并更新其向量
void AUltraDynamicSky::ApproximateRealSunMoonAndStars(double timeOfDay, bool onlyCalculateSun, FVector& sunVector, FVector& moonVector, double& realPhase, FVector& phaseAlignment, FLinearColor& celestialYaw, double& celestialOrbit, double utcTimeOfDay, double utcTimeOfYear, int32 utcMonth, int32 utcDay, int32 dayOfYearIndex, double seasonalTime, FRotator localGeocoordinate, FVector viewerPositionOnEarth, double axialTilt, FRotator zenithDirection, int32 utcYear, FVector sunDirection, FLinearColor starsYaw, double starsOrbit, FVector celestialNorthPole, FVector moonDirection, FVector moonAlignment, double moonRealPhase, FVector earthCenter, double lunarTime, FVector moonCenter, double lunarInclinationCycle, UCurveFloat equationOfTimeCurve, double equationOfTimeOffset)
{
    // 计算太阳位置
    sunVector = FVector(FMath::Cos(timeOfDay), FMath::Sin(timeOfDay), FMath::Cos(axialTilt));
    sunVector.Normalize();

    // 需要计算月亮，则进行模拟
    if (!onlyCalculateSun)
    {
        moonVector = FVector(FMath::Cos(timeOfDay * 0.5), FMath::Sin(timeOfDay * 0.5), FMath::Cos(axialTilt + 10.0f));
        moonVector.Normalize();
        realPhase = FMath::Abs(FMath::Sin(timeOfDay)); // 模拟月相
    }

    // 计算天体朝向
    celestialYaw = FLinearColor(1.0f, 0.0f, 0.0f); // 示例颜色
    celestialOrbit = FMath::Fmod(timeOfDay * 360.0f, 360.0f); // 简单的天体轨道模拟

    UE_LOG(LogTemp, Warning, TEXT("SunVector: %s, MoonVector: %s, RealPhase: %f"), *sunVector.ToString(), *moonVector.ToString(), realPhase);
}

// 设置日期和时间
void AUltraDynamicSky::SetDateAndTime(FDateTime dateTime)
{
    // 更新当前时间
    FDateTime CurrentDateTime = dateTime;
    UE_LOG(LogTemp, Warning, TEXT("DateTime set to: %s"), *CurrentDateTime.ToString());

    // 更新动态天空参数
    UpdateActiveVariables();
}

// 基于可扩展性调整设置
void AUltraDynamicSky::UpdateSettingsBasedOnScalability(int32 effectsQuality, bool reconstruct)
{
    // 根据效果质量调整动态云层分辨率
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

    // 需要重新构造天空设置
    if (reconstruct)
    {
        UE_LOG(LogTemp, Warning, TEXT("Reconstructing sky with updated scalability settings."));
        ConstructionScriptFunction();
    }
}

// 模拟星星的轨迹更新
void AUltraDynamicSky::UpdateStarMovements()
{
    // 星星以固定速度围绕天空旋转
    double StarOrbitSpeed = 0.1f; // 每秒旋转速度
    FRotator StarRotation = FRotator(0.0f, StarOrbitSpeed, 0.0f);
    GetRootComponent()->AddWorldRotation(StarRotation);

    UE_LOG(LogTemp, Warning, TEXT("Updated star movements with speed: %f"), StarOrbitSpeed);
}

// 动态雾霾更新
void AUltraDynamicSky::UpdateFogColor()
{
    // 计算当前雾霾颜色
    FLinearColor CurrentFogColor = FLinearColor(0.8f, 0.7f, 0.6f);
    CurrentFogColor.G -= 0.01f; // 动态减弱绿色分量，模拟渐变效果
    CurrentFogColor.R += 0.02f; // 增强红色分量，模拟日落
    UE_LOG(LogTemp, Warning, TEXT("Fog color updated to: %s"), *CurrentFogColor.ToString());
}

// 捕获并更新反射天空盒
void AUltraDynamicSky::CaptureReflectionSkybox()
{
    // 模拟捕获天空反射
    UE_LOG(LogTemp, Warning, TEXT("Capturing reflection skybox."));
    UpdateStaticVariables(); // 同时更新静态变量
}

// 动态风速变化，用于影响云层和雾霾
void AUltraDynamicSky::UpdateWindSpeed()
{
    // 模拟风速的动态变化
    FVector WindVelocity = FVector(FMath::RandRange(-10.0f, 10.0f), FMath::RandRange(-5.0f, 5.0f), 0.0f);
    UE_LOG(LogTemp, Warning, TEXT("Wind velocity updated to: %s"), *WindVelocity.ToString());
}


// 获取雾霾密度
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

// 获取高度衰减值
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

// 计算起始距离
double AUltraDynamicSky::FogStartDistance()
{
    double density = CurrentFogDensity();
    double distance = 1000.0 / (density + 0.1);
    distance = FMath::Clamp(distance, 100.0, 2000.0);
    return distance;
}

// 获取亮度强度
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

// 获取整体强度
UFUNCTION(BlueprintPure, Category = "Sky Material")
double AUltraDynamicSky::CurrentOverallIntensity()
{
    UMaterialInstanceDynamic* SkyMat = UGameplayStatics::CreateAndGetDynamicMaterialInstance(this, TEXT("SkyMaterial"));
    if (!SkyMat) return 0.0;
    double intensity = SkyMat->GetScalarParameterValue("OverallIntensity");
    return intensity;
}

// 获取云层透明度
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

// 调用事件派发器
void AUltraDynamicSky::FireEditorDispatchers()
{
    // 假设存在事件管理类获取实例
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

// 调整云采样数量
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

// 设置雾霾基础颜色
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

// 获取雾霾散射颜色
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

// 获取定向雾霾散射颜色
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

// 获取天空环境颜色

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

// 获取云散射相位
double AUltraDynamicSky::CurrentVolumetricMultiscatteringPhase1()
{
    double phase = FMath::RandRange(0.0, 1.0);
    return phase;
}

// 获取云层基础高度
void AUltraDynamicSky::VolumetricCloudBaseCloudHeight(double& BaseCloudHeight)
{
    float coverage = FMath::RandRange(0.0f, 1.0f);
    BaseCloudHeight = 1000.0 + coverage * 500.0;
    BaseCloudHeight = FMath::Clamp(BaseCloudHeight, 500.0, 2000.0);
}

// 获取实际云层高度
void AUltraDynamicSky::VolumetricCloudLayerHeight(double BaseCloudHeight, double& LayerHeight)
{
    LayerHeight = BaseCloudHeight * 0.8;
    LayerHeight = FMath::Clamp(LayerHeight, 400.0, 1600.0);
}

// 获取第一层顶海拔高度
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