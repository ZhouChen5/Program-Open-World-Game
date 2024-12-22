#include "PlayerBaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "DebugHelper.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/MeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/Vector.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "BaseWeapon.h"
#include "BowWeapon.h"
#include "MeleeWeapon.h"
#include "ThrowWeapon.h"
#include "Engine/World.h"
#define DEBUG_CLIMB 1


APlayerBaseCharacter::APlayerBaseCharacter()
{
    int a = 0;
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
    FollowCamera->SetupAttachment(CameraBoom);

    LastMovementEndRotation = FRotator::ZeroRotator;

    // ��ʼ��������ض���
    MoveForwardAction = NewObject<UInputAction>(this, TEXT("MoveForwardAction"));
    MoveRightAction = NewObject<UInputAction>(this, TEXT("MoveRightAction"));
    JumpAction = NewObject<UInputAction>(this, TEXT("JumpAction"));
    LookAction = NewObject<UInputAction>(this, TEXT("LookAction"));
    SprintAction = NewObject<UInputAction>(this, TEXT("SprintAction"));
    AimAction = NewObject<UInputAction>(this, TEXT("AimAction"));
    AttackAction = NewObject<UInputAction>(this, TEXT("AttackAction"));
    RollAction = NewObject<UInputAction>(this, TEXT("RollAction"));
    ClimbAction= NewObject<UInputAction>(this, TEXT("ClimbAction"));
    ClimbMoveAction= NewObject<UInputAction>(this, TEXT("ClimbMoveAction"));
    ClimbStopAction = NewObject<UInputAction>(this, TEXT("ClimbStopAction"));
    ChangeWeapon1Action = NewObject<UInputAction>(this, TEXT("ChangeWeapon1Action"));
    ChangeWeapon2Action = NewObject<UInputAction>(this, TEXT("ChangeWeapon2Action"));
    ChangeWeapon3Action = NewObject<UInputAction>(this, TEXT("ChangeWeapon3Action"));
    ChangeWeapon4Action = NewObject<UInputAction>(this, TEXT("ChangeWeapon4Action"));
    ThrowBallAction = NewObject<UInputAction>(this, TEXT("ThrowBallAction"));

    // ��ʼ�������������
    bIsSprint = false;
    SprintSpeedMultiplier = 1.5;

    //��ʼ�������������
    CrouchSpeedMultiplier = 0.5;
    bIsCrouch = false;

    //��ʼ����׼�͹���

    bIsAim = bIsAttack = 0;
    AttackSpeedMultiplier = 0.5;

    // ��ʼ��Shift������״̬Ϊfalse
    bShiftKeyPressed = false;

    //��ʼ���ٶ�Ȩ��
    speedindex = 1;

    //��ʼ������
    bIsRoll = 0;
    Resttime=20;//����ʱ��
    Rolltime = 62+Resttime;
    RollSpeedMutiplier =1000;
    
    //��ʼ������
    bIsClimb = 0;
    ClimbSpeedMutipulier = 0.3;
    bIsStartClimb = 0;
    bIsEndClimb = 0;
    EndClimbTime =EndClimbTimer= 41;//��������������41֡
    changetoClimb = jumpuptime;
    ObjectToClimb = nullptr;
    //��ʼ����Ծ
    //����������18֡
    jumpuptime = 18;
    // ��ʼ�����ڶ�����ͼ�ĵ�ǰ�ƶ��ٶ�
    CurrentMovementSpeed = 0.0f;

    //��ʼ������
    weaponinhand = 1;

    throwdelay = 0;
    //������
    MaxHP = 500;
    notTired=HitPoint = MaxHP;
    tired = 0;
    
    //��ʼ��Ѫ����Ѫ������
   
    MaxHealth = 99999999;

    health = MaxHealth;
    //��ʼ������״̬
    bIsDeath = 0;
}

void APlayerBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (EnhancedInputComponent)
    {
        EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &APlayerBaseCharacter::InputMoveForward);
        EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &APlayerBaseCharacter::InputMoveRight);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerBaseCharacter::InputJump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerBaseCharacter::InputJump);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerBaseCharacter::LookRotation);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APlayerBaseCharacter::InputSprint);
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerBaseCharacter::InputCrouch);
        EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerBaseCharacter::InputAttack);
        EnhancedInputComponent->BindAction(AttackStopAction, ETriggerEvent::Completed, this, &APlayerBaseCharacter::InputAttackStop);
        EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &APlayerBaseCharacter::InputAim);
        EnhancedInputComponent->BindAction(AimStopAction, ETriggerEvent::Completed, this, &APlayerBaseCharacter::InputAimStop);
        EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &APlayerBaseCharacter::InputRoll);
        EnhancedInputComponent->BindAction(ClimbAction, ETriggerEvent::Started, this, &APlayerBaseCharacter::StartClimb);
        EnhancedInputComponent->BindAction(ClimbMoveAction, ETriggerEvent::Triggered, this, &APlayerBaseCharacter::ClimbAndMove);
        EnhancedInputComponent->BindAction(ClimbStopAction, ETriggerEvent::Completed, this, &APlayerBaseCharacter::StopClimb);
        EnhancedInputComponent->BindAction(ChangeWeapon1Action, ETriggerEvent::Started, this, &APlayerBaseCharacter::ChangeWeapon1);
        EnhancedInputComponent->BindAction(ChangeWeapon2Action, ETriggerEvent::Started, this, &APlayerBaseCharacter::ChangeWeapon2);
        EnhancedInputComponent->BindAction(ChangeWeapon3Action, ETriggerEvent::Started, this, &APlayerBaseCharacter::ChangeWeapon3);
        EnhancedInputComponent->BindAction(ChangeWeapon4Action, ETriggerEvent::Started, this, &APlayerBaseCharacter::ChangeWeapon4);
        EnhancedInputComponent->BindAction(ThrowBallAction, ETriggerEvent::Started, this, &APlayerBaseCharacter::InputThrowBall);

        
        // ��Shift�����º��ͷ��¼�
        EnhancedInputComponent->BindAction(ShiftKeyAction, ETriggerEvent::Triggered, this, &APlayerBaseCharacter::OnShiftKeyPressed);
        EnhancedInputComponent->BindAction(ShiftKeyAction, ETriggerEvent::Completed, this, &APlayerBaseCharacter::OnShiftKeyReleased);
    }
}


