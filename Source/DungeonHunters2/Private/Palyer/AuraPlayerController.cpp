// daHuang
// 组件化设计
#include "Palyer/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnenmyInterface"

AAuraPlayerController::AAuraPlayerController()
{
	//bReplicates = true：
	//启用网络复制。在多人游戏中，该控制器的状态会从服务器同步到客户端，确保输入和玩家状态在网络中一致。
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//检查是否有绑定，失败则崩溃
	check(AuraContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	// check(Subsystem);单人游戏
	// 将输入映射上下文（AuraContext）添加到子系统，优先级为0（数值越小优先级越高）
	if (Subsystem)//多人游戏
	{
		Subsystem->AddMappingContext(AuraContext,0);
	}
	

	bShowMouseCursor = true;//显示鼠标光标
	DefaultMouseCursor = EMouseCursor::Default;// 设置默认鼠标光标样式
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);// 不锁定鼠标到视口
	InputModeData.SetHideCursorDuringCapture(false);// 捕获输入时不隐藏光标
	SetInputMode(InputModeData);// 应用输入模式
	
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	check(EnhancedInputComponent);

	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVecter = InputActionValue.Get<FVector2D>();
	const FRotator Rotator = GetControlRotation();
	const FRotator YawRotator(0.f,Rotator.Yaw,0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);

	if (APawn* PawnController = GetPawn<APawn>())
	{
		PawnController->AddMovementInput(ForwardDirection,InputAxisVecter.X);
		PawnController->AddMovementInput(RightDirection,InputAxisVecter.Y);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility,false,CursorHit);
	if (!CursorHit.bBlockingHit) return;
	LastActor = ThisActor;
	ThisActor = Cast<IEnenmyInterface>(CursorHit.GetActor());
	/**
	从游标开始行跟踪。有几种情况：
	*1,LastActor是空的&& ThisActor是空
		-什么都不做
	2,LastActor是空的&& ThisActor是有效的
		- Highlight ThisActor
	3,LastActor是有效的&& ThisActor是空的
		-取消突出最后一个演员
	4,两个actor都是有效的，但是LastActor != ThisActor
		-取消突出LastActor，并突出ThisActor
	5,两个actor都是有效的，并且是同一个actor
		-什么都不做
	 */
	if (ThisActor!=nullptr)
	{
		printf("");
	}
	if (LastActor==nullptr)
	{
		if (ThisActor!=nullptr)
		{
			//2
		this->ThisActor->HighlightActor();	
		}else
		{
			//1
		}
	}else
	{
		if (ThisActor==nullptr)//指向当前
		{
			//3
			LastActor->UnHighlightActor();
		}
		else
		{
			if (ThisActor!=LastActor)
			{
				//4
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			
			}
			else
			{
				//5
			}
		}
		
		
		
	}
	
	
}
