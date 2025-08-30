#include "TPItemActor_Pickable.h"

#include "LyraEx/Component/ItemInteractionComponent/Core/TPItemInteractionComponent.h"
#include "LyraEx/ItemInteraction/ItemInteractionInterface.h"
#include "LyraEx/ItemInteraction/ItemDefinition/ItemPickableDefinition.h"

ATPItemActor_Pickable::ATPItemActor_Pickable()
{
	TPItemType=ETPItemType::IT_Pickable;
	PickableType=ETPItemPickableType::PT_Normal;
}

UItemPickableDefinition* ATPItemActor_Pickable::GetItemDefinition() const
{
	return ItemDefinition.GetDefaultObject();
}

void ATPItemActor_Pickable::OnStartTrigger_Implementation(UTPItemInteractionComponent* ItemInteractionComponent,
	bool bForceInHand)
{
	Super::OnStartTrigger_Implementation(ItemInteractionComponent, bForceInHand);
	
	if (ItemInteractionComponent->GetOwner())
	{
		if (IItemInteractionInterface* ItemInterface=Cast<IItemInteractionInterface>(ItemInteractionComponent->GetOwner()))
		{
			if (USkeletalMeshComponent* CharacterMesh=ItemInterface->Execute_GetCharacterMesh(ItemInteractionComponent->GetOwner()))
			{
				if (ItemDefinition)
				{
					if (ItemDefinition.GetDefaultObject()->StartTriggerAnimMontage)
					{
						ItemInteractionComponent->PlayMontageMulticast(ItemDefinition.GetDefaultObject()->StartTriggerAnimMontage);
					}
					AttachToComponent(CharacterMesh,
						FAttachmentTransformRules::SnapToTargetNotIncludingScale,
						ItemDefinition.GetDefaultObject()->ItemSlotName);
					SetActorRelativeTransform(ItemDefinition.GetDefaultObject()->ItemRelativeTransform);
				}
			}
		}
	}
}

void ATPItemActor_Pickable::OnEndTrigger_Implementation(UTPItemInteractionComponent* ItemInteractionComponent,
	bool bIsPutPack)
{
	Super::OnEndTrigger_Implementation(ItemInteractionComponent, bIsPutPack);

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	//todo:丢弃动画
	if (!bIsPutPack)
	{
		if (ItemDefinition&&ItemDefinition.GetDefaultObject()->EndTriggerAnimMontage)
		{
			ItemInteractionComponent->PlayMontageMulticast(ItemDefinition.GetDefaultObject()->EndTriggerAnimMontage);
		}
	}
}

void ATPItemActor_Pickable::PickupItem_Implementation(UTPItemInteractionComponent* ItemInteractionComponent,
	UPrimitiveComponent* InItemMesh)
{
	if (InItemMesh)
	{
		InItemMesh->SetSimulatePhysics(false);
		InItemMesh->SetRelativeLocation(FVector::ZeroVector);
		InItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		SetReplicatingMovement(false);
	}
}

void ATPItemActor_Pickable::ThrowItem_Implementation(UTPItemInteractionComponent* ItemInteractionComponent,
	UPrimitiveComponent* InItemMesh, bool bIsPutBack)
{
	if (InItemMesh&&ItemDefinition)
	{
		SetReplicatingMovement(true);
		InItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		InItemMesh->SetSimulatePhysics(true);

		if (!bIsPutBack)
		{
			if (ItemInteractionComponent&&ItemInteractionComponent->GetOwner())
			{
				InItemMesh->AddImpulse(ItemInteractionComponent->GetOwner()->GetActorForwardVector()*ItemDefinition.GetDefaultObject()->ThrowItemSpeed,
					NAME_None,
					true);
			}
		}
	}
}
