
#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "JoinSessionCallbackProxy.h"
#include "FindSessionsCallbackProxy.h"
#include "OnlineSubsystem.h"
#include "GameFramework/Character.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Weather/generate.h"

//环境参数
struct EnvParams
{
    float WeatherFactor;
    float TimeFactor;
    float ExposureFactor;
    float CloudCoverage;
    float DustFactor;
};

// 渲染天空，包括光照、动态云、雾霾等。
UCLASS(Blueprintable, BlueprintType, Category = "Ultra Dynamic Sky", HideCategories = ("Collision", "Input", "LOD", "Hidden Exposed", "Dynamic Material Instances", "Cache", "System", "Physics", "HLOD", "Networking", "Cooking", "Change Monitoring", "Hidden Cloud Movement", "Replication"))
class AUltraDynamicSky : public AActor
{
    GENERATED_BODY()

protected:
    // 根据白昼时间计算太阳和月亮的位置，并设置根组件的旋转
    UFUNCTION(BlueprintCallable, Category = "Directional Light")
    void SetSunAndMoonRootRotation(FVector realSunPosition, FVector realMoonPosition, double realMoonPhase, FVector moonAlignment, bool updatingSun, bool updatingMoon);

    // UDS在每一帧调用的更新函数
    UFUNCTION(BlueprintCallable, Category = "Update Sky")
    void TickFunction();

    // 更新动态变量的派生值，减少重复计算
    UFUNCTION(BlueprintCallable, Category = "Update Sky")
    void UpdateCommonDerivatives();

public:
    // 更新系统运行时需要的所有动态参数，如太阳光盘位置、雾霾颜色等
    UFUNCTION(BlueprintCallable, Category = "Update Sky")
    void UpdateActiveVariables();

    // 将时间转换为标准时间格式（时：分：秒）
    UFUNCTION(BlueprintPure, Category = "Utility Functions", meta = (Keywords = "timecode time seconds minutes hours frames"))
    void GetTimeCode(FTimecode& timeCode);

    UFUNCTION(BlueprintPure, Category = "Utility Functions", meta = (Keywords = "get datetime date time day month year hours minutes"))
    void GetDateTime(FDateTime& currentDateTime);

    // 返回太阳在地平线之上
    UFUNCTION(BlueprintPure, Category = "Utility Functions", meta = (Keywords = "day night sun up down horizon"))
    void IsDaytime(bool& yes);

    UFUNCTION(BlueprintPure, Category = "Utility Functions", meta = (Keywords = "day week monday tuesday wednesday thursday friday saturday sunday calendar date weekday weekend"))
    void GetDayOfTheWeek(int32& index, FString& name);

    UFUNCTION(BlueprintCallable, Category = "Utility Functions", meta = (Keywords = "timecode time seconds minutes hours frames"))
    void SetTimeWithTimeCode(FTimecode timeCode);

    UFUNCTION(BlueprintCallable, Category = "Update Sky")
    void UpdateStaticVariables();

protected:
    // 返回当前云层的速度（用于控制云层的移动）
    UFUNCTION(BlueprintPure, Category = "Cloud Movement")
    FVector CloudTextureVelocity();

    // 更新云层的当前位置和阶段
    UFUNCTION(BlueprintCallable, Category = "Cloud Movement")
    void UpdateCloudMovement(double cachedMovementAlpha);

    // 在构造脚本中调用此函数，但也可以手动调用它以重新初始化BP
    UFUNCTION(BlueprintCallable, Category = "Construction Script")
    void ConstructionScriptFunction();

    // 启动时检查可扩展性设置并根据需要调整设置
    UFUNCTION(BlueprintCallable, Category = "Scalability / Performance", meta = (Keywords = "effects quality scalability map game settings"))
    void UpdateSettingsBasedOnScalability(int32 effectsQuality, bool reconstruct);

    // 定期重新捕捉天空光照，启用了该选项
    UFUNCTION(BlueprintCallable, Category = "Sky Light")
    void RecaptureSkyLight();