void APlayerBaseCharacter::Tick(float deletatime)
{
    /*if (bIsAttack) {
        Debug::Print("1");
    }
    else {
        Debug::Print("0");
    }*/
    Super::Tick(deletatime);
    //bIsClimb = 1;
    UCharacterMovementComponent* movement = ACharacter::GetCharacterMovement();
    // ���µ�ǰ�ƶ��ٶ�
    if (movement) {
        movement->MaxWalkSpeed = 600 * speedindex;
    }
    CurrentMovementSpeed = GetVelocity().Size();
    UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
   
    if (bIsRoll>Resttime) {
        auto v=movement->Velocity.GetSafeNormal();
        movement->MaxWalkSpeed = RollSpeedMutiplier;
        if(v.Length()>0)
        movement->Velocity = v * RollSpeedMutiplier;
        else {
            movement->Velocity = GetActorForwardVector() * RollSpeedMutiplier;
        }
        //��ý�ɫ����
        FVector Forward = GetActorForwardVector();
        FVector GroundNormal = FVector::UpVector;
        //��ȡˮƽ����
        FVector Projection = Forward - (Forward.Dot(GroundNormal) / GroundNormal.Dot(GroundNormal)) * GroundNormal;
        AddMovementInput(v,speedindex);
    }
    if (bIsRoll == Resttime)
        movement->StopMovementImmediately();
    if(bIsRoll)
        bIsRoll--;
    else {
        if (bIsCrouch)
            speedindex = CrouchSpeedMultiplier;
        else if (bIsSprint) {
            if (HitPoint > 0 && !tired) {
                speedindex = SprintSpeedMultiplier;
            }
            else {
                bIsSprint = 0;
            }
        }
        else if (bIsAttack) {
            if (HitPoint > 0 && !tired) {
                speedindex = AttackSpeedMultiplier;
            }
            else {
                bIsAttack = 0;
            }
        }
        else if (bIsAim) {
            speedindex = AttackSpeedMultiplier;
        }
        else if (bIsClimb) {
            if (HitPoint > 0 && !tired) {
                }
            else {
                bIsClimb = bIsStartClimb=bIsEndClimb=0;
                Jump();
            }
        }
        else {
            speedindex = 1;
        }
    }
    FVector velocity = GetVelocity();
    // �����ɫ�����ƶ��������ƶ��������ý�ɫ����

    if (!bIsClimb && !bIsStartClimb) {   
        velocity.Z = 0; //ֻҪҪ����ˮƽ����ķ���
        if (velocity.SizeSquared() > KINDA_SMALL_NUMBER||bIsAim)
        {
            if (velocity.SizeSquared() > KINDA_SMALL_NUMBER&&!bIsAim) {
                FRotator newRotation = velocity.Rotation();
               
                SetActorRotation(newRotation);
                LastMovementEndRotation = newRotation;
            }
            if (bIsAim) {
                auto camdirection = GetCameraForwardDirection();
                camdirection.Z = 0;
                SetActorRotation(camdirection.Rotation());
                LastMovementEndRotation = camdirection.Rotation();
            }
        }
        else
        {         
            SetActorRotation(LastMovementEndRotation);
        }
    }
    else {
        
        auto newRotation = GetActorForwardVector();
        //Debug::Print("4");
        SetActorRotation(newRotation.Rotation());
    }

    if (int(HitPoint) == notTired&&tired) {
        tired = 0;
    }
    //�ж��Ƿ�ƣ��
    if (int(HitPoint) == 0) {
        Debug::Print("tired!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        tired = 1;
    }
    if (tired) {
        Debug::Print("tired");
        bIsAttack = bIsClimb = bIsClimbAndMove = bIsEndClimb = bIsSprint = bIsStartClimb = 0;
        if (!bIsCrouch) {
            GetCharacterMovement()->MaxWalkSpeed = 300;
        }
    }
    //�ָ�����
    if (!bIsAttack && HitPoint < MaxHP&& !bShiftKeyPressed&&!bIsClimb) {//����û���Ҳ���shift�Ҳ�����
        HitPoint++;
    }
    else if (((bIsSprint&& CurrentMovementSpeed)||bIsAttack||bIsClimbAndMove)&&!tired) {//���ڼ���״̬���������ƶ�
        if (HitPoint > 0) {
           
            if (bIsAttack)
                HitPoint -= 0.1;
            else
                HitPoint--;
            
        }
        else {
            
            bIsClimb = 0;
            bIsStartClimb = 0;
            bIsEndClimb = 0;
            bIsSprint = 0;
            bIsAttack = 0;
            movement->SetMovementMode(EMovementMode::MOVE_Walking);
            movement->GravityScale = 1.0f;
        }
    }
   //���п�ʼ�����Ķ���
    
    if (bIsStartClimb&&!bIsClimb&&!tired) {
       if(changetoClimb>0)
        changetoClimb--;
        else {
            changetoClimb = jumpuptime;
            bIsStartClimb = 0;
            bIsClimb = 1;
        }
    }
    if (bIsClimb && !tired) {
       
        if (bIsClimbAndMove == 0) {
            movement->Velocity = { 0,0,0 };
        }
        movement->SetMovementMode(EMovementMode::MOVE_Flying);
        
        movement->GravityScale = 0.0f;
        //movement->bConstrainToPlane = false;
        //�ٶ�����
        if (!bIsStartClimb) {
            movement->Velocity.X = movement->Velocity.Y
                = movement->Velocity.Z = 0;
            bIsStartClimb = 1;
        }
        UWorld* World = GetWorld();

        // ����������㣬������Actor����λ��Ϊ�����ɸ���ʵ���������Ϊ�����ʵ�λ�ã������ɫ���ֽ�λ�õ�
        FVector StartLocationWaist = GetActorLocation();
        FVector StartLocationHead = StartLocationWaist;

        // ��ȡ���������
        UCapsuleComponent* topsize = GetCapsuleComponent();
        // ��ȡ��ɫͷ���߶�
        auto height = topsize->GetScaledCapsuleHalfHeight();
        // ����ͷ�����ߵ�λ��
        StartLocationHead.Z += height;

        // �������߷��������������Actor����ǰ����X�������򣩣��ɸ���ʵ�������޸�Ϊ��ͬ����ļ������
        FVector rayDirection = GetActorForwardVector();

        // ���߳��ȣ���Χ�����ɸ���ʵ���������
        float rayLength = 100.0f;

        // ������ײͨ��������ʹ��ECC_WorldStatic��ʾֻ��⾲̬���壬�ɸ�����Ҫ���������ײͨ���Լ�ⲻͬ���͵�����
        FCollisionObjectQueryParams ObjectQueryParams(ECollisionChannel::ECC_WorldStatic);

        // ִ�����߼��
        FHitResult hitResultWaist;

        // �������߼��
        bool bHitwaist = World->LineTraceSingleByObjectType(
            hitResultWaist,
            StartLocationWaist,
            StartLocationWaist + rayDirection * rayLength,
            ObjectQueryParams
        );
        
        if ( bHitwaist)
        {
            // ����������壬��ȡ��������������Ϣ
            AActor* HitActorWaist = hitResultWaist.GetActor();
#if DEBUG_CLIMB
            DrawDebugLine(
                World,
                StartLocationWaist,
                hitResultWaist.Location,
                FColor::Blue,
                false,
                100.0f,
                0,
                1.0f
            );
#endif
            // ��ȡ���е�ķ�������
            FVector HitNormalWaist = - hitResultWaist.Normal;
            LastMovementEndRotation = HitNormalWaist.Rotation();
            SetActorRotation(HitNormalWaist.Rotation());
        }
        else {
           
            SetActorRotation(LastMovementEndRotation);

        }
        FHitResult hitResultBottom;
        auto StartLocationBottom = StartLocationWaist;
        StartLocationBottom.Z -= height;
        rayDirection = -GetActorUpVector();
        rayLength = 30.f;
        bool bHitBottom= World->LineTraceSingleByObjectType(
            hitResultWaist,
            StartLocationBottom,
            StartLocationBottom + rayDirection * rayLength,
            ObjectQueryParams
        );
        if (bHitBottom) {
            movement->SetMovementMode(EMovementMode::MOVE_Walking);
            movement->GravityScale = 1.0f;
#if DEBUG_CLIMB
            DrawDebugLine(
                World,
                StartLocationWaist,
                hitResultBottom.Location,
                FColor::Blue,
                false,
                100.0f,
                0,
                1.0f
            );
            bIsClimb = bIsStartClimb = 0;
#endif
        }
        else {
#if DEBUG_CLIMB
            DrawDebugLine(
                World,
                StartLocationWaist,
                StartLocationWaist + rayDirection * rayLength,
                FColor::Blue,
                false,
                100.0f,
                0,
                1.0f
            );
#endif
        }


    }
    else {
        if (movement->MovementMode == EMovementMode::MOVE_Flying){
            movement->SetMovementMode(EMovementMode::MOVE_Walking);
            movement->GravityScale = 1.0f;
        }
        
    }
    if (bIsEndClimb && !tired) {
        FVector destiny = LastResult.Location;
        auto location = GetActorLocation();
        if (EndClimbTimer) {

            EndClimbTimer--;
            // ��ȡ��ɫ��ǰ������
            FVector ForwardVector = GetActorForwardVector();
            // ��ȡ��ɫ����������
            FVector UpVector = GetActorUpVector();
            // �������ϵ�λ������
            FVector UpDisplacement = UpVector * ((CapsuleComp->GetScaledCapsuleHalfHeight() * 2 + 1) / EndClimbTime);
            // ���λ������
            FVector Displacement = UpDisplacement;
            // ��λ��������ӵ���ǰλ��
            location += Displacement;
            SetActorLocation(location);
        }
        else {
            // ��ȡ��ɫ��ǰ������
            FVector ForwardVector = GetActorForwardVector();
            // ��ȡ��ɫ����������

            FVector ForwardDisplacement = ForwardVector * (CapsuleComp->GetScaledCapsuleRadius() + 2);
            // �������ϵ�λ������
            Debug::print("forward");
            FVector Displacement = ForwardDisplacement;
            // ��λ��������ӵ���ǰλ��
            location += Displacement;
            SetActorLocation(location);
            bIsEndClimb = 0;
            bIsClimb = false;
            bIsStartClimb = 0;
            bIsClimbAndMove = 0;
            EndClimbTimer = EndClimbTime;
        }
    }
    FRotator rotation = GetActorRotation();
    //����Ͷ��
    
    if (GEngine)
    { 
       
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("ABaseWeapon Pointer Address: %d"), health));
    }
    
   
}


void APlayerBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
    RegisterInput();
    /*SpawnWeapon();
    if (weapon) {
        weapon->Destroy();
    }*/
}


// ��ȡ�����ǰ���������ĺ���ʵ��
FVector APlayerBaseCharacter::GetCameraForwardDirection()
{
    APlayerController* playerController = Cast<APlayerController>(GetController());
    if (playerController)
    {
        FVector CameraLocation;
        FRotator CameraRotation;
        playerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
        return CameraRotation.Vector();
    }
    return FVector::ForwardVector;
}

// ��ȡ����������������ĺ���ʵ��
FVector APlayerBaseCharacter::GetCameraRightDirection()
{
    FVector cameraForward = GetCameraForwardDirection();
    return cameraForward.RotateAngleAxis(90.0f, FVector::UpVector);
}
void APlayerBaseCharacter::LookRotation(const FInputActionValue& Value)
{
    FVector2D axis2D = Value.Get<FVector2D>();

    // ����ƶ�ʱֻ�ı�������ӽǣ���Ӱ���ɫ����
    if (Controller)
    {
        //Debug::Print("llll");
        //AddActorLocalRotation();
        //if (!bIsClimb)
        AddControllerYawInput(axis2D.X);
        AddControllerPitchInput(-axis2D.Y);
     
    }
}
void APlayerBaseCharacter::donothing()
{
    if (ballinstance) {
        Debug::Print("speed");

        ballinstance->GetProjectileMovementComponent()->Velocity = GetCameraForwardDirection() * 1000;
        
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, ballinstance->GetProjectileMovementComponent()->Velocity.ToString());
         
        auto Impluse = GetCameraForwardDirection() * 1000 +GetCharacterMovement()->Velocity;

       
        if(!bIsAim)
        ballinstance->RootComp->AddImpulse(Impluse,NAME_None,true);
        else {
            ballinstance->RootComp->AddImpulse(Impluse, NAME_None, true);
        }
        auto cameraloaction = FollowCamera->GetRelativeLocation();
        if (bIsAim) {
            cameraloaction.X -= 150;
            cameraloaction.Y -= 30;
            cameraloaction.Z -= 80;
            bIsAim = 0;
        }
        FollowCamera->SetRelativeLocation(cameraloaction);

   }
}
void APlayerBaseCharacter::delayfun()
{
}
void APlayerBaseCharacter::InputMoveForward(const FInputActionValue& Value)
{
    if (!bIsDeath) {
        if (Controller && !bIsEndClimb)
        {
            if (bIsClimb && !bIsEndClimb)
            {
                //Debug::print("climb");
                UWorld* World = GetWorld();
                if (World == nullptr)
                {
                    return;
                }

                // ͨ����ǰ��ɫʵ����ȡCharacterMovementComponent
                UCharacterMovementComponent* movement = this->GetCharacterMovement();
                // ȷ�����Բ���ƽ�����ƽ��������ƶ�
                movement->bConstrainToPlane = false;

                if (movement)
                {
                    auto position = GetActorLocation();
                    //��ȡ������
                    UCapsuleComponent* topsize = GetCapsuleComponent();
                    //��ȡ��ɫͷ���߶�
                    float height = topsize->GetScaledCapsuleHalfHeight();
                    //���߳���
                    float rayLength = 50.0f;
                    //ȷ���������
                    FVector startLocation = position;
                    startLocation.Z += height;

                    // �������߷�����������Ϊ���ϣ�Z��������
                    double angle = FMath::DegreesToRadians(-Value.Get<float>() * 30);
                    // ��ȡ��ɫָ��ķ�ʽ

                   //FVector LocalVector = RotationMatrix.TransformVector(rayDirection);
                    FVector rayDirection = GetActorForwardVector();
                    // ������ײͨ��������ʹ��ECC_WorldStatic��ʾֻ��⾲̬���壬����Ը���ʵ�������޸�
                    FCollisionObjectQueryParams ObjectQueryParams(ECollisionChannel::ECC_WorldStatic);

                    // ִ�����߼��
                    FHitResult hitResult1;
                    FHitResult hitResult2;
                    bool bHit2 = World->LineTraceSingleByObjectType(
                        hitResult2,
                        startLocation,
                        startLocation + rayDirection * rayLength,
                        ObjectQueryParams
                    );

                    rayDirection = rayDirection.RotateAngleAxis(angle, GetActorRightVector());

                    //�������߼��
                    bool bHit1 = World->LineTraceSingleByObjectType(
                        hitResult1,
                        startLocation,
                        startLocation + rayDirection * rayLength,
                        ObjectQueryParams
                    );

                    if (bHit1)
                    {
                        LastResult = hitResult1;
                        // �õ����㴦�ķ������������ں���ȷ�����������λ�õ���
                        FVector HitNormal1 = hitResult1.Normal;
                        FVector HitNormal2 = hitResult2.Normal;

                        // �����ɫӦ��������Ŀ��λ�ã�����򵥵ؽ���ɫ�ƶ��������Ϸ�һ�㣬ʹ�俴��������������
                        // ���Ը���ʵ���������ϸ�ص������ƫ������������ݽ�ɫ�Ķ�����̬��
                        FVector TargetLocation1 = hitResult1.Location + HitNormal1 * 40;
                        FVector TargetLocation2 = hitResult2.Location + HitNormal2 * 40;

                        // ������������
                        FVector HitLocation1 = hitResult1.Location;
#if DEBUG_CLIMB
                        DrawDebugLine(
                            World,
                            startLocation,
                            HitLocation1,
                            FColor::Red,
                            false,
                            100.0f,
                            0,
                            1.0f
                        );
#endif
                        FVector HitLocation2 = hitResult2.Location;
#if DEBUG_CLIMB                 
                        DrawDebugLine(
                            World,
                            startLocation,
                            HitLocation2,
                            FColor::Red,
                            false,
                            100.0f,
                            0,
                            1.0f
                        );
#endif
                        auto Target = TargetLocation1 - TargetLocation2;
                        Target.GetSafeNormal();
                        //movement->MaxAcceleration=2147482644;

                       

                        AddMovementInput(TargetLocation1 - TargetLocation2, 1);

                        //// ���½�ɫλ��
                        //SetActorLocation(TargetLocation);
                        // ������������Ӹ����߼����������������еĶ������š��ٶȿ��Ƶ�
                        // ���磬���������ٶ�
                        movement->MaxWalkSpeed = 20.0f;
                        // ��������������½�ɫ��״̬��
                        // ��������һ����ʾ���������Ĳ��������ȣ��Ա������������и������״̬������Ӧ����
                        
                    }
                    else
                    {
                        //������������
                        FVector EndLocation = startLocation + rayDirection * rayLength;
#if DEBUG_CLIMB
                        DrawDebugLine(
                            World,
                            startLocation,
                            EndLocation,
                            FColor::Green,
                            false,
                            200.0f,
                            0,
                            1.0f
                        );
#endif
                        movement->bConstrainToPlane = true;
                        movement->MaxWalkSpeed = 600.0f;

                        bIsEndClimb = 1;
                    }
                }
                          
            }
            else
            {

                FVector cameraForward = GetCameraForwardDirection();
                FVector GroundNormal = FVector::UpVector;
                FVector Projection = cameraForward - (cameraForward.Dot(GroundNormal) / GroundNormal.Dot(GroundNormal)) * GroundNormal;

                auto move = ACharacter::GetCharacterMovement();
                //move->GravityScale = 0.0f;
                //move->Velocity.Z = 300 * Value.Get<float>();
                AddMovementInput(Projection, Value.Get<float>());
            }
        }
    }
}

