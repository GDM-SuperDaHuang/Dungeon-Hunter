// daHuang
// 组件化设计
#include "Palyer/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "MovieSceneTracksComponentTypes.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AureAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnenmyInterface.h"
#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	//bReplicates = true：
	//启用网络复制。在多人游戏中，该控制器的状态会从服务器同步到客户端，确保输入和玩家状态在网络中一致。
	bReplicates = true;
	// 创建样条组件，用来记录导航路径点，供 AutoRun 使用
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter,
                                                            bool bBlockedHit,
                                                            bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(),
		                              FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		//???
		FVector SpawnLoc = TargetCharacter->GetActorLocation();
		SpawnLoc.Z += 10.f; // 离地 0.1 米
		DamageText->SetWorldLocation(SpawnLoc);

		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//检查是否有绑定，失败则崩溃
	check(AuraContext);

	// 拿到本地玩家的 EnhancedInput 子系统，用于动态挂/卸映射上下文
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	// check(Subsystem);单人游戏
	// 将输入映射上下文（AuraContext）添加到子系统，优先级为0（数值越小优先级越高）
	if (Subsystem) //多人游戏
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}


	bShowMouseCursor = true; //显示鼠标光标
	DefaultMouseCursor = EMouseCursor::Default; // 设置默认鼠标光标样式

	// 输入模式：既响应游戏（WASD）也响应 UI（点击 Widget）
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // 不锁定鼠标到视口
	InputModeData.SetHideCursorDuringCapture(false); // 捕获输入时不隐藏光标
	SetInputMode(InputModeData); // 应用输入模式
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 我们自定义的 UAuraInputComponent 在 BP 里已经挂载，直接强转
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	check(AuraInputComponent);

	// 普通移动轴绑定
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	// Shift 按下/松开（用来区分“强制施法”与“点地移动”）
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);

	// 一键批量绑定所有“技能输入 Tag”到三个回调
	AuraInputComponent->BindAbilityActions(
		InputConfig, // 数据资产里配了 IA <-> Tag 表
		this,
		&ThisClass::AbilityInputTagPressed,
		&ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
}

// 每帧 Tick 里执行：沿样条移动，直到到达终点
void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		// 找到离 Pawn 最近的样条点
		FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(
			ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		FVector Direction = Spline->
			FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		// 到达终点附近则停
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}


void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// 增强输入默认返回 FVector2D
	const FVector2D InputAxisVecter = InputActionValue.Get<FVector2D>();

	//返回 摄像机或控制器 的 世界旋转
	const FRotator Rotator = GetControlRotation();
	// 提取 Yaw 旋转，忽略 Pitch/Roll
	const FRotator YawRotator(0.f, Rotator.Yaw, 0.f); //水平面朝向（俯仰角清零），防止 上坡/下坡 时 前后方向错位。

	// 把二维输入映射到世界空间的前/右方向
	const FVector ForwardDirection = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);

	if (APawn* PawnController = GetPawn<APawn>())
	{
		PawnController->AddMovementInput(ForwardDirection, InputAxisVecter.X);
		PawnController->AddMovementInput(RightDirection, InputAxisVecter.Y);
	}
}

/* ---------- 光标扫描 – 高亮敌人 ---------- */
void AAuraPlayerController::CursorTrace()
{
	// 只检测 Visibility 通道；false=不复杂轨迹
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;
	// 接口转换：当前/上一帧指向的敌人
	LastActor = ThisActor;
	ThisActor = Cast<IEnenmyInterface>(CursorHit.GetActor());

	// 状态变化才调用接口，减少开销
	if (LastActor != ThisActor)
	{
		if (LastActor)
		{
			LastActor->UnHighlightActor();
		}
		if (ThisActor)
		{
			ThisActor->HighlightActor();
		}
	}
	// /**
	// 从游标开始行跟踪。有几种情况：
	// *1,LastActor是空的&& ThisActor是空
	// 	-什么都不做
	// 2,LastActor是空的&& ThisActor是有效的
	// 	- Highlight ThisActor
	// 3,LastActor是有效的&& ThisActor是空的
	// 	-取消突出最后一个演员
	// 4,两个actor都是有效的，但是LastActor != ThisActor
	// 	-取消突出LastActor，并突出ThisActor
	// 5,两个actor都是有效的，并且是同一个actor
	// 	-什么都不做
	//  */
	// if (ThisActor != nullptr)
	// {
	// 	printf("");
	// }
	// if (LastActor == nullptr)
	// {
	// 	if (ThisActor != nullptr)
	// 	{
	// 		//2
	// 		this->ThisActor->HighlightActor();
	// 	}
	// 	else
	// 	{
	// 		//1
	// 	}
	// }
	// else
	// {
	// 	if (ThisActor == nullptr) //指向当前
	// 	{
	// 		//3
	// 		LastActor->UnHighlightActor();
	// 	}
	// 	else
	// 	{
	// 		if (ThisActor != LastActor)
	// 		{
	// 			//4
	// 			LastActor->UnHighlightActor();
	// 			ThisActor->HighlightActor();
	// 		}
	// 		else
	// 		{
	// 			//5
	// 		}
	// 	}
	// }
}

/* ---------- 6. 技能输入 – 与 GAS 联动 ---------- */
void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	// GEngine->AddOnScreenDebugMessage(1, 3, FColor::Yellow, *InputTag.ToString());

	// 仅左键需要特殊处理：判断是否指向敌人
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
	if (GetASC())
	{
		GetASC()->AbilityInputTagPressed(InputTag);
	}
}

// 松开：区分“短按 / 长按 / 强制施法”三种路线
void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	// 非左键技能：直接走 ASC 的标准输入释放
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}

	if (GetASC())
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}

	// 左键技能：
	// 1. 如果处于瞄准状态 或 按住 Shift → 强制施法
	// 2. 否则判定为“点地移动”：短按就寻路，长按忽略
	if (!bTargeting && !bShiftKeyDown)
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			// 同步寻路，返回完整路径点
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
				this, ControlledPawn->GetActorLocation(),
				CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (FVector PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), PointLoc, 8, 8, FColor::Yellow, false, 5);
				}

				if (NavPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					bAutoRunning = true;
				}
			}

			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ClickNiagaraSystem,CachedDestination);
		}
		FollowTime = 0.0f;
		bTargeting = false;
	}
	GEngine->AddOnScreenDebugMessage(2, 3, FColor::Red, *InputTag.ToString());
}

// 按住：每帧触发；技能需要“蓄力”或“连续发射”时在这里处理
void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// 非左键：直接转发
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}

	// 左键：
	// 1. 瞄准 或 Shift → 施法
	// 2. 否则累计 FollowTime，实时朝鼠标方向移动（类似 Diablo 拖拽）
	if (bTargeting || bShiftKeyDown)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);//????
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		// FHitResult Hit;

		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}
		if (APawn* ControlledPawn = GetPawn<APawn>())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}

	GEngine->AddOnScreenDebugMessage(3, 3, FColor::Green, *InputTag.ToString());
}

// 懒加载获取 ASC：第一次调用时缓存，失败返回空
UAureAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AureAbilitySystemComponent == nullptr)
	{
		AureAbilitySystemComponent = Cast<UAureAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AureAbilitySystemComponent;
}