    // 应用太阳和月亮的计算向量到实际的方向光旋转
    UFUNCTION(BlueprintCallable, Category = "Directional Light")
    void UpdateDirectionalLightRotations();

public:
    // 使用日期时间结构体设置时间和日期
    UFUNCTION(BlueprintCallable, Category = "Utility Functions", meta = (Keywords = "datetime date time day month year time set hours minutes"))
    void SetDateAndTime(FDateTime dateTime);

    // 根据模拟的时间、日期、纬度和经度计算太阳、月亮和星星的朝向
    UFUNCTION(BlueprintCallable, Category = "Simulation")
    void ApproximateRealSunMoonAndStars(double timeOfDay, bool onlyCalculateSun, FVector& sunVector, FVector& moonVector, double& realPhase, FVector& phaseAlignment, FLinearColor& celestialYaw, double& celestialOrbit, double utcTimeOfDay, double utcTimeOfYear, int32 utcMonth, int32 utcDay, int32 dayOfYearIndex, double seasonalTime, FRotator localGeocoordinate, FVector viewerPositionOnEarth, double axialTilt, FRotator zenithDirection, int32 utcYear, FVector sunDirection, FLinearColor starsYaw, double starsOrbit, FVector celestialNorthPole, FVector moonDirection, FVector moonAlignment, double moonRealPhase, FVector earthCenter, double lunarTime, FVector moonCenter, double lunarInclinationCycle, UCurveFloat equationOfTimeCurve, double equationOfTimeOffset);

    // 确保日期有效性，设置了不合法的日期（如12月50日），会自动修正为合法日期
    UFUNCTION(BlueprintCallable, Category = "Date And Time")
    void ForceValidDay();

    // 将当前日期偏移指定的天数，通常在午夜时调用，用于日期更替
    UFUNCTION(BlueprintCallable, Category = "Date And Time")
    void OffsetDate(int32 offset);

    // 检查应启用夏令时
    UFUNCTION(BlueprintCallable, Category = "Simulation")
    void CheckForDaylightSavingsTime(int32 hour, bool dstActive);

    // 请为该函数添加描述
    UFUNCTION(BlueprintCallable)
    void OnRep_ReplicatedTimeOfDay();

    // 服务器端更新复制的时间值，供客户端使用
    UFUNCTION(BlueprintCallable, Category = "Replication")
    void UpdateReplicatedTime();

    // 输出动态系统的状态（如时间、天气等），以便保存到文件或跨关卡传递
    UFUNCTION(BlueprintPure, Category = "Saving and Loading", meta = (Keywords = "save get state"))
    void CreateUDSandUDWStateForSaving(FUDSAndUDWState& packagedState, FUDSAndUDWState state);

    // 使用已保存的状态更新UDS和UDW的状态
    UFUNCTION(BlueprintCallable, Category = "Saving And Loading", meta = (Keywords = "load"))
    void ApplySavedUDSandUDWState(FUDSAndUDWState state);

protected:
    // 计算准确的模拟日落和日出时间
    UFUNCTION(BlueprintCallable, Category = "Simulation")
    void FindRealSunsetSunriseTimes(double sunriseTime, int32 currentFactor, double sunsetTime);

    // 设置由体积云渲染模式控制的CVar
    UFUNCTION(BlueprintCallable, Category = "Volumetric Clouds")
    void ApplyVolumetricMode(TEnumAsByte<UDSVolRTMode> mode);

    UFUNCTION(BlueprintCallable, Category = "Lens Flare")
    void ConstructLensFlare();

    UFUNCTION(BlueprintCallable, Category = "Lens Flare")
    void UpdateLensFlare(bool allowLensFlare);

public:
    UFUNCTION(BlueprintCallable, Category = "Utility Functions", meta = (Keywords = "animate transition time of day hours minutes pass time interpolate"))
    void TransitionTimeOfDay(double newTimeOfDay, double transitionDurationSeconds, TEnumAsByte<EEasingFunc::Type> easingFunction, double easingExponent, bool allowTimeGoingBackwards);

    UFUNCTION(BlueprintCallable, Category = "Sky Light")
    void TransitionSkyLightIntensity(double newIntensity, double transitionDurationSeconds, TEnumAsByte<EEasingFunc::Type> easingFunction, double easingExponent);

