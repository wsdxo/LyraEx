#include "TPItemInteractionComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UTPItemInteractionComponent::UTPItemInteractionComponent()
{
	SetIsReplicatedByDefault(true);
}

void UTPItemInteractionComponent::StartTriggerSelectingItem()
{
	if (!CanStartTriggerInteractingItem())
	{
		UE_LOG(LogTPItemInteractionComponent,Error,TEXT("TPItemInteractionComponent Can't strat trigger on local!"));
		return;
	}
	StartTriggerSelectingItemOnServer(SelectingItem.Get());
}

void UTPItemInteractionComponent::EndTriggerSelectingItem()
{
	if (!CanEndTriggerInteractingItem())
	{
		UE_LOG(LogTPItemInteractionComponent,Error,TEXT("TPItemInteractionComponent Can't strat trigger on local!"))
		return;
	}
	EndTriggerInteractingItemOnServer(false);
}

void UTPItemInteractionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTPItemInteractionComponent,InteractingItem);
}

void UTPItemInteractionComponent::OnRep_InteractingItem()
{
}

void UTPItemInteractionComponent::PlayMontageMulticast_Implementation(UAnimMontage* InMontage, float InPlayRate,
                                                                      FName StartSelectionName)
{
	if (GetOwner())
	{
		if (ACharacter* PlayerCharacter=Cast<ACharacter>(GetOwner()))
		{
			PlayerCharacter->PlayAnimMontage(InMontage,InPlayRate,StartSelectionName);
		}
	}
}

void UTPItemInteractionComponent::ServerTriggerItem(ATPItemActorBase* NewTriggerItem, bool bForceInHand)
{
	if (GetOwner()&&GetOwner()->HasAuthority())
	{
		OnSelectingItemTriggerStart(NewTriggerItem,bForceInHand);
	}
}

bool UTPItemInteractionComponent::CanStartInteraction_Implementation() const
{
	return true;
	//return false;
}

bool UTPItemInteractionComponent::CanEndInteraction_Implementation() const
{
	return true;
	//return false;
}

bool UTPItemInteractionComponent::CanStartTriggerInteractingItem_Implementation() const
{
	if (!SelectingItem.IsValid())
	{
		UE_LOG(LogTPItemInteractionComponent,Error,TEXT("No selecting item by this component"));
		return false;
	}
	return true;
	//return false;
}

bool UTPItemInteractionComponent::CanEndTriggerInteractingItem_Implementation() const
{
	if (InteractingItem==nullptr)
	{
		UE_LOG(LogTPItemInteractionComponent,Error,TEXT("No interacting item by this component"));
		return false;
	}
	return true;
	//return false;
}

void UTPItemInteractionComponent::StartTriggerSelectingItemOnServer_Implementation(ATPItemActorBase* InSelectingItem)
{
	OnSelectingItemTriggerStart(InSelectingItem,false);
}


void UTPItemInteractionComponent::EndTriggerInteractingItemOnServer_Implementation(bool bIsPutPack)
{
	OnSelectingItemTriggerEnd(InteractingItem.Get(),bIsPutPack);
}

void UTPItemInteractionComponent::OnSelectingItemTriggerStart_Implementation(ATPItemActorBase* InSelectingItem,
	bool bForceInHand)
{
}

void UTPItemInteractionComponent::OnSelectingItemTriggerEnd_Implementation(ATPItemActorBase* InSelectingItem,
	bool bForceInHand)
{
}

UAbilitySystemComponent* UTPItemInteractionComponent::GetAbilitySystemComponentByOwner() const
{
	if (GetOwner())
	{
		if (IAbilitySystemInterface* ASI=Cast<IAbilitySystemInterface>(GetOwner()))
		{
			if (UAbilitySystemComponent* ASC=ASI->GetAbilitySystemComponent())
			{
				return ASC;
			}
		}
	}
	return nullptr;
}
