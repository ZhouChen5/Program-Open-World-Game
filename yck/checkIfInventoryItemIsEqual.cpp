#include "UAC_Inventory.h"

// ����ʵ�ֿ�ʼ���˺������ڱȽ���������λ��Ʒ�Ƿ���ȣ���������Ӧ�Ĳ������ز���
void UAC_Inventory::checkIfInventoryItemIsEqual(const FStruct_Inventory_Slot_Item& Struct_Inventory_Item1,
    const FStruct_Inventory_Slot_Item& Struct_Inventory_Item2,
    bool& True) {
    True = false;  // ��Ĭ������Ϊ����ȣ���False

    // �Ƚ�������Ʒ�Ĺؼ��������ж��Ƿ���ȣ��������ؼ����԰�����Ʒ����ָ�롢������ΨһID�ȣ��ɸ���ʵ�ʽṹ�嶨�������
    if (Struct_Inventory_Item1.ItemData == Struct_Inventory_Item2.ItemData &&
        Struct_Inventory_Item1.Amount == Struct_Inventory_Item2.Amount &&
        Struct_Inventory_Item1.UniqueID == Struct_Inventory_Item2.UniqueID) {
        True = true;  // �������ؼ����Զ���ȣ�������Ϊ��ȣ���True
    }
}