    UFUNCTION(BlueprintPure, Category = "𝙴𝟸 · Moon")
    double CurrentMoonLitPercent();

    UFUNCTION(BlueprintPure, Category = "𝙴𝟸 · Moon")
    FLinearColor CurrentMoonLightMaterialColor();

    // 根据当前天空光模式，确定天空光下半球的颜色
    UFUNCTION(BlueprintPure, Category = "𝙴𝟹 · Sky Light")
    FLinearColor CurrentSkyLightLowerHemisphereColor();

    // 根据当前天空光模式，确定天空光的颜色和强度
    UFUNCTION(BlueprintCallable, Category = "𝙴𝟹 · Sky Light")
    void GetCurrentSkyLightColorAndIntensity(double& OutIntensity, FLinearColor& OutColor, double IntensityOutput, FLinearColor ColorOutput, FLinearColor CurrentMultiplierColor, double LuminanceIntensityBalance, FLinearColor BaseColor, FLinearColor TemperatureTint, double BaseIntensity);

    // 根据云层覆盖度调整Mie各向异性散射值
    UFUNCTION(BlueprintPure, Category = "𝙶𝟷 · Sky Atmosphere")
    double CurrentMieAnisotropy();

    // 根据云层覆盖度和雾霾影响调整Mie散射比例
    UFUNCTION(BlueprintPure, Category = "𝙶𝟷 · Sky Atmosphere")
    double CurrentMieScatteringScale(double MaxCloudCoverageLevel, double MaxUnscaledScattering, double FinalScale);

    // 根据云层覆盖度和暴露的设置去调整雷利散射颜色
    UFUNCTION(BlueprintPure, Category = "𝙶𝟷 · Sky Atmosphere")
    FLinearColor CurrentRayleighScatteringColor(FLinearColor BaseColor);

    // 获取星星的颜色，依据星星强度进行调整
    UFUNCTION(BlueprintPure, Category = "𝙷𝟷 · Stars")
    FLinearColor CurrentStarsColor(double AppliedIntensity);

    // 获取夜空辉光的颜色，用于天空材质和云彩
    UFUNCTION(BlueprintPure, Category = "𝙷𝟹 · Sky Glow")
    FLinearColor CurrentNightSkyGlow();

    // 获取极光当前的强度，可以是2D或体积极光
    UFUNCTION(BlueprintPure, Category = "𝙷𝟺 · Aurora")
    double CurrentAuroraIntensity();

    // 获取当前光污染的颜色，依据光污染设置和时间调整
    UFUNCTION(BlueprintPure, Category = "𝙷𝟹 · Sky Glow")
    FLinearColor CurrentLightPollution();

    // 云层覆盖度较高时，将定向光强度降低到0
    UFUNCTION(BlueprintPure, Category = "𝙴𝟺 · Directional Light")
    bool DimmingDirectionalLights();

    // 根据月亮相位调整月亮亮度倍数
    UFUNCTION(BlueprintPure, Category = "𝙴𝟸 · Moon")
    double MoonPhaseLightMultiplier(double& WithoutLightBrightness);

    // 用于在月亮和太阳同时缺席时，增加天空亮度
    UFUNCTION(BlueprintPure, Category = "𝙴𝟺 · Directional Light")
    double AbsentDirectionalLightsBrightness();

    // 在夜间返回1，白天返回0，黎明和黄昏时渐变
    UFUNCTION(BlueprintPure, Category = "Time")
    double NightFilter(bool Cached);

    // 根据云层覆盖度调整2D云层的色调
    UFUNCTION(BlueprintPure, Category = "𝙲𝟸 · 2D Dynamic Clouds")
    FLinearColor Current2DCloudTint();

    // 每帧推进时间偏移，若启用了“Animate Time of Day”
    UFUNCTION(BlueprintPure, Category = "Time")
    void TimeOfDayOffset(double& PerSecond);

    // 获取自游戏开始以来经过的总时间
    UFUNCTION(BlueprintPure, Category = "Time")
    double TotalTimeElapsed();

