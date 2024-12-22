#include "UAC_Inventory.h"

// ����ʵ�ֿ�ʼ���˺������ڲ���ָ�������Ʒ��Ŀ�������������ڵĲ�λ��������������ط��ز���
void UAC_Inventory::FindExistingSlotIndexesOfSpecifiedInventoryItem(FStruct_Inventory_Slot_Item InventoryItem,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    TArray<int32>& SlotsFoundAtIndexes,
    bool& Success,
    TArray<int32> FoundIndexes) {
    Success = false;  // ��ʼ�����ҳɹ���ʶΪfalse
    SlotsFoundAtIndexes.Empty();  // ������ڴ洢�ҵ��Ĳ�λ���������飬ȷ����ʼ״̬��ȷ

    // ����Ŀ��������
    for (int32 i = 0; i < TargetArray.Num(); ++i) {
        const FStruct_Inventory_Slot_Item& currentSlotItem = TargetArray[i];
        // �жϵ�ǰ��λ����Ʒ�Ƿ���ָ���Ŀ����Ʒ��ȣ����������ȵ��ж���������Ʒ����ָ������Լ������ؼ�������ȣ��ɸ���ʵ�ʽṹ�嶨�������
        if (currentSlotItem.ItemData == InventoryItem.ItemData &&
            currentSlotItem.Amount == InventoryItem.Amount&&
            // ������Ӹ������ԵıȽ��жϣ�������Ʒ��ΨһID�ȣ�����ṹ��������ض���Ļ�
            // currentSlotItem.UniqueID == InventoryItem.UniqueID
            ) {
            SlotsFoundAtIndexes.Add(i);  // ���ҵ��Ĳ�λ������ӵ����������
            Success = true;  // ����ҵ�������һ��ƥ��Ĳ�λ�����ò��ҳɹ���ʶΪtrue
        }
    }
}