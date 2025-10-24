// daHuang


#include "AbilitySystem/AureAbilitySystemComponent.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "AuraGameplayTags.h"

void UAureAbilitySystemComponent::AbilityActorInfoSet()
{
	UE_LOG(LogTemp, Log, TEXT("UAureAbilitySystemComponent created: %p"), this);
	//
	//
	/**
	 * 给自己注册一个回调ClientEffectApplied_Implementation：每当“本角色”被施加 GameplayEffect 时触发
	 * 当前 UAureAbilitySystemComponent 所属的 Actor 被应用 GameplayEffect（游戏性效果）时自动触发
	 * 如，ApplyGameplayEffectSpecToSelf，SetNumericAttributeBase，ASC->ApplyModToAttribute的调用 会触发,AttributeSet->SetHealth()不会触发
	 */
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAureAbilitySystemComponent::ClientEffectApplied);
} 

/**
 * 服务器在角色生成时调用，把初始技能（GA）喂给 ASC
 * 每个技能绑定一个输入标签（StartupInputTag），后续按标签触发
 */
void UAureAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbility)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartUpAbility)
	{
		// 创建技能规格（Spec），Level 1，默认不给予输入
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		// 转成咱们自己的基类，取出编辑器里填的 StartupInputTag
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			// 把输入标签塞进 DynamicAbilityTags，后续输入系统按标签检索
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);

			/**
			 * GiveAbility
			 * 1. 生成全局唯一 Handle （通常一种技能一个）
			 * 2. 实例化 GA 对象（CDO → 实例）
			 * 3. 塞进 ActiveAbilitySpecs 数组
			 * 4. 标记网络脏，客户端会收到复制包并本地生成影子 Spec
			 */
			GiveAbility(AbilitySpec);// 真正交给 ASC 管理
		}
		
		// GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

/**
 * 输入系统回调：某输入标签被“按住”时调用
 * 遍历所有已拥有的技能，找到 DynamicAbilityTags 匹配的标签 → 尝试激活
 */
void UAureAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTags)
{
	if (!InputTags.IsValid()) return;

	// GetActivatableAbilities() 返回当前可被激活的技能 Spec
	for (FGameplayAbilitySpec AbilitySpec : GetActivatableAbilities())
	{
		//查找 AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag); 之前存进的标签 
		if (AbilitySpec.DynamicAbilityTags.HasTag(InputTags))
		{
			// 标记“输入按下”状态，用于冷却、消耗等预判
			AbilitySpecInputPressed(AbilitySpec);
			// 如果技能没激活，则尝试激活（服务器仲裁）
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);//释放技能
			}
		}
	}
	
}


/**
 * 输入系统回调：某输入标签“松开”时调用
 * 同样按标签找到对应技能，标记“输入释放”
 */
void UAureAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTags)
{
	if (!InputTags.IsValid()) return;

	for (FGameplayAbilitySpec AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTag(InputTags))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

/**
 * 只要 权威端 通过 任何 ApplyGameplayEffect 接口* 把 带 AssetTag 的 GE 施加到 目标 ASC，就会 在网络包到达客户端时 由 引擎复制系统 自动触发 ClientEffectApplied_Implementation
 * 如，
 * ASC->ApplyGameplayEffectSpecToSelf，
 * ASC->ApplyGameplayEffectSpecToTarget ，
 * UAbilitySystemBlueprintLibrary::ApplyGameplayEffectSpecToActor
 * UAbilitySystemBlueprintLibrary::ApplyGameplayEffectToActor
 * UAbilitySystemComponent::ExecuteGameplayCue等等
 * 会触发
 * 当 GameplayEffect 成功应用到自身后，服务器调用 ClientEffectApplied（RPC），
 * 客户端收到后自动执行本函数（ClientReliable）
 * 用途：把 Effect 里携带的 AssetTag（设计师在蓝图 GE 里填的）广播给 UI 层做提示
 * 
 * UAbilitySystemComponent ：被施加 GE 的那个 ASC 实例本身（客户端的ASC，即自己）
 */
void UAureAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec,
                                                FActiveGameplayEffectHandle ActiveEffectHandle)
{
	// GEngine->AddOnScreenDebugMessage(1,8.f,FColor::Blue,FString("EffectApplied"));
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);

	// for ( const FGameplayTag& Tag:TagContainer)
	// {
	// 	const FString Msg = FString::Printf(TEXT("tag:%s"), *Tag.ToString());
	// 	GEngine->AddOnScreenDebugMessage(-1,8.f,FColor::Blue,Msg);
	// }
}
