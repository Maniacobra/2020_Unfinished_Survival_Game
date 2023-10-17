#pragma once
#ifndef INVENTORY_H_INCLUDED
#define INVENTORY_H_INCLUDED

#include "items.h"
#include "log.h"
#include "program_const.h"

class Inventory
{
	ItemStack m_items[INVENTORY_SIZE];

public:

	Inventory(void);
	bool add_items(ItemStack stack);
	void remove_items(int slot, int number);
	void remove_items(ItemStack items);
	void swap_items(int slot1, int slot2);
	ItemStack& get_items(int slot);
};

#endif // !INVENTORY_H_INCLUDED