    // “Clouds Move with Time of Day”开启，返回根据当前时间应偏移的云层移动量
    UFUNCTION(BlueprintPure, Category = "𝙲𝟺 · Cloud Movement")
    double CloudsTimeOfDayFactor();

    // 获取太阳升起事件的时间
    UFUNCTION(BlueprintPure, Category = "Time")
    double CurrentSunriseEventTime();

    // 获取太阳落山事件的时间
    UFUNCTION(BlueprintPure, Category = "Time")
    double CurrentSunsetEventTime();

    // 获取当前适用的天空材质实例的软引用，用作动态材质实例的父材质
    UFUNCTION(BlueprintPure, Category = "Sky Material")
    TSoftObjectPtr<UMaterialInterface> GetSkyMIDParentMaterialInstance();

    // 获取当前高度雾霾组件的雾霾密度，依据天气、时间和暴露的设置
    UFUNCTION(BlueprintPure, Category = "𝙵𝟸 · Fog Density")
    double CurrentFogDensity();

    // 获取高度雾霾组件的高度衰减值，依据雾霾、云层覆盖度和尘土调整
    UFUNCTION(BlueprintPure, Category = "𝙵𝟸 · Fog Density")
    double FogHeightFalloff();

    // 使用当前的雾霾密度，计算雾霾的起始距离
    UFUNCTION(BlueprintPure, Category = "𝙵𝟸 · Fog Density")
    double FogStartDistance();

    // 获取天空材质中，影响亮度的基础颜色、云层和2D动态云的亮度。夜间元素（如月亮、星星、极光）除外
    UFUNCTION(BlueprintPure, Category = "Sky Material")
    double CurrentLitIntensity();

    // 获取天空材质的整体强度，用于调节天空材质的强度
    UFUNCTION(BlueprintPure, Category = "Sky Material")
    double CurrentOverallIntensity();

    // 获取云层的透明度，用于天空材质中的云朵效果
    UFUNCTION(BlueprintPure, Category = "Sky Material")
    double CurrentWispsOpacity(double Base);

    // 在编辑器中调用日落和日出事件派发器，使在构造脚本中绑定的所有角色能够在编辑器中受到影响
    UFUNCTION(BlueprintCallable, Category = "Event Dispatching")
    void FireEditorDispatchers();

    // 根据全局材质质量水平调整体积云采样数量
    UFUNCTION(BlueprintPure, Category = "𝙲𝟷 · Volumetric Clouds")
    void ScaleSampleCount(double in, double& out);

    // 设置当前雾霾的基础颜色，用于雾霾散射、定向散射以及云层的颜色调整
    UFUNCTION(BlueprintCallable, Category = "𝙵𝟷 · Fog Color")
    void SetCurrentFogBaseColors(double DesaturationAmount, FLinearColor LocalDustFogColor, FLinearColor LocalFogBrightnessMultiplier, FLinearColor LocalBaseDirectionalScatteringFogColor, double SunTwilightBrightnessFalloff, double MoonTwilightBrightnessFalloff);

    // 获取当前的雾霾散射颜色，用于高度雾霾组件
    UFUNCTION(BlueprintPure, Category = "𝙵𝟷 · Fog Color")
    void CurrentFogInscatteringColor(FLinearColor& InscatteringColor, FLinearColor BaseColor);

    // 获取当前的定向雾霾散射颜色，用于高度雾霾组件
    UFUNCTION(BlueprintPure, Category = "𝙵𝟷 · Fog Color")
    void CurrentFogDirectionalInscatteringColor(FLinearColor& DirectionalInscatteringColor, double BaseIntensity);

    // 获取天空材质中，影响云层颜色的天空环境颜色
    UFUNCTION(BlueprintPure, Category = "Sky Material")
    void CurrentSkyAmbientColor(FLinearColor& SkyAmbientColor);

    // 获取体积云的第一个散射相位
    UFUNCTION(BlueprintPure, Category = "𝙲𝟷 · Volumetric Clouds")
    double CurrentVolumetricMultiscatteringPhase1();

