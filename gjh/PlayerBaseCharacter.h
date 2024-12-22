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

//����������
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

    // �����������
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera");
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera");
    UCameraComponent* FollowCamera;

    // �����������
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
    // ��¼��ɫ�ϴ��ƶ�����ʱ�ĳ���
    FRotator LastMovementEndRotation;
    //��¼��������Ӫ��ǩ
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Faction");
    Faction faction = Faction::FRIEND;


    

    int throwdelay;

    int delaytime = 1000;

    //����ʱ��ľ��
    FTimerHandle TimerHandle;

    AThrowWeapon* ballinstance;

    void donothing();
    

    void delayfun();

    // ����ӵĺ������������ڻ�ȡ�������������
    

    // ���봦����
    UFUNCTION(BlueprintCallable, Category = "MyFunction")
    //�ƶ���غ���
    void InputMoveForward(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "MyFunction")
    void InputMoveRight(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "MyFunction")
    //�ӽǵ�ת��
    void LookRotation(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "MyFunction")
    //��Ծ
    void InputJump(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "MyFunction")
    //���
    void InputSprint(const FInputActionValue& Value);
    // Shift�����º��ͷ��¼�����������
    UFUNCTION(BlueprintCallable, Category = "Input")
    void OnShiftKeyPressed();
    UFUNCTION(BlueprintCallable, Category = "Input")
    void OnShiftKeyReleased();
    //�¶�
    UFUNCTION(BlueprintCallable, Category = "Input")
    void InputCrouch(const FInputActionValue& Value);
    //����
    UFUNCTION(BlueprintCallable, Category = "Input")
    void InputAttack(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void InputAim(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void InputAttackStop(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void InputAimStop(const FInputActionValue& Value);
    //����
    UFUNCTION(BlueprintCallable, Category = "Input")
    void InputRoll(const FInputActionValue& Value);
    //����
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

    //����ʵ��
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

    //�л���������
    void RegisterInput();

    void SpawnMeleeWeapon(FName name, TSubclassOf<AMeleeWeapon>WeaponClass);
    
    void SpawnBowWeapon(FName name, TSubclassOf<ABowWeapon>WeaponClass);
    
    void SpawnThrowWeapon(FName name, TSubclassOf<ABaseWeapon>WeaponClass);


    //�л�����
    
    UFUNCTION(BlueprintCallable, Category = "Input")
    void ChangeWeapon1(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void ChangeWeapon2(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void ChangeWeapon3(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void ChangeWeapon4(const FInputActionValue& Value);

    //��������
    enum weaponclass
    {
        none=1,//����
        melee,//��ս
        bow,//��
        throwing//Ͷ������
        
    };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon");
    int weaponinhand;
    // �����������
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint");
    bool bIsSprint;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint");
    float SprintSpeedMultiplier;

    // ������غ�������
    UFUNCTION(BlueprintCallable, Category = "Sprint")
    void StartSprint();

    UFUNCTION(BlueprintCallable, Category = "Sprint")
    void StopSprint();

    //�����������
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
    bool bIsCrouch;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint");
    float CrouchSpeedMultiplier;

    //��׼�͹����������
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint");
    bool bIsAim;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint");
    bool bIsAttack;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint");
    bool bIsThrowBall;

    //Ͷ��������24֡
    int ThrowTime = 70;

    double AttackSpeedMultiplier;
    // ���ڼ�¼Shift���Ƿ񱻰���
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
    bool bShiftKeyPressed;

    //�����������
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
    int bIsRoll;
    //����ʱ��
    int Rolltime;
    //��ȴʱ��
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
    int Resttime;
    double RollSpeedMutiplier;//�ٶ�ϵ��

    //�����������
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

    //��Ծ�������
    int jumpuptime;
    //������
    double HitPoint;

    //�����������
    int MaxHP;

    //����ֵ
    int health;

    //�������ֵ
    int MaxHealth = 600;
    public:
    //�ܵ��˺��ĺ���
    void DiaoXie(float DamageValue);
    protected:
    //����״̬
    bool bIsDeath;
    
    //�����ɫ����
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Property");
    PersonProperty ownerProperty;

    //����ƣ��״̬��������ֵΪ0������δ�ָ���notTired���ܼ���
    bool tired;

    int notTired;
    //�ٶȵ�Ȩ��
    double speedindex;

    // ���ڶ�����ͼ��ȡ��ǰ�ƶ��ٶ�
    
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
    float CurrentMovementSpeed;

    int GetWeaponClass();

    void setThrowEnd();

    void setThrowStart();

    void setOffHand();

    FVector GetCameraForwardDirection();

    FVector GetCameraRightDirection();


    //��ȡ��Ӫ�ӿ�
    UFUNCTION()
    Faction GetFaction();

};