// daHuang


#include "AbilitySystem/AureAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "DungeonHunters2/AuraLogChannels.h"
#include "Interaction/PlayerInterface.h"

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
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Equipped);

			/**
			 * GiveAbility
			 * 1. 生成全局唯一 Handle （通常一种技能一个）
			 * 2. 实例化 GA 对象（CDO → 实例），里面初始化了AvatarActor ，OwnerActor，PlayerController 
			 * 3. 塞进 ActiveAbilitySpecs 数组
			 * 4. 标记网络脏，客户端会收到复制包并本地生成影子 Spec
			 */
			GiveAbility(AbilitySpec); // 真正交给 ASC 管理
		}

		// GiveAbilityAndActivateOnce(AbilitySpec);
	}
	bStartUpAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UAureAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbility)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartUpAbility)
	{
		// 创建技能规格（Spec），Level 1，默认不给予输入
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

/**
 * 输入系统回调：某输入标签被“按住”时调用
 * 遍历所有已拥有的技能，找到 DynamicAbilityTags 匹配的标签 → 尝试激活
 */
void UAureAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTags)
{
	if (!InputTags.IsValid()) return;

	// GetActivatableAbilities() 返回当前可被激活的技能 Spec,GiveAbility(AbilitySpec);
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
				TryActivateAbility(AbilitySpec.Handle); //释放技能
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

void UAureAbilitySystemComponent::FForEachAbility(const ::FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura, Error,
			       TEXT("UAureAbilitySystemComponent::FForEachAbility execution failed in %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag UAureAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		FGameplayTagContainer GameplayTags = AbilitySpec.Ability.Get()->AbilityTags;
		for (FGameplayTag Tag : GameplayTags)
		{
			// ???
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}

	return FGameplayTag();
}

FGameplayTag UAureAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}

	return FGameplayTag();
}

// 获取Abilities.Status前缀的的标签
FGameplayTag UAureAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAureAbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetStatusFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayTag UAureAbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*Spec);
	}
	return FGameplayTag();
}


FGameplayAbilitySpec* UAureAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

void UAureAbilitySystemComponent::UpgradeAttributes(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttributes(AttributeTag);
		}
	}
}

void UAureAbilitySystemComponent::ServerUpgradeAttributes_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
	}
}

void UAureAbilitySystemComponent::UpdateAbilityStatuses(int32 Level)
{
	// 从 DataAsset 获取配技能配置AbilityInfo
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FAuraAbilityInfo Info : AbilityInfo->AbilityInformation)
	{
		// 如果该技能没有能力标签（用于标识该技能），则跳过
		if (!Info.AbilityTag.IsValid())continue;

		 // 若玩家当前等级不足以解锁该技能（未达到 LevelRequirement），则跳过
		if (Level < Info.LevelRequirement) continue;

		// 检查 ASC 中是否已经有这个 Ability（通过技能标签查找 AbilitySpec）
        // 如果返回 nullptr，说明玩家还没有获得/解锁这个技能
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			// 创建一个 GameplayAbilitySpec，等级为 1
            // 这里不使用 Gameplay Ability 的内部等级，而是把外部等级作为概率使用
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);

			// 添加一个动态 Tag（Abilities.Status.Equipped）
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Equipped);

			// 给 AbilitySystemComponent 添加这个 AbilitySpec
            // 这里必须在服务器端调用，，
			// todo这里有问题!!!!
			// 先创建，等下再加标签
							// FGameplayAbilitySpecHandle Handle = GiveAbility(AbilitySpec);
							// // 取得 ASC 中真正的 AbilitySpec
							// FGameplayAbilitySpec* GrantedSpec = FindAbilitySpecFromHandle(Handle);
							// if (GrantedSpec)
							// {
							// 	// 添加动态标签（必须在真正的 Spec 上加）
							// 	GrantedSpec->DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Equipped);

							// 	// 标记脏 → 将变更同步给客户端
							// 	MarkAbilitySpecDirty(*GrantedSpec);
							// }

			GiveAbility(AbilitySpec);

			// **关键：标记 AbilitySpec 已被修改**
            // 通知 ASC 需要把这一 Spec 发送到客户端（同步）
			//是否修改了 AbilitySpec
			// 是 → MarkAbilitySpecDirty
			// 否 → 不要调用
			MarkAbilitySpecDirty(AbilitySpec);

			// RPC 调用：通知客户端 UI 更新技能图标状态、等级显示等
            // （不会修改能力，只是 UI）
			ClientUpdateAbilityStatus(Info.AbilityTag, FAuraGameplayTags::Get().Abilities_Status_Equipped, 1);
		}
	}
}

void UAureAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
		}

		const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

		FGameplayTag Status = GetStatusFromSpec(*AbilitySpec);
		if (Status.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Abilities_Status_Eligible);
			AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Unlocked);
			Status = GameplayTags.Abilities_Status_Unlocked;
		}
		else if (Status.MatchesTagExact(GameplayTags.Abilities_Status_Equipped)
			|| Status.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
		{
			AbilitySpec->Level += 1;
		}
		ClientUpdateAbilityStatus(AbilityTag, Status, AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UAureAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag,
                                                                    const FGameplayTag& Slot)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		const FGameplayTag PreSlot = GetInputTagFromSpec(*AbilitySpec);
		const FGameplayTag Status = GetStatusFromSpec(*AbilitySpec);

		const bool bStatusValid = Status == GameplayTags.Abilities_Status_Eligible || Status ==
			GameplayTags.Abilities_Status_Unlocked;
		if (bStatusValid)
		{
			ClearAbilitiesOfSlot(Slot);
			ClearSlot(AbilitySpec);
			AbilitySpec->DynamicAbilityTags.AddTag(Slot);
			if (Status.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
			{
				AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Abilities_Status_Unlocked);
				AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Equipped);
			}
			MarkAbilitySpecDirty(*AbilitySpec);
		}
		ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, Slot, PreSlot);
	}
}

void UAureAbilitySystemComponent::ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	AbilityEquippedDelegate.Broadcast(AbilityTag, Status, Slot, PreviousSlot);
}

bool UAureAbilitySystemComponent::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescriptions,
                                                              FString& OutNextLevelDescriptions)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (UAuraGameplayAbility* AureAbility = Cast<UAuraGameplayAbility>(AbilitySpec->Ability))
		{
			OutDescriptions = AureAbility->GetDescription(AbilitySpec->Level);
			OutNextLevelDescriptions = AureAbility->GetNextLevelDescription(AbilitySpec->Level + 1);
			return true;
		}
	}
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());

	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_None))
	{
		OutDescriptions = FString();
	}
	else
	{
		OutDescriptions = UAuraGameplayAbility::GetLockedDescription(
			AbilityInfo->FindAbilityInfoForTag(AbilityTag).LevelRequirement);
	}

	OutNextLevelDescriptions = FString();
	return false;
}

void UAureAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* Spec)
{
	FGameplayTag Slot = GetInputTagFromSpec(*Spec);
	Spec->DynamicAbilityTags.RemoveTag(Slot);
	MarkAbilitySpecDirty(*Spec);
}

void UAureAbilitySystemComponent::ClearAbilitiesOfSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec Spec : GetActivatableAbilities())
	{
		if (AbilitiesHasSlot(&Spec, Slot))
		{
			ClearSlot(&Spec);
		}
	}
}

bool UAureAbilitySystemComponent::AbilitiesHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot)
{
	FGameplayTagContainer GameplayTags = Spec->DynamicAbilityTags;
	for (FGameplayTag Tag : Spec->DynamicAbilityTags)
	{
		if (Tag.MatchesTagExact(Slot))
		{
			return true;
		}
	}
	return false;
}



void UAureAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	//广播一次
	if (!bStartUpAbilitiesGiven)
	{
		bStartUpAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}

void UAureAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
                                                                           const FGameplayTag& StatusTag,
                                                                           int32 AbilityLevel)
{
	AbilityStatusChangedDelegate.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

/**
 * 因为 UFUNCTION(Client, Reliable)的缘故，只会让其中一个客户端发生回调。
 * 只要 权威端 通过 任何 ApplyGameplayEffect 接口* 把 带 AssetTag 的 GE 施加到 目标 ASC，就会 在网络包到达客户端时 由 引擎复制系统 自动触发 ClientEffectApplied_Implementation
 * 如，
 * ASC->ApplyGameplayEffectSpecToSelf，
 * ASC->ApplyGameplayEffectSpecToTarget ，
 * UAbilitySystemBlueprintLibrary::ApplyGameplayEffectSpecToActor
 * UAbilitySystemBlueprintLibrary::ApplyGameplayEffectToActor
 * UAbilitySystemComponent::ExecuteGameplayCue等等
 * 会触发
 * 当 GameplayEffect 成功应用到自身后，服务器调用 ClientEffectApplied（RPC），
 * 被影响的特定客户端收到后自动执行本函数（ClientReliable）
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
