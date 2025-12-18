#include "AuraAbilityTypes.h"

bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	// uint32 b 消息头
	uint32 RepBits = 0;
	if (Ar.IsSaving())// 如果是写包行为，内存 → Archive → 网络
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid())
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		if (bIsBlockedHit)
		{
			RepBits |= 1 << 7;
		}
		if (bIsCriticalHit)
		{
			RepBits |= 1 << 8;
		}
		if (bIsSuccessfulDebuff)
		{
			RepBits |= 1 << 9;
		}
		if (DebuffDamage > 0.f)
		{
			RepBits |= 1 << 10;
		}
		if (DebuffDuration > 0.f)
		{
			RepBits |= 1 << 11;
		}
		if (DebuffFrequency > 0.f)
		{
			RepBits |= 1 << 12;
		}
		if (DamageType.IsValid())
		{
			RepBits |= 1 << 13;
		}
		if (!DeathImpulse.IsZero())
		{
			RepBits |= 1 << 14;
		}

		if (!KnockbackForce.IsZero())
		{
			RepBits |= 1 << 15;
		}
	}

	// 从 Ar 读取15个bit到RepBits，
	// 15，有问题？只序列化 bit 0 ~ bit 14，读不到 KnockbackForce？
	Ar.SerializeBits(&RepBits, 16);

	if (RepBits & (1 << 0)) //对应的位置如果有效，则写入值
	{
		Ar << Instigator;// 如果是Saving，则是把 Instigator 写入Ar；如果是Loading，则是把从 Ar 读出数据，赋给 Instigator
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())//如果是读包
		{
			if (!HitResult.IsValid())
			{
				//FHitResult是TSharedPtr类型的指针，不是 UObject子类，没有 NetGUID，需要解引用,赋默认值，
				// 防止HitResult为null，奔溃
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		/**
		 * 如果一个类型没有实现 << 操作符重载，而只有 NetSerialize，就必须调用 NetSerialize。
		 * 反之，如果类型实现了 <<，用 NetSerialize 可能会编译错误或无效。
		*/
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}
	if (RepBits & (1 << 7))
	{
		Ar << bIsBlockedHit;
	}

	if (RepBits & (1 << 8))
	{
		Ar << bIsCriticalHit;
	}

	if (RepBits & (1 << 9))
	{
		Ar << bIsSuccessfulDebuff;
	}

	if (RepBits & (1 << 10))
	{
		Ar << DebuffDamage;
	}
	if (RepBits & (1 << 11))
	{
		Ar << DebuffDuration;
	}
	if (RepBits & (1 << 12))
	{
		Ar << DebuffFrequency;
	}

	if (RepBits & (1 << 13))
	{
		if (Ar.IsLoading())
		{
			if (!DamageType.IsValid())
			{
				//解引用，写默认值
				DamageType = TSharedPtr<FGameplayTag>(new FGameplayTag());
			}
		}
		// IsLoading时，将Ar内容写入DamageType，
		// IsSaving 时，将DamageType被人写入Ar
		DamageType->NetSerialize(Ar, Map, bOutSuccess);
	}

	if (RepBits & (1 << 14))
	{
		DeathImpulse.NetSerialize(Ar, Map, bOutSuccess);
	}

	if (RepBits & (1 << 15))
	{
		KnockbackForce.NetSerialize(Ar, Map, bOutSuccess);
	}
	

	if (Ar.IsLoading())
	{
		// 更新施加者的ASC
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}

	bOutSuccess = true;
	return true;
}
