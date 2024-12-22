#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "JoinSessionCallbackProxy.h"
#include "FindSessionsCallbackProxy.h"
#include "OnlineSubsystem.h"
#include "GameFramework/Character.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Weather/generate.h"


/** Please add a class description */
UCLASS(Blueprintable, BlueprintType)
class UAC_Inventory : public UActorComponent
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Add")
	void Try Add Item to inventory automaticly(FStruct_Inventory_Slot_Item Inventory Item to be added, UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Target Array, bool Override Stack size, int32 Override max stack size, bool Override dynamic slot creation to true, bool Use this function as check only, bool& All items added, int32& Amount not added, int32& Empty slots filled, int32 local amount that could not be added into existing stacks, FStruct_Inventory_Slot_Item Local inventory item to be added, int32 local already added into new stack in other loop, int32 Number of loops required, int32 local amount that could not be added due to weight);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact Trace(TArray<TEnumAsByte<EObjectTypeQuery> > Trace Object);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Utility")
	void Find Empty Slot(UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Array, int32& Slot Index, bool& Success, bool local Found Empty slot, int32 local slot index of empty slot);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Utility")
	void Find Existing Slot indexes of specified inventory item(FStruct_Inventory_Slot_Item Inventory Item, UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Target Array, TArray<int32>& Slots found at indexes, bool& Succes, TArray<int32> Found Indexes);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Add")
	void Try Process item to existing stack(int32 Slot Index, FStruct_Inventory_Slot_Item Item to add, int32 correction of amount previously added, UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Array, bool Override Stack size, int32 Override max stack size, bool Use this function as check only, int32& amount added, bool& Success, int32 local amount added, int32 local slot index, FStruct_Inventory_Slot_Item local item to add, int32 local current amount in slot, int32 local max stack size, int32 local amount to be added, int32 Local Room in stack, bool local override Stack size, int32 local override max stack size);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Utility")
	void Get slot info by index(int32 slot index, UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Array, bool Override stack size, int32 New stack size, FStruct_Inventory_Slot_Item& Inventory Item data(slot), FStruct_Item& Item data, bool& Stackable, int32& Max stack size, int32& Current Amount, int32& Room in stack, bool& Stack is full);

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Inventory|Utility")
	void check if inventory item is equal(const FStruct_Inventory_Slot_Item& Struct_Inventory_Item 1, const FStruct_Inventory_Slot_Item& Struct_Inventory_Item 2, bool& True);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Add")
	void Try add items to existing stacks automaticly(FStruct_Inventory_Slot_Item Inventory Item, UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Target Array, bool Override Stack size, int32 Override max stack size, bool Use this function as check only, bool& No existing stacks found, bool& All Items added, int32& Amount that could not be added into existing stacks, FStruct_Inventory_Slot_Item local inventory item, int32 Local Amount previously added);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Add")
	void Try add items to empty and new slots automaticly(const FStruct_Inventory_Slot_Item& inventory item to be added, int32 Amount to add, UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Target Array, bool Override Stack Size, int32 New Stack Size, bool Override dynamic slot creation, bool Use this function as check only, bool& All items added, int32& Amount not added, int32& Empty slots filled, int32 local amount to add, FStruct_Inventory_Slot_Item Local inventory item to be added, int32 local amount added into empty stacks);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Utility")
	void Create Empty slot(const TArray<FStruct_Inventory_Slot_Item>& TargetArray, int32& New slot index);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Utility")
	void Get Inventory Item info(const FStruct_Inventory_Slot_Item& inventory item, bool Override Stack Size, int32 New Stack Size, FStruct_Item& Item, bool& Stackable, int32& Max stack size, int32& Amount in slot, int32& Room in slot);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Add")
	void Try Add items to stack(const FStruct_Inventory_Slot_Item& Struct_Inventory_Item, int32 Slot Index, int32 Amount to be added, int32 Amount in slot, int32 Room in slot, const TArray<FStruct_Inventory_Slot_Item>& TargetArray, bool Use this function as check only, int32& Amount added, bool& All items added, int32& Amount left to be added);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Utility")
	void How many stacks should be created(const FStruct_Inventory_Slot_Item& inventory item, int32 Amount to add, bool Override Stack Size, int32 New Stack Size, int32& amount of new stacks required);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Remove")
	void Remove Item from inventory automaticly(FStruct_Inventory_Slot_Item Inventory Item to remove, int32 Amount to remove, UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Target Array, bool& Success, int32& amount not removed, int32 local Amount to remove, int32 local current amount in slot, int32 local remainder to be removed, int32 local found slot index, FDataTableRowHandle local existing slot found, int32 local amount removed);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Remove")
	void Remove item at slot index(int32 Slot Index, int32 Amount, UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Target Array, bool To be removed item is equiped, bool& Success, bool& Empty slot, FStruct_Inventory_Slot_Item Local inventory slot item, int32 Local amount to remove, int32 local slot index);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Default|Initialization")
	void Initialize starting inventory(bool& Initial Inventory loaded);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Move")
	void Move Item to slot index(FStruct_Inventory_Slot_Item Inventory Item, UAC_Inventory_C* Source invenvtory component, UAC_Inventory_C* Destination Inventory, int32 Source Slot Index, int32 Destination Slot Index, UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Source Array, UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Destination Array, bool Automatic placement, bool Is from equipment, bool& All items added, FStruct_Inventory_Slot_Item local inventory item to be moved, UAC_Inventory_C* local destination inventory comp, int32 local source slot index, int32 local destination slot index, FStruct_Inventory_Slot_Item local source item, FStruct_Inventory_Slot_Item local destination item, bool local is an equipment move, UAC_Inventory_C* local source inventory component, TArray<FStruct_Inventory_Slot_Item> Local Source Array, TArray<FStruct_Inventory_Slot_Item> Local Destination Array, bool Local Is Automatic Placement, TEnumAsByte<Enum_Equipment_Slots> Local to which equipment slot should this item to be added, FStruct_Inventory_Slot_Item Local Destination slot info);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Crafting|Utility")
	void How many max items are craftable from inventory(FStruct_Inventory_Slot_Item Which Item to be made, UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Target Array, int32& Numer of items craftable, bool& No items can be made, int32& Number of items fit in crafting queue, int32 How many items found, TArray<FStruct_Inventory_Slot_Item> Local Recipe to be made, int32 Lowest number of possible items to be crafted);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Utility")
	void How many of item in inventory(FStruct_Inventory_Slot_Item Inventory Item, UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Array, int32& amount of items found, FStruct_Inventory_Slot_Item& of item, int32 local items found);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Default|Initialization")
	void Initialize Crafting Queue();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Crafting|Queue")
	void Add item to crafting Queue(const FStruct_Inventory_Slot_Item& Item);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Crafting|Queue")
	void Check Amount of filled slots in queue(int32& Amount in crafting queue, bool& All queue slots used, int32 Local Amount in crafting queue);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Move")
	void Try Move Item automaticly(UAC_Inventory_C* Destination Inventory, FStruct_Inventory_Slot_Item Inventory Item to bemoved, UAC_Inventory_C* source inventory, bool& Success);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Crafting|Remove")
	void Remove Item or slot from Queue on slot index(const FStruct_Inventory_Slot_Item & Struct_InventoryS_Item, bool Remove all from stack, int32 Slot Index);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Crafting|Remove")
	void Remove all items from queue();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Crafting|Queue")
	void Reset Crafting Queue();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Utility")
	void Can all items be added(UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Items to add, bool Use Multiplier, int32 Amount Multiplier, bool& All items can be added, bool Local All items can be added, int32 local Total Empty slots filled);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Move")
	void Try Transfer All to Destination Inventory(UAC_Inventory_C* Destination Inventory);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory Actions")
	void Transfer Item By TransferType(TEnumAsByte<Enum_Transfer_Types> Selection, FStruct_Inventory_Slot_Item Inventory Item, int32 Custom amount, UAC_Inventory_C* Destination Inventory, int32 Source Slot Index, UAC_Inventory_C* Source, bool& Success);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory Actions")
	void Split Inventory Stack by SplitType(TEnumAsByte<Enum_split_types> Split Type, const FStruct_Inventory_Slot_Item& inventory item to be splitted, int32 Custom Amount, int32 Source Slot Index);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Crafting|Crafting Actions")
	void Craft Item By Action Type(FStruct_Inventory_Slot_Item Which Item to be made, int32 Custom amount to be made, TEnumAsByte<Enum_CraftingAction_Types> Crafting Action Type);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Drop")
	void Drop Item by slot index(int32 Slot Index, int32 Amount, FStruct_Inventory_Slot_Item Inventory Item, UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Target Array, bool Is from Hotbar);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Auto Remove")
	void Remove all empty slots from start index to end of array(int32 Index to start removing, const TArray<FStruct_Inventory_Slot_Item>& TargetArray, bool& Completed);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Fuel")
	void Excecute OnOff Logic(bool Turn On, bool Switch Sate, bool& Is On);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Crafting|Queue")
	void Process Crafting queue slot index 0 and initialize timer();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Fuel")
	void Check to start Fuel Timer(bool& Found and removed Combustible in current inventory, bool Local Found Combustible in current inventory);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Durability|Decay")
	void Find item indexes to decay(TArray<int32>& Found item indexes to decay, TArray<int32> local found item indexes to decay, TArray<FStruct_Inventory_Slot_Item> Items to decay);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Durability|Decay")
	void Process Durability and decay logic by slot index(int32 Slot Index, UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Target array, bool& zero durability, FStruct_Inventory_Slot_Item Local inventory slot item, int32 Local slot index);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Durability|Decay")
	void Decay Item by slot index(int32 Slot Index, UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Target Array, bool& Durability is zero, bool& No Items left to decay, FStruct_Inventory_Slot_Item Local inventory slot item, int32 Local slot index);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Utility")
	void Set Inventory Slot to empty by index(int32 Index, const TArray<FStruct_Inventory_Slot_Item>& TargetArray);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Durability|Repair")
	void Try Repair Item by slot index(int32 Slot Index, UAC_Inventory_C* source inventory, UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Target Array, bool Is from hotbar, FStruct_Inventory_Slot_Item Local Inventory item reference, int32 local slot index);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Restocking")
	void Initialize_Stocking();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Vendor|Utility")
	void Can all items be bought ? (const FStruct_Inventory_Slot_Item & Item to be bought, UAC_Inventory_C * Seller Inventory Component, UAC_Inventory_C * Buyer Inventory Component, int32 Amount of items to buy, int32 & Buyer has resources for this many items, bool& Buyer has sufficient resources to buy complete amount, bool& Seller has sufficient amount in inventory, bool& All Items can be bought, int32& Amount that can be bought);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Utility")
	void Whole array of items can be found in inventory ? (UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>&Array, bool& Sufficient items found, bool Sufficient items);

	/** Should be called at Inventory Component where Is Vendor is true */
	UFUNCTION(BlueprintCallable, Category = "Vendor|Utility")
	void Calculate sell costs of one item(const FStruct_Inventory_Slot_Item& Item to be bought, UAC_Inventory_C* Seller Inventory Component, UAC_Inventory_C* Buyer Inventory Component, double& Cost, FStruct_Inventory_Slot_Item& Of Item, int32& Rounded cost);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Vendor|Trade Item")
	void Try Buy Item From Seller by slot index(FStruct_Inventory_Slot_Item Item to buy, UAC_Inventory_C* Seller Inventory Component, UAC_Inventory_C* Buyer Inventory Component, int32 Slot index of Item, int32 Amount to buy, FStruct_Inventory_Slot_Item Local Item to buy, UAC_Inventory_C* Local seller inventory component, UAC_Inventory_C* Local Buyer inventory component, int32 Local Amount to buy, int32 Local slot index of item);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Vendor|Utility")
	void For how many items does the buyer has resources(UAC_Inventory_C* Buyer Inventory Component, const FStruct_Inventory_Slot_Item& Item to be bought, UAC_Inventory_C* Seller Inventory Component, int32& Buyer has resources for this many items);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Vendor|Trade Item")
	void Process Buy item by trade action(TEnumAsByte<Enum_Trading_Action_Types> Trade Action, FStruct_Inventory_Slot_Item Item to buy, UAC_Inventory_C* Seller Inventory Component, UAC_Inventory_C* Buyer Inventory Component, int32 Custom Amount to buy, int32 Slot index of Item, FStruct_Inventory_Slot_Item Local Item to buy, UAC_Inventory_C* Local Seller inventory component, UAC_Inventory_C* Local buyer inventory component, int32 Local Custom amount to buy, int32 Local slot index, int32 Local Amount to buy);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Vendor|Trade Item")
	void Try to Automaticly Buy Item from Seller(FStruct_Inventory_Slot_Item Item to buy, UAC_Inventory_C* Seller Inventory Component, UAC_Inventory_C* Buyer Inventory Component, int32 Amount to buy, FStruct_Inventory_Slot_Item Local Item to buy, UAC_Inventory_C* Local seller inventory component, UAC_Inventory_C* Local Buyer inventory component, int32 Local Amount to buy, int32 Local slot index of item);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Restocking")
	void Restock Inventory(FStruct_Inventory_Slot_Item Local Item to be added);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Restocking")
	void Try Restock Item(const FStruct_Inventory_Slot_Item& Item to restock, FStruct_Inventory_Slot_Item Local Item to be added);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Restocking")
	void Reset Inventory Stock();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Equipment|Utility")
	void Check if equipment slot is empty, if not set to empty(TEnumAsByte<Enum_Equipment_Slots> Equipment Slot, bool& Slot is empty, int32& Equipment Slot index);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Equipment|Utility")
	void Get equipment slot index by equipment slot type(TEnumAsByte<Enum_Equipment_Slots> Equipment Slot, int32& Found slot index in equipment array of same equipment slot type, bool& found same equipment slot type, FStruct_Inventory_Slot_Item& Inventory Item in equipment slot, int32 local found slot index of same equipment slot type, bool local found same equipment slot type);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Equipment|Utility")
	void To which equipment slot should this item be added ? (FStruct_Inventory_Slot_Item Inventory Item, TEnumAsByte<Enum_Equipment_Slots>&Equipment Slot to add, bool& Empty slot found, bool& Is not equipment, TEnumAsByte<Enum_Equipment_Slots> local Equipment Slot to add, bool Found an empty slot to add to);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Equipment|Utility")
	void Check if equipment slot is empty(TEnumAsByte<Enum_Equipment_Slots> Equipment Slot, bool& Empty, FStruct_Inventory_Slot_Item& Inventory Item in slot, int32& At slot index in equipment array of same equipment slot type);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Equipment|Equip")
	void Automaticly Equip Item(FStruct_Inventory_Slot_Item Item to equip, int32 Source Slot index, UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Target Array, bool Is from hotbar, bool& Item is equiped, FStruct_Inventory_Slot_Item& Equiped Item, TEnumAsByte<Enum_Equipment_Slots>& Equiped to this equipement slot, bool& Item was already equiped.But added more to the stack, bool& Item is swapped, FStruct_Inventory_Slot_Item& Removed Item, bool& Item is not equiped since character does not meet level requirements, FStruct_Inventory_Slot_Item Local item to equip, int32 local found index slot in equipment array, TEnumAsByte<Enum_Equipment_Slots> local To which equipment slot to add, int32 local source slot index, FStruct_Inventory_Slot_Item Local Item found in equipment slot);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Equipment|Utility")
	void Find Equipment slot in equipment array(TEnumAsByte<Enum_Equipment_Slots> Which equipment slot to find, bool& equipment slot found, int32& index of found equipment slot in equipment array, FStruct_Inventory_Slot_Item& item found in equipment slot, bool& The found slot is empty ? , bool local equipment slot found, int32 Local index in equipment slot found, FStruct_Inventory_Slot_Item local item found in equipment slot);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Utility")
	void Swap Item(UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Source Array, UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Destination Array, int32 Source index, int32 Destination Index, UAC_Inventory_C* Source, UAC_Inventory_C* Destination, bool& Swap peformed, FStruct_Inventory_Slot_Item local source item, FStruct_Inventory_Slot_Item local destination item, int32 local source index, int32 local destination index, bool local is swapping equipment);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Equipment|Unequip")
	void Automaticly unequip Item(FStruct_Inventory_Slot_Item Item to unequip, int32 Source Index from equipment array, bool& Item is unequiped);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Equipment|Visual")
	void Equip_Visual(TEnumAsByte<Enum_Equipment_Slots> Equipment slot to change, FStruct_Inventory_Slot_Item Equipment to change, bool Is Removing, FName Local Socket Name, FStruct_Inventory_Slot_Item local equipment to change, UStaticMesh* Quiver Mesh, TEnumAsByte<Enum_Equipment_Slots> local equipment slot to change);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Equipment|Visual")
	void Attach Equipment to 3D Model Viewer(FName SocketName, UClass * Class, UStaticMesh * Static Mesh, AActor * &Equipped actor to 3D model viewer);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Recipe")
	void Learn Recipe(FStruct_Inventory_Slot_Item Recipe Item, int32 Source slot index, UAC_Inventory_C* Player Inventory Component, UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Target Array, UPARAM(ref) bool& Is from hotbar, bool& Success, FStruct_Inventory_Slot_Item Local item to learn, int32 local slot index);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Utility")
	void Array Contains Item(UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Array, FStruct_Inventory_Slot_Item Item to find, bool& Item Found in array, bool local Item Found in array);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Adding or Removing widget(UUserWidget * Widget, APlayerController * Player Controller);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Consume")
	void Consume_Item(FStruct_Inventory_Slot_Item Inventory Item to consume, int32 Slot index, UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Target Array, bool Is from hotbar);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Can Interact Trace(TArray<TEnumAsByte<EObjectTypeQuery> > Trace Object);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Level Requirement")
	void Is character meeting all level requirements ? (FStruct_Inventory_Slot_Item Item to use, bool& Is meeting level requirement, bool local does not meet level requirement);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Level Requirement")
	void Is meeting level requirement(FStruct_Skill_Level_Requirement Level  Requirement, bool& Is meeting level requirement, bool local does not meet level requirement);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Utility")
	void Can item be added due to weight ? (FStruct_Inventory_Slot_Item item, bool& All items can be added, int32 & Amount that can be added, int32 & Amount that can not be added, double local weight to be added, int32 local amount of item that can be added due to weight);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Restocking")
	void Restock Complete Category lists(UPARAM(ref) TArray<FStruct_Vendor_Sell_Categories_Details>& auto inlcude list, const TArray<TEnumAsByte<Enum_Item_Categories> >& Ignore categories, bool Local Item is ignored);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Recipe")
	void Set Default known crafting recipes();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Equipment|Initialization")
	void Initialize_Starting_Equipment();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_Inventory Slots();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_Equipment();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Utility")
	void Is_Valid_Inventory_Move(int32 Source Slot index, bool& Is valid move);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Equipment|Visual")
	void Equip Visual to 3D model viewer(TEnumAsByte<Enum_Equipment_Slots> Index, AActor * actor to equip, FName SocketName, UStaticMesh * Static Mesh, bool Is removing);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_Is on();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_Crafting Queue();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Equipment|Visual")
	void Equip_Or_Remove_SkeletalMesh(USkeletalMesh* Skeletal Mesh, TEnumAsByte<Enum_Equipment_Slots> Equipment slot to change, bool Is removing);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Pickup Item(ABP_Pickup_Master_C* InputPin, FStruct_Inventory_Slot_Item Inventory item);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Equipment|Utility")
	void Get Current Equipement at Slot(TEnumAsByte<Enum_Equipment_Slots> Equipment Slot, bool& Has found an weapon, TEnumAsByte<Enum_Weapon_Type>& Weapon Type, FStruct_Item& Item);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	void Excecute_Hotbar_Logic_At_Index(int32 Source Slot Index);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Save")
	void Save_Inventory_Component(USaveGame* Local Save Object);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Save")
	void Load Inventories(UBP_Save_Inventory_Component_C* Local save);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Save")
	void Get_Guid_Inventory(FString& Guid_Inventory);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	void Link_Item_To_Hotbar(int32 Inventory Slot Index, int32 Hotbar Slot Index, bool Is swapping on hotbar, bool Link to Equipment);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	void Delink_Item_from_hotbar(int32 Inventory Slot Index, int32 Hotbar Slot Index);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	void Swap_Hotbar_Slot(int32 From slot index, int32 To slot index);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_Hotbar_Slots();
