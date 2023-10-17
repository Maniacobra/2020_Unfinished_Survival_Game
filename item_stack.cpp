#include "items.h"

ItemStack::ItemStack(void)
{
	m_item = ITEM_NOTHING;
	m_amount = 0;
}

ItemStack::ItemStack(ItemType item, int number)
{
	m_item = item;
	m_amount = number;
}

bool ItemStack::add_items(ItemStack& stack, bool nolimit)
{
	if (m_item == ITEM_NOTHING || m_item != stack.get_item() || (!nolimit && m_amount >= get_properties().stack_max_size))
		return false;
	if (!nolimit && m_amount + stack.get_number() > ItemPropertiesList[m_item].stack_max_size)
	{
		stack.remove_items(get_properties().stack_max_size - m_amount);
		m_amount = get_properties().stack_max_size;
		return false;
	}
	m_amount += stack.get_number();
	stack = ItemStack();
	return true;
}

int ItemStack::remove_items(int number)
{
	m_amount -= number;
	return -m_amount;
}

ItemProperties ItemStack::get_properties(void) const
{
	return ItemPropertiesList[m_item];
}

ItemType ItemStack::get_item(void) const
{
	return m_item;
}

int ItemStack::get_number(void) const
{
	return m_amount;
}