void APlayerBaseCharacter::InputMoveRight(const FInputActionValue& Value)
{
    if (!bIsDeath) {
        if (Controller && !bIsEndClimb)
        {
            if (bIsClimb)
            {
                //Debug::print("climb");
                UWorld* World = GetWorld();
                if (World == nullptr)
                {
                    return;
                }

                // ͨ����ǰ��ɫʵ����ȡCharacterMovementComponent
                UCharacterMovementComponent* movement = this->GetCharacterMovement();
                // ȷ�����Բ���ƽ�����ƽ��������ƶ�
                movement->bConstrainToPlane = false;

                if (movement)
                {
                    auto position = GetActorLocation();
                    //��ȡ������
                    UCapsuleComponent* topsize = GetCapsuleComponent();
                    //��ȡ��ɫͷ���߶�
                    float height = topsize->GetScaledCapsuleHalfHeight();
                    //���߳���
                    float rayLength = 50.0f;
                    //ȷ���������
                    FVector startLocation = position;
                    startLocation.Z += height;

                    double angle = FMath::DegreesToRadians(-Value.Get<float>() * 20);

                    // ��ȡ��ɫָ��ķ�ʽ
                    FVector rayDirection = GetActorForwardVector();

                    // ������ײͨ��������ʹ��ECC_WorldStatic��ʾֻ��⾲̬���壬����Ը���ʵ�������޸�
                    FCollisionObjectQueryParams ObjectQueryParams(ECollisionChannel::ECC_WorldStatic);

                    // ִ�����߼��
                    FHitResult hitResult1;
                    FHitResult hitResult2;
                    bool bHit2 = World->LineTraceSingleByObjectType(
                        hitResult2,
                        startLocation,
                        startLocation + rayDirection * rayLength,
                        ObjectQueryParams
                    );

                    rayDirection = rayDirection.RotateAngleAxis(angle, GetActorUpVector());

                    //�������߼��
                    bool bHit1 = World->LineTraceSingleByObjectType(
                        hitResult1,
                        startLocation,
                        startLocation + rayDirection * rayLength,
                        ObjectQueryParams
                    );

                    if (bHit1)
                    {
                        // �õ����㴦�ķ������������ں���ȷ�����������λ�õ���
                        FVector HitNormal1 = hitResult1.Normal;
                        FVector HitNormal2 = hitResult2.Normal;

                        // �����ɫӦ��������Ŀ��λ�ã�����򵥵ؽ���ɫ�ƶ��������Ϸ�һ�㣬ʹ�俴��������������
                        // ���Ը���ʵ���������ϸ�ص������ƫ������������ݽ�ɫ�Ķ�����̬��
                        FVector TargetLocation1 = hitResult1.Location + HitNormal1 * 40;
                        FVector TargetLocation2 = hitResult2.Location + HitNormal2 * 40;

                        // ������������
                        FVector HitLocation1 = hitResult1.Location;
#if DEBUG_CLIMB
                        DrawDebugLine(
                            World,
                            startLocation,
                            HitLocation1,
                            FColor::Blue,
                            false,
                            100.0f,
                            0,
                            1.0f
                        );
#endif
                        FVector HitLocation2 = hitResult2.Location;
#if DEBUG_CLIMB
                        DrawDebugLine(
                            World,
                            startLocation,
                            HitLocation2,
                            FColor::Blue,
                            false,
                            100.0f,
                            0,
                            1.0f
                        );
#endif
                        auto Target = TargetLocation1 - TargetLocation2;
                        Target.GetSafeNormal();
                        //movement->MaxAcceleration = 2147482644;

                        /*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::FromInt(Target.X * 100));
                        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::FromInt(Target.Y * 100));
                        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::FromInt(Target.Z * 100));
                        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::FromInt(movement->GetMaxAcceleration()));*/


                        AddMovementInput(-TargetLocation1 + TargetLocation2, 1);

                        //// ���½�ɫλ��
                        //SetActorLocation(TargetLocation);
                        // ������������Ӹ����߼����������������еĶ������š��ٶȿ��Ƶ�
                        // ���磬���������ٶ�
                        movement->MaxWalkSpeed = 20.0f;
                        // ��������������½�ɫ��״̬��
                        // ��������һ����ʾ���������Ĳ��������ȣ��Ա������������и������״̬������Ӧ����
                        //bIsClimb = true;
            //            // ����Ƿ񵽴�����Ŀ��λ�ã������ʾ�����ɸ���ʵ��������ƣ�
            //            //if (FVector::Distance(GetActorLocation(), TargetLocation) < 10.f)
            //            //{
            //            //    // ����ӽ�Ŀ��λ�ã����������״̬��ʾ������ɵ�
            //            //    bIsClimb = false;
            //            //    bIsStartClimb = 0;
            //            //    movement->bConstrainToPlane = true;
            //            //    movement->MaxWalkSpeed = 600.0f;
            //            //}
            //        }
                    }
                    else
                    {
                        //������������
                        FVector EndLocation = startLocation + rayDirection * rayLength;
#if DEBUG_CLIMB
                        DrawDebugLine(
                            World,
                            startLocation,
                            EndLocation,
                            FColor::Cyan,
                            false,
                            200.0f,
                            0,
                            1.0f
                        );
#endif
                        movement->bConstrainToPlane = true;
                        movement->MaxWalkSpeed = 600.0f;
                        bIsClimb = false;
                        bIsStartClimb = 0;
                    }
                }
                //            // ���û�м�⵽���������壬�ָ���ɫ�������ƶ�״̬�������
                //            movement->bConstrainToPlane = true;
                //            movement->MaxWalkSpeed = 600.0f;
                //            bIsClimb = false;
                //            bIsStartClimb = 0;
                //        }
                //    }
            }
            else
            {
                FVector cameraForward = GetCameraRightDirection();
                FVector GroundNormal = FVector::UpVector;
                FVector Projection = cameraForward - (cameraForward.Dot(GroundNormal) / GroundNormal.Dot(GroundNormal)) * GroundNormal;

                //auto move = ACharacter::GetCharacterMovement();
                //move->GravityScale = 0.0f;
                //move->Velocity.Z = 300 * Value.Get<float>();
                AddMovementInput(Projection, Value.Get<float>());
            }
        }
    }
}

