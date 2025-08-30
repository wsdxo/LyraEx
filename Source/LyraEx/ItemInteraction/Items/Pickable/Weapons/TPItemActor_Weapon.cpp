#include "TPItemActor_Weapon.h"

#include "LyraEx/Component/ItemInteractionComponent/Core/TPItemInteractionComponent.h"
#include "LyraEx/Component/WeaponManagerComponent/WeaponManagerComponent.h"
#include "LyraEx/ItemInteraction/ItemInteractionInterface.h"
#include "LyraEx/ItemInteraction/ItemDefinition/ItemPickableDefinition.h"
#include "LyraEx/ItemInteraction/ItemInstance/WeaponInstance.h"
#include "Net/UnrealNetwork.h"

UE_DISABLE_OPTIMIZATION

ATPItemActor_Weapon::ATPItemActor_Weapon()
{
	PickableType=ETPItemPickableType::PT_Weapon;

	bReplicateUsingRegisteredSubObjectList=true;

	WeaponMesh=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionProfileName(FName(TEXT("TPItemPickable")));
	WeaponMesh->BodyInstance.bSimulatePhysics=true;
	WeaponMesh->SetIsReplicated(true);
}

void ATPItemActor_Weapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATPItemActor_Weapon,WeaponInstance);
}

void ATPItemActor_Weapon::EquipToSlot()
{
	if(InteractingComponent.IsValid()&&InteractingComponent->GetOwner())
	{
		if(IItemInteractionInterface* ItemInterface=Cast<IItemInteractionInterface>(InteractingComponent->GetOwner()))
		{
			if(USkeletalMeshComponent* CharacterMesh=ItemInterface->Execute_GetCharacterMesh(InteractingComponent->GetOwner()))
			{
				PickupItem(InteractingComponent.Get(),WeaponMesh);

				//todo:绑定至玩家武器槽位并调整transfrom
				//AttachToComponent();
				//SetActorRelativeTransform();
			}
		}
	}
}

void ATPItemActor_Weapon::EquipToHand()
{
	if(InteractingComponent.IsValid()&&InteractingComponent->GetOwner())
	{
		if(IItemInteractionInterface* ItemInterface=Cast<IItemInteractionInterface>(InteractingComponent->GetOwner()))
		{
			if(USkeletalMeshComponent* CharacterMesh=ItemInterface->Execute_GetCharacterMesh(InteractingComponent->GetOwner()))
			{
				if(ItemDefinition)
				{
					PickupItem(InteractingComponent.Get(),WeaponMesh);

					AttachToComponent(CharacterMesh,FAttachmentTransformRules::SnapToTargetNotIncludingScale,ItemDefinition.GetDefaultObject()->ItemSlotName);
					SetActorRelativeTransform(ItemDefinition.GetDefaultObject()->ItemRelativeTransform);
				}
			}
		}
	}
}

void ATPItemActor_Weapon::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority()&&WeaponInstance==nullptr)
	{
		WeaponInstance=NewObject<UWeaponInstance>(this,WeaponInstanceClass);
		if(WeaponInstance)
		{
			//WeaponInstance->InitInstance();
			AddReplicatedSubObject(WeaponInstance);
		}
	}
}

void ATPItemActor_Weapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(HasAuthority())
	{
		RemoveReplicatedSubObject(WeaponInstance);
	}
	Super::EndPlay(EndPlayReason);
}

void ATPItemActor_Weapon::OnStartTrigger_Implementation(UTPItemInteractionComponent* ItemInteractionComponent,
	bool bForceInHand)
{
	if(WeaponInstance==nullptr)
	{
		return;
	}
	if(UWeaponManagerComponent* WeaponManager=GetWeaponManager(ItemInteractionComponent))
	{
		// if (WeaponManager->AddWeapon(this))	
		// {
		 		if(bForceInHand)
		 		{
		 			ATPItemActorBase::OnStartTrigger_Implementation(ItemInteractionComponent,bForceInHand);
		 			//WeaponManager->EquipWeapon((int32)WeaponInstance->GetWeaponSlot());
		 			EquipToHand();
		 		}
				else
				{
					ATPItemActorBase::OnStartTrigger_Implementation(ItemInteractionComponent,bForceInHand);
					EquipToSlot();
				}
		// }
	}
}

void ATPItemActor_Weapon::OnEndTrigger_Implementation(UTPItemInteractionComponent* ItemInteractionComponent,
	bool bIsPutPack)
{
	if(WeaponInstance)
	{
		if (UWeaponManagerComponent*WeaponManager=GetWeaponManager(ItemInteractionComponent))
		{
			if(bIsPutPack)
			{
				// if(WeaponManager->GetWeaponSlot(WeaponManager->GetEquipSlot())==this&&WeaponManager->UnequipWeapon())
				// {
				// 	EquipToSlot();
				// }
			}
			else
			{
				// if(WeaponManager->RemoveWeapon((int32)WeaponInstance->GetWeaponSlot()))
				// {
				// 	ThrowItem(ItemInteractionComponent,WeaponMesh,false);
				// 	Super::OnEndTrigger_Implementation(ItemInteractionComponent,bIsPutPack);
				// }
			}
		}
	}
}

UWeaponManagerComponent* ATPItemActor_Weapon::GetWeaponManager(UTPItemInteractionComponent* InInteractionComponent)
{
	if(InInteractionComponent)
	{
		if(InInteractionComponent->GetOwner())
		{
			return InInteractionComponent->GetOwner()->GetComponentByClass<UWeaponManagerComponent>();
		}
	}
	return nullptr;
}
UE_ENABLE_OPTIMIZATION