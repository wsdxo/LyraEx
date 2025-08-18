#include "TPItemActorBase.h"
#include "LyraEx/Component/ItemInteractionComponent/Core/TPItemInteractionComponent.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogTPItemActor)
ATPItemActorBase::ATPItemActorBase()
{
	PrimaryActorTick.bCanEverTick=true;
	bReplicates=true;
	bNetLoadOnClient=false;

	TPItemType=ETPItemType::IT_InValid;
}

bool ATPItemActorBase::StartTrigger(UTPItemInteractionComponent* ItemInteractionComponent, bool bForceInHand)
{
	if (!CanStartTrigger(ItemInteractionComponent,bForceInHand))
	{
		UE_LOG(LogTPItemActor,Error,TEXT("TPItemActor %s is already interacting"),*GetName());
		return false;
	}
	OnStartTrigger(ItemInteractionComponent,bForceInHand);
	return true;
}

bool ATPItemActorBase::EndTrigger(UTPItemInteractionComponent* ItemInteractionComponent, bool bIsPutPack)
{
	if (!CanEndTrigger(ItemInteractionComponent,bIsPutPack))
	{
		UE_LOG(LogTPItemActor,Error,TEXT("TPItemActor %s can't EndTrigger"),*GetName());
		return false;
	}
	OnEndTrigger(ItemInteractionComponent,bIsPutPack);
	return true;
}

void ATPItemActorBase::OnStartTrigger_Implementation(UTPItemInteractionComponent* ItemInteractionComponent,
	bool bForceInHand)
{
	InteractingComponent = ItemInteractionComponent;
}

void ATPItemActorBase::OnEndTrigger_Implementation(UTPItemInteractionComponent* ItemInteractionComponent,
	bool bIsPutPack)
{
	InteractingComponent.Reset();
}

void ATPItemActorBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATPItemActorBase,InteractingComponent);
}

bool ATPItemActorBase::CanStartTrigger_Implementation(UTPItemInteractionComponent* ItemInteractionComponent,
                                                      bool bForceInHand)
{
	return ItemInteractionComponent && InteractingComponent==nullptr;
}

bool ATPItemActorBase::CanEndTrigger_Implementation(UTPItemInteractionComponent* ItemInteractionComponent,
	bool bIsPutPack)
{
	return InteractingComponent.IsValid() && InteractingComponent == ItemInteractionComponent;
}
