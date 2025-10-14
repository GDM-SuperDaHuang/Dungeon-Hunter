// daHuang


#include "AuraAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "AuraGameplayTags.h"
UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);
	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);

	return *AuraAssetManager;
	// static UAuraAssetManager* instance;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	FAuraGameplayTags::InitializeNativeGameplayTags();

	//5.4之后似乎不用调用该函数论
	// UAbilitySystemGlobals::Get().InitGlobalData();
}
