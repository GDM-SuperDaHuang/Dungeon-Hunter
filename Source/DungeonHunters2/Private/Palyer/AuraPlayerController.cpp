// daHuang
// 组件化设计
#include "Palyer/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AureAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnenmyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	//bReplicates = true：
	//启用网络复制。在多人游戏中，该控制器的状态会从服务器同步到客户端，确保输入和玩家状态在网络中一致。
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//检查是否有绑定，失败则崩溃
	check(AuraContext);

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

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // 不锁定鼠标到视口
	InputModeData.SetHideCursorDuringCapture(false); // 捕获输入时不隐藏光标
	SetInputMode(InputModeData); // 应用输入模式
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	check(AuraInputComponent);

	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
	                                       &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(
			ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		FVector Direction = Spline->
			FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}


void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVecter = InputActionValue.Get<FVector2D>();
	const FRotator Rotator = GetControlRotation();
	const FRotator YawRotator(0.f, Rotator.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);

	if (APawn* PawnController = GetPawn<APawn>())
	{
		PawnController->AddMovementInput(ForwardDirection, InputAxisVecter.X);
		PawnController->AddMovementInput(RightDirection, InputAxisVecter.Y);
	}
}


void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;
	LastActor = ThisActor;
	ThisActor = Cast<IEnenmyInterface>(CursorHit.GetActor());

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

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(1, 3, FColor::Yellow, *InputTag.ToString());

	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
	}

	if (GetASC())
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}
	
	if (!bTargeting && !bShiftKeyDown)
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
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
				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				bAutoRunning = true;
			}
		}
		FollowTime = 0.0f;
		bTargeting = false;
	}
	GEngine->AddOnScreenDebugMessage(2, 3, FColor::Red, *InputTag.ToString());
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}

	if (bTargeting || bShiftKeyDown)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
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

UAureAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AureAbilitySystemComponent == nullptr)
	{
		AureAbilitySystemComponent = Cast<UAureAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AureAbilitySystemComponent;
}
