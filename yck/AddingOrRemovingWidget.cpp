#include "UAC_Inventory.h"

// 函数实现开始，此函数根据传入的小部件和玩家控制器，执行添加或移除小部件的操作
void UAC_Inventory::AddingOrRemovingWidget(UUserWidget* Widget, APlayerController* PlayerController) {
    if (Widget && PlayerController) {
        if (Widget->IsInViewport()) {
            // 如果小部件已在视口中（即已显示），则移除它
            Widget->RemoveFromViewport();
        }
        else {
            // 如果小部件不在视口中，则添加它到视口中显示
            PlayerController->AddWidget(Widget);
        }
    }
}

void UAC_Inventory::Consume_Item(FStruct_Inventory_Slot_Item InventoryItemToConsume,
    int32 SlotIndex,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    bool IsFromHotbar) {
    // 先判断槽位索引是否合法
    if (SlotIndex < 0 || SlotIndex >= TargetArray.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& targetSlot = TargetArray[SlotIndex];
    // 判断要消耗的物品是否与槽位中的物品匹配（这里简单以物品数据指针相等为例，可按需完善）
    if (targetSlot.ItemData == InventoryItemToConsume.ItemData) {
        // 减少物品数量（这里简单示例，可根据实际物品消耗规则调整，比如按数量消耗、按比例消耗等）
        targetSlot.Amount--;
        if (targetSlot.Amount == 0) {
            // 如果数量减为0，移除该物品槽位
            TargetArray.RemoveAt(SlotIndex);
        }

        // 如果是从快捷栏消耗，可执行额外的快捷栏相关逻辑（比如更新快捷栏显示等，这里简单示例，可按需扩展）
        if (IsFromHotbar) {
            HandleHotbarConsumeLogic(SlotIndex);
        }

        // 这里还可以添加消耗物品后引发的其他逻辑，比如角色属性变化等（假设存在相应函数来处理）
        ApplyConsumeEffect(InventoryItemToConsume);
    }
}

void UAC_Inventory::CanInteractTrace(TArray<TEnumAsByte<EObjectTypeQuery> > TraceObject) {
    // 这里可以使用UE5的碰撞查询、射线检测等机制来判断是否能追踪到符合条件的对象
    // 示例：创建一个碰撞查询参数结构体实例
    FCollisionQueryParams queryParams;
    queryParams.AddIgnoredActor(GetOwner());  // 忽略自身

    TArray<FHitResult> hitResults;
    bool bHitAnything = false;

    // 遍历追踪对象类型数组，针对每种类型进行检测（这里简单示例，实际可能更复杂）
    for (TEnumAsByte<EObjectTypeQuery> objectType : TraceObject) {
        // 进行射线检测（这里假设从角色位置向前发射射线，可根据实际调整起点、方向等参数）
        bHitAnything = GetWorld()->LineTraceMultiByObjectType(hitResults, GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * InteractionTraceDistance, objectType, queryParams);
        if (bHitAnything) {
            break;
        }
    }

    // 根据是否检测到对象，可进行后续相关设置或返回结果（这里简单示例，可根据实际需求完善逻辑）
    if (bHitAnything) {
        SetCanInteract(true);
    }
    else {
        SetCanInteract(false);
    }
}

void UAC_Inventory::IsCharacterMeetingAllLevelRequirements(FStruct_Inventory_Slot_Item ItemToUse,
    bool& IsMeetingLevelRequirement,
    bool LocalDoesNotMeetLevelRequirement) {
    IsMeetingLevelRequirement = false;
    LocalDoesNotMeetLevelRequirement = true;

    // 假设存在获取角色当前等级的函数GetCharacterLevel
    int32 characterLevel = GetCharacterLevel();

    // 检查物品是否有等级要求以及角色等级是否满足（这里假设物品结构体中有RequiredLevel成员变量表示所需等级）
    if (ItemToUse.ItemData && ItemToUse.ItemData->RequiredLevel <= characterLevel) {
        IsMeetingLevelRequirement = true;
        LocalDoesNotMeetLevelRequirement = false;
    }
}

void UAC_Inventory::CanItemBeAddedDueToWeight(FStruct_Inventory_Slot_Item item,
    bool& AllItemsCanBeAdded,
    int32& AmountThatCanBeAdded,
    int32& AmountThatCanNotBeAdded,
    double LocalWeightToBeAdded,
    int32 LocalAmountOfItemThatCanBeAddedDueToWeight) {
    AllItemsCanBeAdded = false;
    AmountThatCanBeAdded = 0;
    AmountThatCanNotBeAdded = item.Amount;
    LocalWeightToBeAdded = item.ItemData->Weight * item.Amount;
    LocalAmountOfItemThatCanBeAddedDueToWeight = 0;

    // 假设存在获取当前库存总重量的函数GetCurrentInventoryWeight以及库存重量限制属性MaxInventoryWeight
    double currentWeight = GetCurrentInventoryWeight();
    double maxWeight = MaxInventoryWeight;

    // 计算剩余可承载重量
    double remainingWeight = maxWeight - currentWeight;

    // 判断物品能否添加以及可添加的数量（根据重量限制和物品单个重量计算）
    if (LocalWeightToBeAdded <= remainingWeight) {
        AllItemsCanBeAdded = true;
        AmountThatCanBeAdded = item.Amount;
        AmountThatCanNotBeAdded = 0;
        LocalAmountOfItemThatCanBeAddedDueToWeight = item.Amount;
    }
    else {
        // 如果不能全部添加，计算可添加的部分数量（这里简单按重量整除计算，可根据实际优化）
        LocalAmountOfItemThatCanBeAddedDueToWeight = FMath::FloorToInt(remainingWeight / item.ItemData->Weight);
        AmountThatCanBeAdded = LocalAmountOfItemThatCanBeAddedDueToWeight;
        AmountThatCanNotBeAdded = item.Amount - LocalAmountOfItemThatCanBeAddedDueToWeight;
    }
}

void UAC_Inventory::RestockCompleteCategoryLists(UPARAM(ref) TArray<FStruct_Vendor_Sell_Categories_Details>& AutoIncludeList,
    const TArray<TEnumAsByte<Enum_Item_Categories> >& IgnoreCategories,
    bool LocalItemIsIgnored) {
    LocalItemIsIgnored = false;

    // 遍历自动包含列表，检查每个类别是否在忽略类别数组中
    for (int32 i = 0; i < AutoIncludeList.Num(); ++i) {
        FStruct_Vendor_Sell_Categories_Details& categoryDetails = AutoIncludeList[i];
        LocalItemIsIgnored = false;
        for (TEnumAsByte<Enum_Item_Categories> ignoreCategory : IgnoreCategories) {
            if (categoryDetails.Category == ignoreCategory) {
                LocalItemIsIgnored = true;
                break;
            }
        }

        // 如果该类别要被忽略，从自动包含列表中移除它（这里简单示例，可根据实际调整移除逻辑）
        if (LocalItemIsIgnored) {
            AutoIncludeList.RemoveAt(i);
            i--;  // 索引回退，因为移除了一个元素
        }
    }
}

void UAC_Inventory::SetDefaultKnownCraftingRecipes() {
    // 假设存在一个默认配方数组DefaultCraftingRecipes（里面包含了FStruct_Inventory_Slot_Item类型的配方物品信息等）
    for (const FStruct_Inventory_Slot_Item& recipeItem : DefaultCraftingRecipes) {
        // 调用添加已知配方的函数（假设存在AddKnownRecipe函数）将配方添加到玩家已知配方列表
        AddKnownRecipe(recipeItem);
    }
}

void UAC_Inventory::Initialize_Starting_Equipment() {
    // 假设存在一个初始装备数组StartingEquipment（包含了FStruct_Inventory_Slot_Item类型的初始装备物品信息）
    for (const FStruct_Inventory_Slot_Item& equipmentItem : StartingEquipment) {
        // 根据装备物品的类型等信息，确定其对应的装备槽位（这里假设存在相应函数来确定槽位）
        TEnumAsByte<Enum_Equipment_Slots> equipmentSlot = DetermineEquipmentSlot(equipmentItem.ItemData);

        // 将装备添加到对应的装备槽位（这里简单添加，实际可能涉及更多规则，如已有装备的处理等）
        EquipmentSlots.Add(equipmentItem);

        // 这里还可以添加设置装备初始属性、绑定视觉效果等相关的初始化逻辑（假设存在相应函数）
        SetupEquipmentInitialAttributes(equipmentItem);
        BindEquipmentVisuals(equipmentItem);
    }
}

void UAC_Inventory::OnRep_InventorySlots() {
    // 这里可以添加更新UI显示库存槽位变化的逻辑（假设存在相应函数来更新UI）
    UpdateInventoryUI();

    // 也可以将最新的库存槽位数据同步到本地缓存等其他相关操作（假设存在相应的数据操作函数）
    SyncInventoryDataToLocalCache();
}

void UAC_Inventory::OnRep_Equipment() {
    // 更新角色外观以体现装备变化（假设存在相应函数来更新角色外观的视觉效果）
    UpdateCharacterEquipmentVisuals();

    // 同步装备数据到本地缓存等相关操作（假设存在相应的数据操作函数）
    SyncEquipmentDataToLocalCache();
}