public:
	/** Please add a variable description */
	static_assert(false, "You will need to add DOREPLIFETIME(UAC_Inventory, Inventory Slots) to GetLifetimeReplicatedProps");
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Inventory", ReplicatedUsing = "OnRep_Inventory Slots")
	TArray<FStruct_Inventory_Slot_Item> Inventory Slots;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Interaction")
	double Trace Length Pic kUp Items;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Generic")
	TObjectPtr<ACharacter> Owning Character Reference;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	bool Dynamic slot creation;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Initial Inventory")
	int32 Amount of default inventory slots;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Initial Inventory")
	TArray<FStruct_Inventory_Slot_Item> Starting items;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "UI", meta = (EditInline = "true"))
	TObjectPtr<UUserWidget> Inventory UI;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FString Inventory Name;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	TEnumAsByte<Enum_Inventory_Types> Inventory Type;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "默认")
	TObjectPtr<UAC_Inventory_C> Destination inventory component;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventory Updated);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "默认")
	FInventory Updated Inventory Updated;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory|Weight")
	bool Use Weight;

	/** Please add a variable description */
	static_assert(false, "You will need to add DOREPLIFETIME(UAC_Inventory, Learned Recipes) to GetLifetimeReplicatedProps");
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Recipes", Replicated)
	TArray<FStruct_Inventory_Slot_Item> Learned Recipes;

	/** Please add a variable description */
	static_assert(false, "You will need to add DOREPLIFETIME(UAC_Inventory, Crafting Queue) to GetLifetimeReplicatedProps");
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Crafting", ReplicatedUsing = "OnRep_Crafting Queue")
	TArray<FStruct_Inventory_Slot_Item> Crafting Queue;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Crafting")
	int32 Amount of Crafting Queue Slots;

	/** Please add a variable description */
	static_assert(false, "You will need to add DOREPLIFETIME(UAC_Inventory, Current Crafting time) to GetLifetimeReplicatedProps");
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Crafting", Replicated)
	double Current Crafting time;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Crafting")
	FTimerHandle Crafting queue timer handle;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Crafting")
	FStruct_Inventory_Slot_Item Current item being crafted;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Crafting")
	bool Dynamic slot creation for crafting queue;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Recipes")
	int32 Default amount of Recipe slots;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Equipment")
	bool Has Equipment;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Crafting")
	int32 Amount not added;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Overrides")
	int32 Max Split Amount override;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Crafting")
	double Crafting Speed Multiplier;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Overrides")
	int32 Max Craft Amount override;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "OnOff")
	bool Has turn on off;

	/** Please add a variable description */
	static_assert(false, "You will need to add DOREPLIFETIME(UAC_Inventory, Is on) to GetLifetimeReplicatedProps");
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "On Off", ReplicatedUsing = "OnRep_Is on")
	bool Is on;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "On Off")
	bool Requires Consume Fuel when on;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "On Off")
	double Fuel Consumption Rate;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "On Off")
	TArray<FStruct_Inventory_Slot_Item> Accepted Fuel;

	/** Please add a variable description */
	static_assert(false, "You will need to add DOREPLIFETIME(UAC_Inventory, current combustion time) to GetLifetimeReplicatedProps");
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "On Off", Replicated)
	double current combustion time;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "On Off")
	double Combustion Rate mutliplier;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "On Off")
	FTimerHandle Fuel Timer Handle;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "On Off")
	FStruct_Inventory_Slot_Item Current Combustible used;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Decay")
	TArray<int32> Item indexes to automaticly decay;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Decay")
	FTimerHandle Decay Timer Handle;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FInventory Slot Updated, FStruct_Inventory_Slot_Item, Item slot, int32, Slot Index, TArray<FStruct_Inventory_Slot_Item>&, target array);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "默认")
	FInventory Slot Updated Inventory Slot Updated;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Decay")
	double Decay Interval;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Vendor")
	bool IsVendor;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Vendor")
	FDataTableRowHandle TradeList;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Vendor")
	double Base Player Sell to Vendor price Multiplier;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Vendor")
	double Base Vendor Sell to player Price Multiplier;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Restock")
	double Current Restock Time;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Restock")
	double Original Restock Time;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Restock")
	FTimerHandle Restock Timer Handle;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Restock")
	FTimerHandle Inventory Reset Timer;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Restock")
	double Current Reset Time;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Restock")
	double Original Reset Time;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Decay")
	bool Override to no decay in this inventory;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Decay")
	double Decay multiplier for this inventory;

	/** Please add a variable description */
	static_assert(false, "You will need to add DOREPLIFETIME(UAC_Inventory, Equiped Equipment) to GetLifetimeReplicatedProps");
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Equipment", ReplicatedUsing = "OnRep_Equiped Equipment")
	TArray<FStruct_Inventory_Slot_Item> Equiped Equipment;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Attritube System")
	TArray<FStruct_Inventory_Slot_Item> Active Status modifications;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEquipment Added or Removed, FStruct_Inventory_Slot_Item, Inventory Item, bool, Item removed ? , TEnumAsByte<Enum_Equipment_Slots>, Equipment slot changed);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "默认")
	FEquipment Added or Removed Equipment Added or Removed;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Equipment")
	TArray<FStruct_Inventory_Slot_Item> Attached Equipment Actors;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Equipment|Attached Visuals")
	FStruct_Equipment_Visual Head;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Equipment|Attached Visuals")
	FStruct_Equipment_Visual Body;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Equipment|Attached Visuals")
	FStruct_Equipment_Visual Hands;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Equipment|Attached Visuals")
	FStruct_Equipment_Visual Legs;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Equipment|Attached Visuals")
	FStruct_Equipment_Visual Feet;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Equipment|Attached Visuals")
	FStruct_Equipment_Visual Necklace;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Equipment|Attached Visuals")
	FStruct_Equipment_Visual Ring 1;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Equipment|Attached Visuals")
	FStruct_Equipment_Visual Ring 2;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Equipment|Attached Visuals")
	FStruct_Equipment_Visual Primary Weapon;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Equipment|Attached Visuals")
	FStruct_Equipment_Visual Throwable;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Equipment|Attached Visuals")
	FStruct_Equipment_Visual Ammo;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Equipment|Attached Visuals")
	FStruct_Equipment_Visual Backpack;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Equipment|Attached Visuals")
	FStruct_Equipment_Visual Consumeable Slot 1;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Equipment|Attached Visuals")
	FStruct_Equipment_Visual Consumeable Slot 2;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Interaction")
	TObjectPtr<AActor> Current Actor interacting with;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Interaction", meta = (EditInline = "true"))
	TObjectPtr<UUserWidget> Current Active Widget;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Interaction")
	bool Is there a widget active;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Interaction")
	bool Is Equipment Panel open;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConsume Item, FStruct_Inventory_Slot_Item, Inventory ITem to Consume);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "默认")
	FConsume Item Consume Item;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Loot")
	bool Is Lootchest;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Loot")
	FDataTableRowHandle Loot_List;

	/** Please use with care. This means it will restock everytime the timer hits zero. This can have a performance impact! */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Restock")
	bool Has Restock Inventory;

	/** The reset is prefered above restock timer. Reset is only triggered when there is a change in the inventory. Restock is continious. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Restock")
	bool Has Reset Inventory;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Restock")
	bool Inventory has changed;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Restock")
	bool Override not able to start reset timer;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Interaction")
	TObjectPtr<AActor> Current pickup possible interaction;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sounds")
	TObjectPtr<USoundBase> Drop Item out of inventory Sound;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItem added or Removed, FStruct_Inventory_Slot_Item, Item slot, bool, Removed ? );
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "默认")
	FItem added or Removed Item added or Removed;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "默认")
	TObjectPtr<UCameraComponent> Camera Component reference;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "默认")
	TObjectPtr<UAC_Skill_and_Level_Manager_C> Skill and level manager Reference;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "默认")
	TObjectPtr<UAC_Attribute_Manager_C> attribute manager;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory|Weight")
	double Maximum Inventory Weight;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Inventory|Weight")
	double Encumbered Movement speed penalty in percent;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Equipment")
	TArray<FStruct_Inventory_Slot_Item> Starting_Equipment;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "默认")
	TObjectPtr<UAC_Combat_C> Combat component reference;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Generic")
	TObjectPtr<ABP_Quiver_C> Equiped Quiver Reference;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWorkstation Switched on / off, bool, Is On);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "默认")
	FWorkstation Switched on / off Workstation Switched on / off;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Interaction")
	TObjectPtr<ABP_Pickup_Master_C> current pickup interacting with;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> Active montage;

	/** Please add a variable description */
	static_assert(false, "You will need to add DOREPLIFETIME(UAC_Inventory, Temporary item reference to update) to GetLifetimeReplicatedProps");
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Generic", Replicated)
	FStruct_Inventory_Slot_Item Temporary item reference to update;

	/** Please add a variable description */
	static_assert(false, "You will need to add DOREPLIFETIME(UAC_Inventory, Temporary slot reference to update) to GetLifetimeReplicatedProps");
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Generic", Replicated)
	int32 Temporary slot reference to update;

	/** Please add a variable description */
	static_assert(false, "You will need to add DOREPLIFETIME(UAC_Inventory, Temporary array reference to update) to GetLifetimeReplicatedProps");
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Generic", Replicated)
	TArray<FStruct_Inventory_Slot_Item> Temporary array reference to update;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FQueue Updated);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "默认")
	FQueue Updated Queue Updated;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Inventory")
	TArray<FStruct_Inventory_Slot_Item> Inventory Slots_Previous;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Equipment")
	TArray<FStruct_Inventory_Slot_Item> Equiped Equipment_previous;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEquipment Slot Updated, FStruct_Inventory_Slot_Item, item slot, int32, slot index, TArray<FStruct_Inventory_Slot_Item>&, target array);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "默认")
	FEquipment Slot Updated Equipment Slot Updated;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdate Decay Info, int32, Slot Index, FStruct_Inventory_Slot_Item, Inventory Item);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "默认")
	FUpdate Decay Info Update Decay Info;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Crafting")
	bool Has_Crafting;

	/** Please add a variable description */
	static_assert(false, "You will need to add DOREPLIFETIME(UAC_Inventory, Hotbar_Slots) to GetLifetimeReplicatedProps");
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Hotbar", ReplicatedUsing = "OnRep_Hotbar_Slots")
	TArray<FStruct_Inventory_Slot_Item> Hotbar_Slots;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Save", meta = (ExposeOnSpawn = "true"))
	FString Guid_Inventory;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Hotbar")
	TArray<FStruct_Inventory_Slot_Item> Hotbar_Slots_Previous;
};