    // 获取云层的基础高度，依据云层覆盖度调整
    UFUNCTION(BlueprintPure, Category = "𝙲𝟷 · Volumetric Clouds")
    void VolumetricCloudBaseCloudHeight(double& BaseCloudHeight);

    // 获取体积云实际的云层高度，若使用两层云层
    UFUNCTION(BlueprintPure, Category = "𝙲𝟷 · Volumetric Clouds")
    void VolumetricCloudLayerHeight(double BaseCloudHeight, double& LayerHeight);

    // 获取当前云层第一层的顶部海拔高度，用于云雾后处理效果
    UFUNCTION(BlueprintPure, Category = "𝙲𝟷 · Volumetric Clouds")
    void VolumetricCloudFirstLayerTopAltitude(double& CloudTopAltitude);

    // 用于体积云材质中，调整规范化的高度范围，以支持双层云层
    UFUNCTION(BlueprintPure, Category = "𝙲𝟷 · Volumetric Clouds")
    void VolumetricCloudLayerScale(double& LayerScale);

    // 获取天空大气成分
    UFUNCTION(BlueprintPure, Category = "𝙵𝟷 · Fog Color")
    void SkyAtmosphereFogContribution(double& Output, double OvercastBrightnessMultiplier);

    // 查询项目设置和UDS版本
    UFUNCTION(BlueprintCallable, Category = "Construct")
    void QueryProjectSettingsAndUDSVersion();

    // 用于根据云层覆盖度和雾霾调整定向光强度
    UFUNCTION(BlueprintPure, Category = "𝙴𝟺 · Directional Light")
    void DirectionalInscatteringMultiplier(double& Multiplier);

    // 获取当前月光的颜色，基于原始色和着色色进行计算
    UFUNCTION(BlueprintPure, Category = "𝙴𝟸  ·  Moon")
    void CurrentMoonLightColor(FLinearColor& Color, FLinearColor BaseColor, FLinearColor TintedColor);

    // 应用控制台变量，仅在必要时应用，以避免重复设置已经设置的变量
    UFUNCTION(BlueprintCallable, Category = "System Functions")
    void ApplyConsoleVariableWithCheck(FString Cvar, double Setting, int32 Type, bool ExistingBool, float ExistingFloat, int32 ExistingInt);

    // 生成云层阴影的光向量和取消值
    UFUNCTION(BlueprintCallable, Category = "𝙴𝟵  ·  Cloud Shadows")
    void CloudShadowsLightVectorAndCancelValue(FLinearColor& Vector, double& CancelValue, FVector ShadowsVector, double TransitionMultiplier);

    // 计算太阳或月亮的方向光源角度
    UFUNCTION(BlueprintPure, Category = "𝙴𝟷  ·  Sun")
    void DirectionalSourceAngle(double DiskScale, double MaxScaleFactor, double ScaleSetting, double& Out);

    // 计算当前体积云层的最大追踪距离，取决于摄像机在云层中的位置
    UFUNCTION(BlueprintPure, Category = "𝙲𝟷  ·  Volumetric Clouds")
    void CurrentMaxTraceDistance(double& Out);

    // 需要，更新体积云层的材质实例为简化版或复杂版
    UFUNCTION(BlueprintCallable, Category = "𝙲𝟷  ·  Volumetric Clouds")
    void UpdateCurrentVolumetricCloudsMID();

    // 更新覆盖层扰动平面的材质参数和可见性
    UFUNCTION(BlueprintCallable, Category = "𝙲𝟷  ·  Volumetric Clouds")
    void UpdateOvercastTurbulence(double TurbulenceStrength);

    // 启用，创建体积云使用的动态材质实例
    UFUNCTION(BlueprintCallable, Category = "Construction Script")
    void ConstructOvercastTurbulence();

    // 计算太阳和月亮的雾霾颜色在地平线以下时的变化
    UFUNCTION(BlueprintPure, Category = "𝙵𝟷  ·  Fog Color")
    void TwilightBrightnessFalloff(double Z, double& Scale);

