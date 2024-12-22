#pragma once

#include "CoreMinimal.h"
#include "playerCharacter.h"
#include "PersonalProperty.h"
#include "Factions.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "AIController.h"
#include "PlayerBaseCharacter.generated.h"

//声明两个类
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
class UCharacterAnimInstance;
class ABaseWeapon;
class AThrowWeapon;
class ABowWeapon;
class AMeleeWeapon;
UCLASS()
class PALUWORLD_API APlayerBaseCharacter : public AplayerCharacter
{
    GENERATED_BODY()

public:
    APlayerBaseCharacter();

    virtual void Tick(float deletatime) override;


protected:

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void BeginPlay() override;

    // 摄像机相关组件
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera");
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera");
    UCameraComponent* FollowCamera;

    // 输入相关属性
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    UInputMappingContext* DefaultMappingConext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    UInputAction* MoveForwardAction;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    UInputAction* MoveRightAction;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    UInputAction* JumpAction;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    UInputAction* LookAction;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    UInputAction* SprintAction;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    UInputAction* ShiftKeyAction;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    UInputAction* CrouchAction;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    UInputAction* AimAction;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    UInputAction* AttackAction;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    UInputAction* AimStopAction;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    UInputAction* AttackStopAction;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    UInputAction* RollAction;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    UInputAction* ClimbAction;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    UInputAction* ClimbMoveAction;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    UInputAction* ClimbStopAction;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    UInputAction* ChangeWeapon1Action;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    UInputAction* ChangeWeapon2Action;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    UInputAction* ChangeWeapon3Action;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    UInputAction* ChangeWeapon4Action;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    UInputAction* ThrowBallAction;
    // 记录角色上次移动结束时的朝向
    FRotator LastMovementEndRotation;
    //记录己方的阵营标签
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Faction");
    Faction faction = Faction::FRIEND;


    

    int throwdelay;

    int delaytime = 1000;

    //控制时间的句柄
    FTimerHandle TimerHandle;

    AThrowWeapon* ballinstance;

    void donothing();
    

    void delayfun();

    // 新添加的函数声明，用于获取摄像机方向向量
    

    // 输入处理函数
    UFUNCTION(BlueprintCallable, Category = "MyFunction")
    //移动相关函数
    void InputMoveForward(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "MyFunction")
    void InputMoveRight(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "MyFunction")
    //视角的转换
    void LookRotation(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "MyFunction")
    //跳跃
    void InputJump(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "MyFunction")
    //冲刺
    void InputSprint(const FInputActionValue& Value);
    // Shift键按下和释放事件处理函数声明
    UFUNCTION(BlueprintCallable, Category = "Input")
    void OnShiftKeyPressed();
    UFUNCTION(BlueprintCallable, Category = "Input")
    void OnShiftKeyReleased();
    //下蹲
    UFUNCTION(BlueprintCallable, Category = "Input")
    void InputCrouch(const FInputActionValue& Value);
    //攻击
    UFUNCTION(BlueprintCallable, Category = "Input")
    void InputAttack(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void InputAim(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void InputAttackStop(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void InputAimStop(const FInputActionValue& Value);
    //翻滚
    UFUNCTION(BlueprintCallable, Category = "Input")
    void InputRoll(const FInputActionValue& Value);
    //攀爬
    UFUNCTION(BlueprintCallable, Category = "Input")
    void InputClimbUpWard(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void InputClimbRiht(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void StartClimb(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void EndClimb(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void StopClimb(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void ClimbAndMove(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void InputThrowBall(const FInputActionValue& Value);

    //武器实例
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponClass");
    TSubclassOf<AMeleeWeapon> Axe;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponClass");
    TSubclassOf<AThrowWeapon> RedBall;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponClass");
    TSubclassOf<ABaseWeapon> RedBallInHand;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponClass");
    TSubclassOf<AMeleeWeapon> PickAxe;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponClass");
    TSubclassOf<ABowWeapon> Bow;
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponClass");
    ABaseWeapon* weapon;
protected:

    //切换武器种类
    void RegisterInput();

    void SpawnMeleeWeapon(FName name, TSubclassOf<AMeleeWeapon>WeaponClass);
    
    void SpawnBowWeapon(FName name, TSubclassOf<ABowWeapon>WeaponClass);
    
    void SpawnThrowWeapon(FName name, TSubclassOf<ABaseWeapon>WeaponClass);


    //切换武器
    
    UFUNCTION(BlueprintCallable, Category = "Input")
    void ChangeWeapon1(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void ChangeWeapon2(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void ChangeWeapon3(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void ChangeWeapon4(const FInputActionValue& Value);

    //武器种类
    enum weaponclass
    {
        none=1,//空手
        melee,//近战
        bow,//弓
        throwing//投掷武器
        
    };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon");
    int weaponinhand;
    // 疾跑相关属性
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint");
    bool bIsSprint;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint");
    float SprintSpeedMultiplier;

    // 疾跑相关函数声明
    UFUNCTION(BlueprintCallable, Category = "Sprint")
    void StartSprint();

    UFUNCTION(BlueprintCallable, Category = "Sprint")
    void StopSprint();

    //蹲走相关属性
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    bool bIsCrouch;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint");
    float CrouchSpeedMultiplier;

    //瞄准和攻击相关属性
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint");
    bool bIsAim;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint");
    bool bIsAttack;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint");
    bool bIsThrowBall;

    //投掷动作共24帧
    int ThrowTime = 70;

    double AttackSpeedMultiplier;
    // 用于记录Shift键是否被按下
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
    bool bShiftKeyPressed;

    //翻滚相关属性
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
    int bIsRoll;
    //翻滚时间
    int Rolltime;
    //冷却时间
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
    int Resttime;
    double RollSpeedMutiplier;//速度系数

    //攀爬相关属性
    UPROPERTY(EditAnywhere , BlueprintReadOnly, Category = "Input")
    bool bIsClimb;

    UPROPERTY(EditAnywhere , BlueprintReadOnly, Category = "Input")
    bool bIsStartClimb;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    bool bIsClimbAndMove;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    bool bIsEndClimb;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    int EndClimbTime;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    int EndClimbTimer;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    FHitResult LastResult;

    
    int changetoClimb;
    double ClimbSpeedMutipulier;
    AActor* ObjectToClimb;

    bool checkClimb();

    //跳跃相关属性
    int jumpuptime;
    //体力条
    double HitPoint;

    //最大体力上限
    int MaxHP;

    //生命值
    int health;

    //最大生命值
    int MaxHealth = 600;
    public:
    //受到伤害的函数
    void DiaoXie(float DamageValue);
    protected:
    //死亡状态
    bool bIsDeath;
    
    //定义角色属性
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Property");
    PersonProperty ownerProperty;

    //定义疲劳状态，即体力值为0后体力未恢复至notTired不能疾跑
    bool tired;

    int notTired;
    //速度的权重
    double speedindex;

    // 用于动画蓝图获取当前移动速度
    
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
    float CurrentMovementSpeed;

    int GetWeaponClass();

    void setThrowEnd();

    void setThrowStart();

    void setOffHand();

    FVector GetCameraForwardDirection();

    FVector GetCameraRightDirection();


    //获取阵营接口
    UFUNCTION()
    Faction GetFaction();

};