void APlayerBaseCharacter::InputJump(const FInputActionValue& Value)
{
    if (!bIsDeath) {
        bool isjump = Value.Get<bool>();
        if (bIsClimb) {
            bIsClimb = 0;
            return;
        }

        if (!bIsCrouch) {
            if (isjump && !bIsClimb)
            {
                Jump();
            }
            else
            {
                StopJumping();
            }
        }
        else {
            bIsCrouch = !bIsCrouch;
        }
    }
}

void APlayerBaseCharacter::InputSprint(const FInputActionValue& Value)
{
    if (!bIsDeath) {
        bool issprint = Value.Get<bool>();
        if (bShiftKeyPressed && !tired && !bIsClimb) {
            AActor* owner = GetOwner();

            owner->SetActorRotation({ 20,45,70 });
            //Debug::print("pressed2");
            StartSprint();
        }
        else {
            //Debug::print("not1");
            StopSprint();
        }
    }
}

void APlayerBaseCharacter::StartSprint()
{
    if (!bIsDeath) {
        if (!tired) {
            //Debug::print("pressed1");
            bIsSprint = true;
        }
        else {
            bIsSprint = 0;
        }
    }
   // Debug::print("sprinting");
   
}

void APlayerBaseCharacter::StopSprint()
{
    if (!bIsDeath) {
        bIsSprint = false;
        //Debug::print("notsprinting");
    }
}

