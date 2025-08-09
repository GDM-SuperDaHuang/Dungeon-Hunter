// daHuang


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AureAttributeSet.h"
#include "Components/SphereComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void AAuraEffectActor::OnOverlap(UPrimitiveComponent* UOverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		UAbilitySystemComponent* AbilitySystemComponent = ASCInterface->GetAbilitySystemComponent();
		const UAureAttributeSet* AureAttributeSet = Cast<UAureAttributeSet>(AbilitySystemComponent->GetAttributeSet(UAureAttributeSet::StaticClass()));

		UAureAttributeSet* MutableAuraAttriButeSet = const_cast<UAureAttributeSet*>(AureAttributeSet);
		MutableAuraAttriButeSet->SetHealth(AureAttributeSet->GetHealth()+25.f);
		Destroy();
	}
	
}

void AAuraEffectActor::EndOverlap(UPrimitiveComponent* UOverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&AAuraEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this,&AAuraEffectActor::EndOverlap);
}



