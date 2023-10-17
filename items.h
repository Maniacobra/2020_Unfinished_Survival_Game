#pragma once
#ifndef ITEMS_H_INCLUDED
#define ITEMS_H_INCLUDED

#include <string>

#include "block_enum.h"

enum ItemType
{	
	ITEM_NOTHING,
	ITEM_COBBLESTONE,
	ITEM_LOG,
	ITEM_LOGS,
	ITEM_PICKAXE,
	ITEM_ROCK,
	ITEM_STICK,
};

struct ItemProperties
{
	std::string texture_name;
	int associated_block;
	int stack_max_size;
};

static const struct ItemProperties ItemPropertiesList[] =
{
	{"", BLOCK_VOID, 0}, // NOTHING
	{"cobblestone", BLOCK_COBBLESTONE, 20}, // COBBLESTONE
	{"log", BLOCK_VOID, 10}, // LOG
	{"logs", BLOCK_LOGS, 20}, // LOGS
	{"pickaxe", BLOCK_VOID, 1}, // PICKAXE
	{"rock", BLOCK_VOID, 50}, // ROCK
	{"stick", BLOCK_STICKS, 30}, // STICK
};

class ItemStack
{
	ItemType m_item;
	int m_amount;

public:

	ItemStack(void);
	ItemStack(ItemType item, int number);
	bool add_items(ItemStack& stack, bool nolimit=false);
	int remove_items(int number);
	// Const
	ItemProperties get_properties(void) const;
	ItemType get_item(void) const;
	int get_number(void) const;
};

#endif // !ITEMS_H_INCLUDED