bool APlayerBaseCharacter::checkClimb()
{
    if (!bIsDeath) {
        // ��ȡ����
        UWorld* World = GetWorld();
        if (World == nullptr)
        {
            return false;
        }

        // ����������㣬������Actor����λ��Ϊ�����ɸ���ʵ���������Ϊ�����ʵ�λ�ã������ɫ���ֽ�λ�õ�
        FVector StartLocationWaist = GetActorLocation();
        FVector StartLocationHead = StartLocationWaist;

        // ��ȡ���������
        UCapsuleComponent* topsize = GetCapsuleComponent();
        // ��ȡ��ɫͷ���߶�
        auto height = topsize->GetScaledCapsuleHalfHeight();
        // ����ͷ�����ߵ�λ��
        StartLocationHead.Z += height;

        // �������߷��������������Actor����ǰ����X�������򣩣��ɸ���ʵ�������޸�Ϊ��ͬ����ļ������
        FVector rayDirection = GetActorForwardVector();

        // ���߳��ȣ���Χ�����ɸ���ʵ���������
        float rayLength = 200.0f;

        // ������ײͨ��������ʹ��ECC_WorldStatic��ʾֻ��⾲̬���壬�ɸ�����Ҫ���������ײͨ���Լ�ⲻͬ���͵�����
        FCollisionObjectQueryParams ObjectQueryParams(ECollisionChannel::ECC_WorldStatic);

        // ִ�����߼��
        FHitResult hitResultHead;
        FHitResult hitResultWaist;

        // �������߼��
        bool bHitwaist = World->LineTraceSingleByObjectType(
            hitResultWaist,
            StartLocationWaist,
            StartLocationWaist + rayDirection * rayLength,
            ObjectQueryParams
        );

        // ͷ�����߼��
        bool bHithead = World->LineTraceSingleByObjectType(
            hitResultHead,
            StartLocationHead,
            StartLocationHead + rayDirection * rayLength,
            ObjectQueryParams
        );

        if (bHithead && bHitwaist)
        {
            // ����������壬��ȡ��������������Ϣ
            AActor* HitActorHead = hitResultHead.GetActor();
            AActor* HitActorWaist = hitResultWaist.GetActor();

            // ��ȡ���е�ķ�������
            FVector HitNormalHead = hitResultHead.Normal;
            FVector HitNormalWaist = hitResultWaist.Normal;

            // �������������¶��Ƿ��ڿ�������Χ�ڣ��������������¶ȷ�ΧΪ [-45, 45] �ȣ��ɸ���ʵ���������
            float angleThreshold = 45.0f;
            float angleHead = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(FVector::UpVector, HitNormalHead)));
            float angleWaist = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(FVector::UpVector, HitNormalWaist)));

            if (FMath::Abs(angleHead) >= angleThreshold && FMath::Abs(angleWaist) >= angleThreshold)
            {
                // ��ȡ����ǰ������������
                ObjectToClimb = HitActorHead;

                /*if (GEngine)
                {
                    FString DebugMessageHead = FString::Printf(TEXT("Raycast hit actor (head): %s"), *HitActorHead->GetName());
                    FString DebugMessageWaist = FString::Printf(TEXT("Raycast hit actor (waist): %s"), *HitActorWaist->GetName());
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, DebugMessageHead);
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, DebugMessageWaist);
                }*/

                // ���Ƶ������ߣ�ֻ�ڿ����׶���ʾ��
                FVector HitHeadLocation = hitResultHead.Location;
                FVector HitWaistLocation = hitResultWaist.Location;
#if DEBUG_CLIMB
                DrawDebugLine(
                    World,
                    StartLocationHead,
                    HitHeadLocation,
                    FColor::Red,
                    false,
                    2.0f,
                    0,
                    1.0f
                );
                DrawDebugLine(
                    World,
                    StartLocationWaist,
                    HitWaistLocation,
                    FColor::Red,
                    false,
                    2.0f,
                    0,
                    1.0f
                );
#endif
                return true;
            }
            else
            {
                // ����¶Ȳ��ڿ�������Χ�ڣ���ӡ��ʾ��Ϣ������δ���еĵ������ߣ���ѡ��
                if (GEngine)
                {
                    FString DebugMessage = FString::Printf(TEXT("Surface slope out of climbable range"));
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, DebugMessage);
                }

                FVector EndLocation = StartLocationHead + rayDirection * rayLength;
#if DEBUG_CLIMB
                DrawDebugLine(
                    World,
                    StartLocationHead,
                    EndLocation,
                    FColor::Green,
                    false,
                    2.0f,
                    0,
                    1.0f
                );
                EndLocation.Z -= height;
                DrawDebugLine(
                    World,
                    StartLocationWaist,
                    EndLocation,
                    FColor::Black,
                    false,
                    2.0f,
                    0,
                    1.0f
                );
#endif
                return false;
            }
        }
        else
        {
            // ���δ�������壬����δ���еĵ������ߣ���ѡ��
            FVector EndLocation = StartLocationHead + rayDirection * rayLength;
#if DEBUG_CLIMB
            DrawDebugLine(
                World,
                StartLocationHead,
                EndLocation,
                FColor::Green,
                false,
                2.0f,
                0,
                1.0f
            );
#endif
            return false;
        }
    }
    else {
        return false;
    }
}
void APlayerBaseCharacter::DiaoXie(float DamageValue)
{
    health -= DamageValue;
    if (health <= 0) {
        bIsDeath = 1;
    }

}
int APlayerBaseCharacter::GetWeaponClass()
{
    return weaponinhand;
}
void APlayerBaseCharacter::setThrowEnd()
{
    bIsThrowBall = 0;
    if (weapon) {
        Debug::Print("destory");
        // if(!(weapon->Destroyed()))
        weapon->Destroy();
        weapon = nullptr;
    }
    SpawnThrowWeapon("ball", RedBallInHand);
    weaponinhand = weaponclass::throwing;
}
void APlayerBaseCharacter::setThrowStart()
{
    bIsThrowBall = 1;
    
}


void APlayerBaseCharacter::setOffHand()
{
    if (RedBall) {
        ballinstance = GetWorld()->SpawnActor<AThrowWeapon>(RedBall, weapon->GetActorLocation(), GetActorRotation());
        if (ballinstance) {
            
            if (GEngine) {
                throwdelay = delaytime;               
                //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, ballinstance->GetProjectileMovementComponent()->Velocity.ToString());
                Debug::Print("stop");
                GetWorldTimerManager().SetTimer(TimerHandle, this,&APlayerBaseCharacter::donothing, 0.05, false,0.05);
             
                
                }
        }
    }
    if (weapon) {
        weapon->Destroy();
    }
}
void APlayerBaseCharacter::OnShiftKeyPressed()
{
    if (!bIsDeath) {
        bShiftKeyPressed = true;
        if (!bIsSprint && !tired)
        {
            //Debug::print("pressed3");
            StartSprint();
        }
    }
}

