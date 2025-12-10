// daHuang


#include "AbilitySystem/Data/AbilityInfo.h"

#include "DungeonHunters2/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (FAuraAbilityInfo Info : AbilityInformation)
	{
		if (Info.AbilityTag == AttributeTag)
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Ability info not found,AbilityTag :【%s】 on AuraAbilityInfo :【%s】 "),
		       *AttributeTag.ToString(), *GetNameSafe(this));
	}
	return FAuraAbilityInfo();
}
