// daHuang


#include "Actor/AuraProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

AAuraProjectile::AAuraProjectile()
{
	// 1. 关闭 Tick， projectile 一般靠 MovementComponent 推进，不需要每帧回调
	PrimaryActorTick.bCanEverTick = false;
	// 2. 打开网络同步：让服务器生成、客户端自动复制
	bReplicates = true;
	 /*--------------------------------------------------------------------------
        碰撞体：SphereComponent
        原则：只检测、不阻挡，避免把别人顶飞；用 Overlap 做“命中”
    --------------------------------------------------------------------------*/
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);// 根组件必须是碰撞体，否则 MovementComponent 会警告
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);// 先全部忽略
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);// 打玩家/敌人全靠这一行

 	/*--------------------------------------------------------------------------
        飞行组件：ProjectileMovementComponent
        负责“直线飞行 + 重力可选 + 速度同步”
    --------------------------------------------------------------------------*/
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.0f;// 初始速度
	ProjectileMovement->MaxSpeed = 550.0f; // 最大速度（想做成“加速箭”可再把 Max 提高）
	ProjectileMovement->ProjectileGravityScale = 0;// 0 = 纯直线，火球冰箭常用；弓箭可把这里调 0.5~1
}

/*------------------------------------------------------------------------------
    生命周期：服务器 & 客户端都会走
------------------------------------------------------------------------------*/
void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	// 动态绑定：当 Sphere 与任意组件发生 Overlap 时调用 OnSphereOverlap
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
}
/*------------------------------------------------------------------------------
    碰撞回调：只在服务器执行
    这里只打日志，具体“伤害/特效/销毁”请继承或 BlueprintImplementableEvent
------------------------------------------------------------------------------*/
void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
}
