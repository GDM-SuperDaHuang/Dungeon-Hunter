// daHuang


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

/**
 * 生成投射物的具体逻辑
 * 1. 检查权限（仅服务器生成投射物，避免客户端重复生成）
 * 2. 获取发射位置（通过战斗接口从武器 socket 获取）
 * 3. 生成投射物并设置初始状态
 */
void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	// 仅在服务器端生成投射物（网络同步：确保多端一致性，避免客户端各自生成）
	bool bHasAuthority = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bHasAuthority) return;

	// 获取施法者的战斗接口（用于获取武器发射点等战斗相关数据）
	// ICombatInterface由角色类实现，提供GetCombatSocketLocation等战斗相关方法
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		// 从武器的特定socket（如"WeaponTip"）获取发射位置
		const FVector CombatSocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - CombatSocketLocation).Rotation();
		Rotation.Pitch = 0.f;


		//射击 ,并且在武器处产生
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(CombatSocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());


		// 延迟生成投射物（Deferred Spawn）：先创建实例但不初始化，可在初始化前设置属性
		/**
		 * SpawnActor → 碰撞体立即生效 → 可能 与生成者重叠 → 引擎报错。
		 * SpawnActorDeferred → 碰撞体、组件、BeginPlay 都不激活 → 给你时间 设置属性 → 再 Finish。
		 */
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//伤害计算
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
			GetAvatarActorFromActorInfo());\
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
			DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
		FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		// 根据配置表等级获取对应的伤害值
		const float ScaledDame = Damage.GetValueAtLevel(GetAbilityLevel());
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Damage: %f"),ScaledDame));

		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, ScaledDame);
		Projectile->DamageEffectHandle = SpecHandle;


		// SpawnActorDeferred需要FinishSpawning， 完成投射物生成（应用之前的设置，触发AAuraProjectile的BeginPlay）
		Projectile->FinishSpawning(SpawnTransform);
	};
}