void APlayerBaseCharacter::OnShiftKeyReleased()
{
    if (!bIsDeath) {
        bShiftKeyPressed = false;
        if (bIsSprint)
        {
            //Debug::print("not1");
            StopSprint();
        }
    }
}

void APlayerBaseCharacter::InputCrouch(const FInputActionValue& Value)
{
    //Debug::print("9");
    if (!bIsDeath) {
        if (!bIsClimb)
            bIsCrouch = !bIsCrouch;
    }
}

void APlayerBaseCharacter::InputAttack(const FInputActionValue& Value)
{
    if (!bIsDeath) {
        Debug::print("attack");
        //��̲��ܹ���
        if (!bIsSprint && !tired && !bIsClimb && weaponinhand != weaponclass::throwing) {
            bIsAttack = 1;
            if (bIsAttack) {
                //
                speedindex = AttackSpeedMultiplier;
            }
            else {
                //Debug::print("notattack");
                speedindex = 1;
            }
        }
        if (tired) {
            bIsAttack = 0;
        }
    }
}

void APlayerBaseCharacter::InputAim(const FInputActionValue& Value)
{
    if (!bIsDeath) {
        //��̲�����׼
        //��ս����������׼
        if (!bIsAim && !bIsSprint && !bIsClimb && weaponinhand != weaponclass::melee) {
            bIsAim = 1;
            if (bIsAim) {
                Debug::print("aim");
                speedindex = AttackSpeedMultiplier;
                // ��ȡ��ҿ��������ƵĽ�ɫ
                auto cameraloaction = FollowCamera->GetRelativeLocation();
                cameraloaction.X += 150;
                cameraloaction.Y += 30;
                cameraloaction.Z += 80;
                FollowCamera->SetRelativeLocation(cameraloaction);

                
                Debug::Print("ppp");
                auto cameradirection = FollowCamera->GetForwardVector();
                cameradirection.Z = 0;
                SetActorRotation(cameradirection.Rotation());
                Debug::Print("kkk");
            }
            else {
                //Debug::print("notaim");
                speedindex = 1;
            }
        }
    }
}

void APlayerBaseCharacter::InputAttackStop(const FInputActionValue& Value)
{
    if (!bIsDeath) {
        Debug::print("notattack");
        if (weaponinhand != weaponclass::throwing) {
            bIsAttack = 0;
            if (bIsAttack) {
                //Debug::print("attack");
                speedindex = AttackSpeedMultiplier;
            }
            else {

                //
                speedindex = 1;
            }
        }
        if (tired) {
            bIsAttack = 0;
        }
    }
}

void APlayerBaseCharacter::InputAimStop(const FInputActionValue& Value)
{
    if (!bIsDeath) {
        if (bIsAim && weaponinhand != weaponclass::melee) {
            bIsAim = 0;
            if (bIsAim) {
                // Debug::print("aim");
                speedindex = AttackSpeedMultiplier;
            }
            else {
                auto cameraloaction = FollowCamera->GetRelativeLocation();

                cameraloaction.X -= 150;
                cameraloaction.Y -= 30;
                cameraloaction.Z -= 80;

                FollowCamera->SetRelativeLocation(cameraloaction);

                speedindex = 1;
            }
        }
    }
}

void APlayerBaseCharacter::InputRoll(const FInputActionValue& Value)
{
    if (!bIsDeath) {
        if (!bIsRoll && !tired && !bIsClimb) {
            // ��ȡ��ǰ��ɫ������任
            FTransform CurrentTransform = GetActorTransform();

            // ��ȡ��ǰ��ɫ����ת����FRotator��ʽ��
            FRotator CurrentRotation = CurrentTransform.Rotator();

            
            auto move = ACharacter::GetCharacterMovement();
            move->StopMovementImmediately();
            Debug::print("roll");
            move->MaxWalkSpeed = RollSpeedMutiplier;
            HitPoint -= 50;
            bIsRoll = Rolltime;
        }
    }
}


void APlayerBaseCharacter::InputClimbUpWard(const FInputActionValue& Value)
{
    if (!bIsDeath) {
        //�����ɫͷ����ȡ
        FVector Upward = GetActorUpVector();
        AddMovementInput(Upward, Value.Get<float>());
    }
}

void APlayerBaseCharacter::InputClimbRiht(const FInputActionValue& Value)
{
}


