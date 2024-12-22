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

    // 初始化输入相关动作
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

    // 初始化疾跑相关属性
    bIsSprint = false;
    SprintSpeedMultiplier = 1.5;

    //初始化蹲走相关属性
    CrouchSpeedMultiplier = 0.5;
    bIsCrouch = false;

    //初始化瞄准和攻击

    bIsAim = bIsAttack = 0;
    AttackSpeedMultiplier = 0.5;

    // 初始化Shift键按下状态为false
    bShiftKeyPressed = false;

    //初始化速度权重
    speedindex = 1;

    //初始化翻滚
    bIsRoll = 0;
    Resttime=20;//翻滚时间
    Rolltime = 62+Resttime;
    RollSpeedMutiplier =1000;
    
    //初始化攀爬
    bIsClimb = 0;
    ClimbSpeedMutipulier = 0.3;
    bIsStartClimb = 0;
    bIsEndClimb = 0;
    EndClimbTime =EndClimbTimer= 41;//攀爬结束动画共41帧
    changetoClimb = jumpuptime;
    ObjectToClimb = nullptr;
    //初始化跳跃
    //起跳动画共18帧
    jumpuptime = 18;
    // 初始化用于动画蓝图的当前移动速度
    CurrentMovementSpeed = 0.0f;

    //初始化武器
    weaponinhand = 1;

    throwdelay = 0;
    //体力条
    MaxHP = 500;
    notTired=HitPoint = MaxHP;
    tired = 0;
    
    //初始化血量和血量上线
   
    MaxHealth = 99999999;

    health = MaxHealth;
    //初始化死亡状态
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

        
        // 绑定Shift键按下和释放事件
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
    // 更新当前移动速度
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
        //获得角色朝向
        FVector Forward = GetActorForwardVector();
        FVector GroundNormal = FVector::UpVector;
        //获取水平分量
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
    // 如果角色正在移动，根据移动方向设置角色朝向

    if (!bIsClimb && !bIsStartClimb) {   
        velocity.Z = 0; //只要要沿着水平方向的分量
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
    //判断是否疲劳
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
    //恢复体力
    if (!bIsAttack && HitPoint < MaxHP&& !bShiftKeyPressed&&!bIsClimb) {//体力没满且不按shift且不攻击
        HitPoint++;
    }
    else if (((bIsSprint&& CurrentMovementSpeed)||bIsAttack||bIsClimbAndMove)&&!tired) {//处于疾跑状态并且正在移动
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
   //进行开始攀爬的动作
    
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
        //速度置零
        if (!bIsStartClimb) {
            movement->Velocity.X = movement->Velocity.Y
                = movement->Velocity.Z = 0;
            bIsStartClimb = 1;
        }
        UWorld* World = GetWorld();

        // 定义射线起点，这里以Actor自身位置为例，可根据实际情况调整为更合适的位置，比如角色的手脚位置等
        FVector StartLocationWaist = GetActorLocation();
        FVector StartLocationHead = StartLocationWaist;

        // 获取胶囊体组件
        UCapsuleComponent* topsize = GetCapsuleComponent();
        // 获取角色头顶高度
        auto height = topsize->GetScaledCapsuleHalfHeight();
        // 更新头顶射线的位置
        StartLocationHead.Z += height;

        // 定义射线方向，这里假设沿着Actor的正前方（X轴正方向），可根据实际需求修改为不同方向的检测射线
        FVector rayDirection = GetActorForwardVector();

        // 射线长度（范围），可根据实际情况调整
        float rayLength = 100.0f;

        // 设置碰撞通道，这里使用ECC_WorldStatic表示只检测静态物体，可根据需要添加其他碰撞通道以检测不同类型的物体
        FCollisionObjectQueryParams ObjectQueryParams(ECollisionChannel::ECC_WorldStatic);

        // 执行射线检测
        FHitResult hitResultWaist;

        // 腰部射线检测
        bool bHitwaist = World->LineTraceSingleByObjectType(
            hitResultWaist,
            StartLocationWaist,
            StartLocationWaist + rayDirection * rayLength,
            ObjectQueryParams
        );
        
        if ( bHitwaist)
        {
            // 如果命中物体，获取命中物体的相关信息
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
            // 获取命中点的法线向量
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
            // 获取角色的前向向量
            FVector ForwardVector = GetActorForwardVector();
            // 获取角色的上向向量
            FVector UpVector = GetActorUpVector();
            // 计算向上的位移向量
            FVector UpDisplacement = UpVector * ((CapsuleComp->GetScaledCapsuleHalfHeight() * 2 + 1) / EndClimbTime);
            // 组合位移向量
            FVector Displacement = UpDisplacement;
            // 将位移向量添加到当前位置
            location += Displacement;
            SetActorLocation(location);
        }
        else {
            // 获取角色的前向向量
            FVector ForwardVector = GetActorForwardVector();
            // 获取角色的上向向量

            FVector ForwardDisplacement = ForwardVector * (CapsuleComp->GetScaledCapsuleRadius() + 2);
            // 计算向上的位移向量
            Debug::print("forward");
            FVector Displacement = ForwardDisplacement;
            // 将位移向量添加到当前位置
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
    //进行投掷
    
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


// 获取摄像机前向方向向量的函数实现
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

// 获取摄像机右向方向向量的函数实现
FVector APlayerBaseCharacter::GetCameraRightDirection()
{
    FVector cameraForward = GetCameraForwardDirection();
    return cameraForward.RotateAngleAxis(90.0f, FVector::UpVector);
}
void APlayerBaseCharacter::LookRotation(const FInputActionValue& Value)
{
    FVector2D axis2D = Value.Get<FVector2D>();

    // 鼠标移动时只改变摄像机视角，不影响角色朝向
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

                // 通过当前角色实例获取CharacterMovementComponent
                UCharacterMovementComponent* movement = this->GetCharacterMovement();
                // 确保可以不受平面限制进行攀爬移动
                movement->bConstrainToPlane = false;

                if (movement)
                {
                    auto position = GetActorLocation();
                    //获取胶囊体
                    UCapsuleComponent* topsize = GetCapsuleComponent();
                    //获取角色头顶高度
                    float height = topsize->GetScaledCapsuleHalfHeight();
                    //射线长度
                    float rayLength = 50.0f;
                    //确定射线起点
                    FVector startLocation = position;
                    startLocation.Z += height;

                    // 定义射线方向，这里设置为向上（Z轴正方向）
                    double angle = FMath::DegreesToRadians(-Value.Get<float>() * 30);
                    // 获取角色指针的方式

                   //FVector LocalVector = RotationMatrix.TransformVector(rayDirection);
                    FVector rayDirection = GetActorForwardVector();
                    // 设置碰撞通道，这里使用ECC_WorldStatic表示只检测静态物体，你可以根据实际需求修改
                    FCollisionObjectQueryParams ObjectQueryParams(ECollisionChannel::ECC_WorldStatic);

                    // 执行射线检测
                    FHitResult hitResult1;
                    FHitResult hitResult2;
                    bool bHit2 = World->LineTraceSingleByObjectType(
                        hitResult2,
                        startLocation,
                        startLocation + rayDirection * rayLength,
                        ObjectQueryParams
                    );

                    rayDirection = rayDirection.RotateAngleAxis(angle, GetActorRightVector());

                    //进行射线检测
                    bool bHit1 = World->LineTraceSingleByObjectType(
                        hitResult1,
                        startLocation,
                        startLocation + rayDirection * rayLength,
                        ObjectQueryParams
                    );

                    if (bHit1)
                    {
                        LastResult = hitResult1;
                        // 得到交点处的法线向量，用于后续确定攀爬方向和位置调整
                        FVector HitNormal1 = hitResult1.Normal;
                        FVector HitNormal2 = hitResult2.Normal;

                        // 计算角色应该攀爬的目标位置，这里简单地将角色移动到交点上方一点，使其看起来像是在攀爬
                        // 可以根据实际需求更精细地调整这个偏移量，比如根据角色的动作姿态等
                        FVector TargetLocation1 = hitResult1.Location + HitNormal1 * 40;
                        FVector TargetLocation2 = hitResult2.Location + HitNormal2 * 40;

                        // 画出调试射线
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

                        //// 更新角色位置
                        //SetActorLocation(TargetLocation);
                        // 可以在这里添加更多逻辑来处理攀爬过程中的动画播放、速度控制等
                        // 例如，设置攀爬速度
                        movement->MaxWalkSpeed = 20.0f;
                        // 根据攀爬情况更新角色的状态等
                        // 比如设置一个表示正在攀爬的布尔变量等，以便在其他函数中根据这个状态进行相应处理
                        
                    }
                    else
                    {
                        //画出调试射线
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

                // 通过当前角色实例获取CharacterMovementComponent
                UCharacterMovementComponent* movement = this->GetCharacterMovement();
                // 确保可以不受平面限制进行攀爬移动
                movement->bConstrainToPlane = false;

                if (movement)
                {
                    auto position = GetActorLocation();
                    //获取胶囊体
                    UCapsuleComponent* topsize = GetCapsuleComponent();
                    //获取角色头顶高度
                    float height = topsize->GetScaledCapsuleHalfHeight();
                    //射线长度
                    float rayLength = 50.0f;
                    //确定射线起点
                    FVector startLocation = position;
                    startLocation.Z += height;

                    double angle = FMath::DegreesToRadians(-Value.Get<float>() * 20);

                    // 获取角色指针的方式
                    FVector rayDirection = GetActorForwardVector();

                    // 设置碰撞通道，这里使用ECC_WorldStatic表示只检测静态物体，你可以根据实际需求修改
                    FCollisionObjectQueryParams ObjectQueryParams(ECollisionChannel::ECC_WorldStatic);

                    // 执行射线检测
                    FHitResult hitResult1;
                    FHitResult hitResult2;
                    bool bHit2 = World->LineTraceSingleByObjectType(
                        hitResult2,
                        startLocation,
                        startLocation + rayDirection * rayLength,
                        ObjectQueryParams
                    );

                    rayDirection = rayDirection.RotateAngleAxis(angle, GetActorUpVector());

                    //进行射线检测
                    bool bHit1 = World->LineTraceSingleByObjectType(
                        hitResult1,
                        startLocation,
                        startLocation + rayDirection * rayLength,
                        ObjectQueryParams
                    );

                    if (bHit1)
                    {
                        // 得到交点处的法线向量，用于后续确定攀爬方向和位置调整
                        FVector HitNormal1 = hitResult1.Normal;
                        FVector HitNormal2 = hitResult2.Normal;

                        // 计算角色应该攀爬的目标位置，这里简单地将角色移动到交点上方一点，使其看起来像是在攀爬
                        // 可以根据实际需求更精细地调整这个偏移量，比如根据角色的动作姿态等
                        FVector TargetLocation1 = hitResult1.Location + HitNormal1 * 40;
                        FVector TargetLocation2 = hitResult2.Location + HitNormal2 * 40;

                        // 画出调试射线
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

                        //// 更新角色位置
                        //SetActorLocation(TargetLocation);
                        // 可以在这里添加更多逻辑来处理攀爬过程中的动画播放、速度控制等
                        // 例如，设置攀爬速度
                        movement->MaxWalkSpeed = 20.0f;
                        // 根据攀爬情况更新角色的状态等
                        // 比如设置一个表示正在攀爬的布尔变量等，以便在其他函数中根据这个状态进行相应处理
                        //bIsClimb = true;
            //            // 检查是否到达攀爬目标位置（这里简单示例，可根据实际情况完善）
            //            //if (FVector::Distance(GetActorLocation(), TargetLocation) < 10.f)
            //            //{
            //            //    // 如果接近目标位置，可设置相关状态表示攀爬完成等
            //            //    bIsClimb = false;
            //            //    bIsStartClimb = 0;
            //            //    movement->bConstrainToPlane = true;
            //            //    movement->MaxWalkSpeed = 600.0f;
            //            //}
            //        }
                    }
                    else
                    {
                        //画出调试射线
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
                //            // 如果没有检测到可攀爬物体，恢复角色的正常移动状态相关设置
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
        // 获取世界
        UWorld* World = GetWorld();
        if (World == nullptr)
        {
            return false;
        }

        // 定义射线起点，这里以Actor自身位置为例，可根据实际情况调整为更合适的位置，比如角色的手脚位置等
        FVector StartLocationWaist = GetActorLocation();
        FVector StartLocationHead = StartLocationWaist;

        // 获取胶囊体组件
        UCapsuleComponent* topsize = GetCapsuleComponent();
        // 获取角色头顶高度
        auto height = topsize->GetScaledCapsuleHalfHeight();
        // 更新头顶射线的位置
        StartLocationHead.Z += height;

        // 定义射线方向，这里假设沿着Actor的正前方（X轴正方向），可根据实际需求修改为不同方向的检测射线
        FVector rayDirection = GetActorForwardVector();

        // 射线长度（范围），可根据实际情况调整
        float rayLength = 200.0f;

        // 设置碰撞通道，这里使用ECC_WorldStatic表示只检测静态物体，可根据需要添加其他碰撞通道以检测不同类型的物体
        FCollisionObjectQueryParams ObjectQueryParams(ECollisionChannel::ECC_WorldStatic);

        // 执行射线检测
        FHitResult hitResultHead;
        FHitResult hitResultWaist;

        // 腰部射线检测
        bool bHitwaist = World->LineTraceSingleByObjectType(
            hitResultWaist,
            StartLocationWaist,
            StartLocationWaist + rayDirection * rayLength,
            ObjectQueryParams
        );

        // 头部射线检测
        bool bHithead = World->LineTraceSingleByObjectType(
            hitResultHead,
            StartLocationHead,
            StartLocationHead + rayDirection * rayLength,
            ObjectQueryParams
        );

        if (bHithead && bHitwaist)
        {
            // 如果命中物体，获取命中物体的相关信息
            AActor* HitActorHead = hitResultHead.GetActor();
            AActor* HitActorWaist = hitResultWaist.GetActor();

            // 获取命中点的法线向量
            FVector HitNormalHead = hitResultHead.Normal;
            FVector HitNormalWaist = hitResultWaist.Normal;

            // 检查攀爬表面的坡度是否在可攀爬范围内，这里假设可攀爬坡度范围为 [-45, 45] 度，可根据实际需求调整
            float angleThreshold = 45.0f;
            float angleHead = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(FVector::UpVector, HitNormalHead)));
            float angleWaist = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(FVector::UpVector, HitNormalWaist)));

            if (FMath::Abs(angleHead) >= angleThreshold && FMath::Abs(angleWaist) >= angleThreshold)
            {
                // 获取到当前正在爬的物体
                ObjectToClimb = HitActorHead;

                /*if (GEngine)
                {
                    FString DebugMessageHead = FString::Printf(TEXT("Raycast hit actor (head): %s"), *HitActorHead->GetName());
                    FString DebugMessageWaist = FString::Printf(TEXT("Raycast hit actor (waist): %s"), *HitActorWaist->GetName());
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, DebugMessageHead);
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, DebugMessageWaist);
                }*/

                // 绘制调试射线（只在开发阶段显示）
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
                // 如果坡度不在可攀爬范围内，打印提示信息并绘制未命中的调试射线（可选）
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
            // 如果未命中物体，绘制未命中的调试射线（可选）
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
        //冲刺不能攻击
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
        //冲刺不能瞄准
        //近战武器不能瞄准
        if (!bIsAim && !bIsSprint && !bIsClimb && weaponinhand != weaponclass::melee) {
            bIsAim = 1;
            if (bIsAim) {
                Debug::print("aim");
                speedindex = AttackSpeedMultiplier;
                // 获取玩家控制器控制的角色
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
            // 获取当前角色的世界变换
            FTransform CurrentTransform = GetActorTransform();

            // 获取当前角色的旋转（以FRotator形式）
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
        //朝向角色头顶爬取
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

                // 获取世界指针
                UWorld* World = GetWorld();
                if (World == nullptr)
                {
                    return;
                }
               
                // 进行射线检测相关设置
                float rayLength = 200.0f; // 射线长度，可根据实际情况调整
                FVector startLocation = GetActorLocation();
                startLocation.Z += GetCapsuleComponent()->GetScaledCapsuleHalfHeight(); // 从角色头顶上方一点作为射线起点
                // 射线方向为角色正前方
                FVector rayDirection = GetActorForwardVector();
                FCollisionObjectQueryParams ObjectQueryParams(ECollisionChannel::ECC_WorldStatic);

                // 执行射线检测
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
                    // 获取交点处物体的根组件作为目标组件
                    AActor* HitActor = hitResult.GetActor();
                    if (HitActor)
                    {
                        Debug::Print("hitthewaLL!!");
                        // 获取交点处的法线向量
                        FVector HitNormal = hitResult.Normal;

                        // 计算角色应该附着的位置，使其贴合攀爬表面，这里简单地将角色移动到交点上方一点，可根据实际需求更精细地调整
                        FVector newlocation = hitResult.Location + HitNormal * 20;
                        Debug::Print("startclimb1");
                        // 设置角色的位置
                        SetActorLocation(newlocation);

                        // 设置角色的旋转，使其面向攀爬方向，这里根据攀爬表面的法线向量来设置旋转
                        FRotator newRotation = HitNormal.Rotation();
                         // 可根据需要调整俯仰角度，这里设置为水平方向
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

                        // 获取角色的移动组件
                        UCharacterMovementComponent* MovementComponent = GetCharacterMovement();

                        // 设置攀爬相关的移动属性，如取消重力影响、设置攀爬速度等
                        MovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
                        MovementComponent->GravityScale = 0.0f;
                        MovementComponent->MaxWalkSpeed = 50.0f;

                        //设置角色的攀爬状态为正在攀爬
                        //bIsClimb = true;
                    }
                    else
                    {
                        // 如果没能获取到有效的被击中Actor，可能需要进行一些错误处理，这里简单打印错误信息
                        UE_LOG(LogTemp, Error, TEXT("未能获取到有效的被击中Actor，无法完成附着和攀爬设置。"));
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
