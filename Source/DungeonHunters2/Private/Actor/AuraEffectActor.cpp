// daHuang


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

// #include "AbilitySystemComponent.h"
// #include "AbilitySystemInterface.h"
// #include "AbilitySystem/AureAttributeSet.h"
// #include "Components/SphereComponent.h"

// 初始化需要碰撞的物品
AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;// 禁用 Tick 函数（不需要每帧更新）
	// Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");// 创建静态网格组件
	// SetRootComponent(Mesh);// 将 Mesh 设为根组件
	// Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");// 创建球形碰撞组件
	// Sphere->SetupAttachment(GetRootComponent());// 将碰撞组件附着到根组件（Mesh）上

	this->SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
	
}

/**
 * UPrimitiveComponent* UOverlappedComponent:物品的碰撞胶囊体
 * AActor* OtherActor:这里指玩家,
 * UPrimitiveComponent* OtherComp::这里指玩家,碰撞胶囊体
 * int32 OtherBodyIndex：对方碰撞组件中具体碰撞体的索引（适用于包含多个子碰撞体的组件），这里可以是玩家身上的多部位，如果有多个碰撞体，如“头部”“躯干”“手臂” 等多个碰撞体
 * bool bFromSweep：含义：指示碰撞是否由 “移动扫描（Sweep）” 触发。true：碰撞是因某个 Actor 移动时扫描到障碍物（如角色走路时撞到这个 “治疗光球”）。false：碰撞是因静态重叠（如 “治疗光球” 生成时直接与角色重叠）。
 * 
 * const FHitResult& SweepResult：包含碰撞的详细物理信息的结构体，用于需要精确位置的逻辑（如在碰撞点播放特效、生成粒子）。例如，你可以在 SweepResult.ImpactPoint 处 spawn 一个 “治疗闪光” 特效。
 * 								如，ImpactPoint：碰撞发生的世界坐标点（如 “治疗光球” 与玩家碰撞的精确位置）
 * 								如，ImpactNormal：碰撞点的法线方向（如碰撞表面的垂直方向）
 * 								如，Actor：与 OtherActor 一致，指向碰撞的对方 Actor。
 * */
// void AAuraEffectActor::OnOverlap(UPrimitiveComponent* UOverlappedComponent, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	// OtherActor 玩家
// 	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
// 	{
// 		UAbilitySystemComponent* AbilitySystemComponent = ASCInterface->GetAbilitySystemComponent();
// 		const UAureAttributeSet* AureAttributeSet = Cast<UAureAttributeSet>(AbilitySystemComponent->GetAttributeSet(UAureAttributeSet::StaticClass()));
//
// 		UAureAttributeSet* MutableAuraAttriButeSet = const_cast<UAureAttributeSet*>(AureAttributeSet);
// 		//修改属性，出方式不会触发回调 UOverlayWidgetController::HealthChanged
// 		MutableAuraAttriButeSet->SetHealth(AureAttributeSet->GetHealth()+25.f);
// 		MutableAuraAttriButeSet->SetMana(AureAttributeSet->GetMana()-25.f);
//
// 		/**
// 		 * // 触发回调的方  式：使用ASC的SetNumericAttributeBase接口
// 			AbilitySystemComponent->SetNumericAttributeBase(
// 				UAureAttributeSet::GetHealthAttribute(),  // 属性标识
// 				AureAttributeSet->GetHealth() + 25.f      // 新值
// 			);
// 		 */
// 		Destroy();
// 	}
// 	
// }
//
// void AAuraEffectActor::EndOverlap(UPrimitiveComponent* UOverlappedComponent, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
// {
// 	
// }

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	/***
	 * OnComponentBeginOverlap是 OnComponentBeginOverlap 是 Unreal 内置的动态多播委托，
	 * DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(
		FComponentBeginOverlapSignature,
		UPrimitiveComponent*, OverlappedComponent,
		AActor*, OtherActor,
		UPrimitiveComponent*, OtherComp,
		int32, OtherBodyIndex,
		bool, bFromSweep,
		const FHitResult&, SweepResult
	);
	 */
	// Sphere->OnComponentBeginOverlap.AddDynamic(this,&AAuraEffectActor::OnOverlap);
	// Sphere->OnComponentEndOverlap.AddDynamic(this,&AAuraEffectActor::EndOverlap);
}

/**
 * Target: 当前的AAuraEffectActor实例，如血瓶,蓝瓶之类。
 * UGameplayEffect: 是效果的 “蓝图模板”，定义了效果的类型（瞬时 / 持续 / 周期性）、影响的属性（如加血、减蓝）、数值规则（如 + 50 血、每秒 - 10 血）等静态配置。
 * 					指定基于哪个 UGameplayEffect 模板生成规格（例如 “治疗效果”“中毒效果”）。
 */
void AAuraEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// 1. 获取目标的 AbilitySystemComponent（ASC）
	//只有实现了 IAbilitySystemInterface 的 Actor（如玩家 AAuraCharacter、敌人 AAuraEnemy）才会拥有 ASC
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (!TargetASC) return;
	// 2. 校验效果模板有效性
	check(GameplayEffectClass);
	// 3. 创建效果上下文（记录效果来源等信息）FGameplayEffectContextHandle
	//作用：上下文用于存储效果的额外元数据，如效果的来源（哪个 Actor 施加的）、触发方式（碰撞 / 技能等），方便后续逻辑追溯（如伤害来源判定、效果来源显示）。
	//实现：EffectContextHandle.AddSourceObject(this) 将当前 AAuraEffectActor 设为效果来源，后续可通过上下文获取该信息,
	//	   例如在日志中记录 “治疗光球（AuraEffectActor）为玩家施加了加血效果”）。
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);// 记录效果来源是当前 AuraEffectActor
	// 4. 生成效果规格并应用到目标
	// MakeOutgoingSpec 会基于这个 UGameplayEffect 模板，生成一个包含动态参数的 “实例规格”（FGameplayEffectSpec），相当于 “根据模板创建一个具体的执行计划”
	// Level：效果的等级（代码中是 1.f），用于动态调整效果强度（例如等级 2 的治疗量是等级 1 的 2 倍，模板中可通过 Level 变量定义数值规则）。
	// EffectContextHandle：效果上下文（FGameplayEffectContextHandle），包含效果的来源、触发方式等元数据（如 “这个治疗效果来自哪个 AuraEffectActor”）。
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass,1.f,EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}