void APlayerBaseCharacter::StartClimb(const FInputActionValue& Value)
{
   
    if (!bIsDeath) {
       
        auto anim = GetMesh()->GetAnimInstance();
        //auto bisfall=anim
        
        if (bIsClimb || tired) {
            bIsClimb = 0;
            bIsStartClimb = 0;
            return;           
        }
        else {
            bool result = checkClimb();
            
            if (result)
            {
                bIsStartClimb = 1;

                // ��ȡ����ָ��
                UWorld* World = GetWorld();
                if (World == nullptr)
                {
                    return;
                }
               
                // �������߼���������
                float rayLength = 200.0f; // ���߳��ȣ��ɸ���ʵ���������
                FVector startLocation = GetActorLocation();
                startLocation.Z += GetCapsuleComponent()->GetScaledCapsuleHalfHeight(); // �ӽ�ɫͷ���Ϸ�һ����Ϊ�������
                // ���߷���Ϊ��ɫ��ǰ��
                FVector rayDirection = GetActorForwardVector();
                FCollisionObjectQueryParams ObjectQueryParams(ECollisionChannel::ECC_WorldStatic);

                // ִ�����߼��
                FHitResult hitResult;
                bool bHit = World->LineTraceSingleByObjectType(
                    hitResult,
                    startLocation,
                    startLocation + rayDirection * rayLength,
                    ObjectQueryParams
                );
               
                if (bHit)
                {
                    Debug::print("hitandstart");
                    // ��ȡ���㴦����ĸ������ΪĿ�����
                    AActor* HitActor = hitResult.GetActor();
                    if (HitActor)
                    {
                        Debug::Print("hitthewaLL!!");
                        // ��ȡ���㴦�ķ�������
                        FVector HitNormal = hitResult.Normal;

                        // �����ɫӦ�ø��ŵ�λ�ã�ʹ�������������棬����򵥵ؽ���ɫ�ƶ��������Ϸ�һ�㣬�ɸ���ʵ���������ϸ�ص���
                        FVector newlocation = hitResult.Location + HitNormal * 20;
                        Debug::Print("startclimb1");
                        // ���ý�ɫ��λ��
                        SetActorLocation(newlocation);

                        // ���ý�ɫ����ת��ʹ�����������������������������ķ���������������ת
                        FRotator newRotation = HitNormal.Rotation();
                         // �ɸ�����Ҫ���������Ƕȣ���������Ϊˮƽ����
                        Debug::print("rotation");
                        FRotator ra = GetActorRotation();
                        FString Score1 = FString::FromInt(ra.Pitch);
                        Debug::print(Score1);
                        Score1 = FString::FromInt(ra.Yaw);
                        Debug::print(Score1);
                        Score1 = FString::FromInt(ra.Roll);
                        Debug::print(Score1);
                        SetActorRotation(newRotation);
                        Score1 = FString::FromInt(ra.Pitch);
                        Debug::print(Score1);
                        Score1 = FString::FromInt(ra.Yaw);
                        Debug::print(Score1);
                        Score1 = FString::FromInt(ra.Roll);
                        Debug::print(Score1);
                        Debug::print("finishrotation");

                        // ��ȡ��ɫ���ƶ����
                        UCharacterMovementComponent* MovementComponent = GetCharacterMovement();

                        // ����������ص��ƶ����ԣ���ȡ������Ӱ�졢���������ٶȵ�
                        MovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
                        MovementComponent->GravityScale = 0.0f;
                        MovementComponent->MaxWalkSpeed = 50.0f;

                        //���ý�ɫ������״̬Ϊ��������
                        //bIsClimb = true;
                    }
                    else
                    {
                        // ���û�ܻ�ȡ����Ч�ı�����Actor��������Ҫ����һЩ����������򵥴�ӡ������Ϣ
                        UE_LOG(LogTemp, Error, TEXT("δ�ܻ�ȡ����Ч�ı�����Actor���޷���ɸ��ź��������á�"));
                    }
                }
            }
            else
            {
                bIsClimb = false;
                bIsStartClimb = 0;
            }
        }
    }
}

void APlayerBaseCharacter::EndClimb(const FInputActionValue& Value)
{

}

void APlayerBaseCharacter::StopClimb(const FInputActionValue& Value)
{
    if (!bIsDeath) {
        if (bIsClimb) {
            bIsClimbAndMove = 0;
        }
    }
}

void APlayerBaseCharacter::ClimbAndMove(const FInputActionValue& Value)
{
    if (!bIsDeath) {
        if (bIsClimb) {
            bIsClimbAndMove = 1;
        }
    }
}

void APlayerBaseCharacter::InputThrowBall(const FInputActionValue& Value)
{
    if (!bIsDeath) {
        if (!bIsThrowBall && weaponinhand == weaponclass::throwing) {

            
            speedindex = 1;


        }
        if (!bIsThrowBall) {
            bIsThrowBall = 1;
        }
    }
}

Faction APlayerBaseCharacter::GetFaction()
{
    return faction;
}

void APlayerBaseCharacter::RegisterInput()
{

    APlayerController* playerController = Cast<APlayerController>(Controller);
    if (playerController)
    {
        UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
        if (subsystem)
        {
            subsystem->AddMappingContext(DefaultMappingConext, 0);
        }
    }
}

void APlayerBaseCharacter::SpawnMeleeWeapon(FName name, TSubclassOf<AMeleeWeapon> WeaponClass)
{
    weapon = Cast<ABaseWeapon>(GetWorld()->SpawnActor<AMeleeWeapon>(WeaponClass));
    if (weapon) {
        Debug::Print("arm");
        FAttachmentTransformRules attachrules(EAttachmentRule::SnapToTarget, false);

        weapon->AttachToComponent(GetMesh(), attachrules, name);

        weapon->SetOwner(this);


    }
}

void APlayerBaseCharacter::SpawnBowWeapon(FName name, TSubclassOf<ABowWeapon> WeaponClass)
{
    weapon = Cast<ABaseWeapon>(GetWorld()->SpawnActor<ABowWeapon>(WeaponClass));
    if (weapon) {
        Debug::Print("arm");
        FAttachmentTransformRules attachrules(EAttachmentRule::SnapToTarget, false);

        weapon->AttachToComponent(GetMesh(), attachrules, name);

        weapon->SetOwner(this);


    }
}

void APlayerBaseCharacter::SpawnThrowWeapon(FName name, TSubclassOf<ABaseWeapon> WeaponClass)
{
    weapon = Cast<ABaseWeapon>(GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass));
    if (weapon) {
        Debug::Print("arm");
        FAttachmentTransformRules attachrules(EAttachmentRule::SnapToTarget, false);

        weapon->AttachToComponent(GetMesh(), attachrules, name);

        weapon->SetOwner(this);


    }
}



void APlayerBaseCharacter::ChangeWeapon1(const FInputActionValue& Value)
{
    if (!bIsDeath) {
        if (weapon) {
            Debug::Print("destory");
            weapon->Destroy();
            weapon = nullptr;
            //weapon = nullptr;
        }
        weaponinhand = weaponclass::none;
    }
}

void APlayerBaseCharacter::ChangeWeapon2(const FInputActionValue& Value)
{
    if (!bIsDeath) {
        if (weapon) {
            Debug::Print("destory");
            // if(!(weapon->Destroyed()))
            weapon->Destroy();
            weapon = nullptr;
        }
        SpawnMeleeWeapon("axe", Axe);
        weaponinhand = weaponclass::melee;
    }
}

void APlayerBaseCharacter::ChangeWeapon3(const FInputActionValue& Value)
{
    if (!bIsDeath) {
        if (weapon) {
            Debug::Print("destory");
            // if(!(weapon->Destroyed()))
            weapon->Destroy();
            weapon = nullptr;
        }

        SpawnMeleeWeapon("pickaxe", PickAxe);
        weaponinhand = weaponclass::melee;
    }
}

void APlayerBaseCharacter::ChangeWeapon4(const FInputActionValue& Value)
{
    if (!bIsDeath) {
        if (weapon) {
            Debug::Print("destory");
            // if(!(weapon->Destroyed()))
            weapon->Destroy();
            weapon = nullptr;
        }
        SpawnThrowWeapon("ball", RedBallInHand);
        weaponinhand = weaponclass::throwing;
    }
}