    // 加载系统所需的所有软对象引用
    UFUNCTION(BlueprintCallable, Category = "Loading")
    void LoadRequiredAssets();

    // 查找云层阴影光函数动态材质实例的父材质软引用
    UFUNCTION(BlueprintPure, Category = "𝙴𝟵  ·  Cloud Shadows")
    void CloudShadowsParentMaterial(TSoftObjectPtr<UMaterialInterface>& Mat);

    // 查找选定镜头光晕类型的材质软引用
    UFUNCTION(BlueprintPure, Category = "𝙸𝟸  ·  Lens Flare")
    void LensFlareParentMaterial(TSoftObjectPtr<UMaterialInterface>& Mat);

    // 根据云层覆盖度将方向光强度减小至0，启用此设置
    UFUNCTION(BlueprintPure, Category = "𝙴𝟺  ·  Directional Light")
    void DirectionalLightDimming(double& Out);

    // 将时间过渡的过渡时间设置为false，仅在计时器用于过渡系统时使用
    UFUNCTION(BlueprintCallable, Category = "Time Transition")
    void FinishTimeTransition();

    // 使用过渡计时器设置一天中的时间，并根据需要递增日期
    UFUNCTION(BlueprintCallable, Category = "Time Transition")
    void TickTimeTransition(double NewTimeOfDay);

    // 启动玩家遮蔽组件，启用了室内调整
    UFUNCTION(BlueprintCallable, Category = "𝙽𝟷  ·  Interior Adjustments")
    void InitializeOcclusion();

    // 使用方向平衡和散射倍率过滤月光强度
    UFUNCTION(BlueprintPure, Category = "𝙴𝟸  ·  Moon")
    void FilteredMoonLightIntensity(double Unfiltered, double& Intensity);

    // 调整基本太阳光强度，基于太阳向量
    UFUNCTION(BlueprintPure, Category = "𝙴𝟷  ·  Sun")
    void AdjustBaseSunLightIntensity(FVector SunVector, double& Intensity, double& Multiplier);

    // 计算月亮或太空行星遮挡太阳的程度，用于日全食效果
    UFUNCTION(BlueprintPure, Category = "𝙴𝟷  ·  Sun")
    void CurrentSolarEclipseValues(double& EclipsePercent, FLinearColor& TintColor, double SunFractionShowing, double SunAngularDiameter, FUDS_Space_Planet SpacePlanet, FRotator SunRootRotation, FRotator MoonRootRotation);

    // 将缓存的太阳向量转换为0到1之间的浮动值，其中0表示正上方，1表示正下方
    UFUNCTION(BlueprintPure, Category = "𝙴𝟷  ·  Sun")
    void SunHeight(bool Cached, double& Z);

    // 强制完全更新属性缓存
    UFUNCTION(BlueprintCallable, Category = "Cache")
    void HardResetCache();

    // 获取通过缓存属性保存的浮动属性值
    UFUNCTION(BlueprintCallable, Category = "Cache")
    void GetCachedFloat(TEnumAsByte<UDS_CachedProperties> Property, double& Value);

    // 定期缓存动态属性，包括组件设置、材质参数、光照设置等
    UFUNCTION(BlueprintCallable, Category = "Cache")
    void CacheProperties(int32 CacheGroup, bool StartingCacheFill, int32 CurrentCacheGroup);

    // 缓存一个浮动属性，稍后通过GetCachedFloat重新调用
    UFUNCTION(BlueprintCallable, Category = "Cache")
    void CacheFloat(TEnumAsByte<UDS_CachedProperties> Property, double SetValue, double ChangeTolerance, int32 Index);

    // 缓存一个颜色属性，稍后通过GetCachedColor重新调用
    UFUNCTION(BlueprintCallable, Category = "Cache")
    void CacheColor(TEnumAsByte<UDS_CachedProperties> Property, FLinearColor SetValue, double ChangeTolerance, int32 Index);

    // 获取缓存的颜色属性
    UFUNCTION(BlueprintCallable, Category = "Cache")
    void GetCachedColor(TEnumAsByte<UDS_CachedProperties> Property, FLinearColor& Value);

};
