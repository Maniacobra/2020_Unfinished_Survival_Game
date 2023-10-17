#include "inventory.h"

Inventory::Inventory(void)
{
	for (int i = 0; i < INVENTORY_SIZE; i++)
	{
		m_items[i] = ItemStack(ITEM_NOTHING, 0);
	}
}

bool Inventory::add_items(ItemStack stack)
{	
	for (int i = 0; i < INVENTORY_SIZE; i++)
		if (m_items[i].add_items(stack))
			break;
	if (stack.get_number() != 0)
	{
		for (int i = 0; i < INVENTORY_SIZE; i++)
		{
			if (m_items[i].get_item() == ITEM_NOTHING)
			{
				m_items[i] = stack;
				break;
			}
		}
	}
	return stack.get_number() == 0;
}

void Inventory::remove_items(int slot, int number)
{
	m_items[slot].remove_items(number);
	if (m_items[slot].get_number() <= 0)
		m_items[slot] = ItemStack();
}

void Inventory::remove_items(ItemStack items)
{
	for (int i = INVENTORY_SIZE - 1; i >= 0; i--)
	{
		if (m_items[i].get_item() == items.get_item())
		{
			int rest = m_items[i].remove_items(items.get_number());
			if (rest > 0)
			{
				items = ItemStack(items.get_item(), rest);
				m_items[i] = ItemStack();
			}
			else if (rest == 0)
			{	
				m_items[i] = ItemStack();
				break;
			}
			else
				break;
		}
	}
}

void Inventory::swap_items(int slot1, int slot2)
{
	ItemStack items_slot1 = m_items[slot1];
	m_items[slot1] = m_items[slot2];
	m_items[slot2] = items_slot1;
}

ItemStack& Inventory::get_items(int slot)
{
	return m_items[slot];
}
