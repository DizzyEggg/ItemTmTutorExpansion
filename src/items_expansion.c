#include "types.h"
#include "defines.h"
#include "functions.h"
#include <stdlib.h>
#include "locations.h"

u16 TM_move_table[ALL_TMS + ALL_HMS] = {
    264, 337, 352, 347, 46, 92, 258, 339, 331, 237,
    241, 269, 58, 59, 63, 113, 182, 240, 202, 219, 218,
    76, 231, 85, 87, 89, 216, 91, 94, 247, 280, 104,
    115, 351, 53, 188, 201, 126, 317, 332, 259, 263,
    290, 156, 213, 168, 211, 285, 289, 315, 15, 19,
    57, 70, 148, 249, 127, 291, [50 ... ALL_TMS + ALL_HMS - 1] = 1
};

u16 Tutor_Move_Table[TUTOR_MOVES] = {
    5, 14, 25, 34, 38, 68, 69, 102, 118, 135, 138, 86, 153,
    157, 164, 223, 205, 244, 173, 196, 203, 189, 8, 207, 214,
    129, 111, 9, 7, 210, [31 ... TUTOR_MOVES - 1] = 1
};

u16 valid_item_check(u16 item)
{
    if (item == 377 || item > ALL_ITEMS)
        return 0;
    return item;
}

u16 get_TM_move(u16 item_ID)
{
    if (item_ID < FIRST_TM_INDEX || !valid_item_check(item_ID))
        return 0;
    return TM_move_table[item_ID - FIRST_TM_INDEX];
}

u16 get_tutor_move(u8 tutorID)
{
    return Tutor_Move_Table[tutorID];
}

u8 is_move_a_HM(u16 move)
{
    for (u8 i = ALL_TMS; i < ALL_HMS + ALL_TMS; i++)
    {
        if (TM_move_table[i] == move)
            return 1;
    }
    return 0;
}

void TM_field(u8 taskID)
{
    void* text_to_display;
    if (var800E_last_used_item >= ALL_TMS + 378)
        text_to_display = (void*) 0x85E9047;
    else
        text_to_display = (void*) 0x85E9037;
    bag_print_msg(taskID, 1, text_to_display, &TM_boot_sound_and_print);
}

u32 check_bitfield_comp(u16 species, u16 bitID, u32 table_ptr, u8 words_no)
{
    if (species == 412 || species > ALL_POKES)
        return 0;
    u8 adder = 0;
    if (bitID > 31 && bitID <= 63)
    {
        adder = 1;
        bitID -= 32;
    }
    else if (bitID > 63 && bitID <= 95)
    {
        adder = 2;
        bitID -= 64;
    }
    else if (bitID > 95 && bitID <= 127)
    {
        adder = 3;
        bitID -= 96;
    }
    u32* bitfield = (u32*) table_ptr + (species * words_no + adder);
    return bits_table[bitID] & *bitfield;
}

u32 check_TUTOR_compatibility(struct pokemon* poke, u16 tutorID)
{
    u8 word_no;
    if (TUTOR_MOVES <= 32)
        word_no = 1;
    else if (TUTOR_MOVES <= 64)
        word_no = 2;
    else if (TUTOR_MOVES > 64 && TUTOR_MOVES <= 96)
        word_no = 3;
    else
        word_no = 4;
    return check_bitfield_comp(get_attributes(poke, ATTR_SPECIES_2, 0), tutorID, tutor_comp_ptr, word_no);
}

u32 check_TM_compatibility(u16 species, u16 TM_id)
{
    return check_bitfield_comp(species, TM_id, tm_comp_ptr, 4);
}

u32 check_TM_compatibility2(struct pokemon* poke, u16 TM_id)
{
    return check_TM_compatibility(get_attributes(poke, ATTR_SPECIES_2, 0), TM_id);
}

void bag_buffer_item(void* text_buffer, u16 itemID)
{
    void* string_to_decode;
    switch (bag_info.curr_pocket)
    {
    case 2: //tms and hms
        strcpy_xFF_terminated_0(&script_text_buff2[0], &move_name_table->movename[get_TM_move(itemID)].letter[0]);
        if (itemID >= FIRST_TM_INDEX + ALL_TMS)
        {
            int_to_string(&script_text_buff1[0], itemID - (FIRST_TM_INDEX + ALL_TMS) + 1, 2, 1);
            string_to_decode = (void*) 0x085E921A;
        }
        else
        {
            u16 TM_no = itemID - FIRST_TM_INDEX + 1;
            u8 formatting = 2;
            u8 digits = 2;
            if (TM_no > 99)
            {
                formatting = 0;
                digits = 3;
            }
            int_to_string(&script_text_buff1[0], TM_no, formatting, digits);
            string_to_decode = (void*) 0x085E9210;
        }
        break;
    case 3: //berries
        int_to_string(&script_text_buff1[0], itemID - 132, 2, 2);
        buffer_item(itemID, &script_text_buff2[0]);
        string_to_decode = (void*) 0x085E9210;
        break;
    default:
        buffer_item(itemID, text_buffer);
        return;
    }
    fdecode_and_copy(text_buffer, string_to_decode);
    return;
}

enum move_comp{
    CAN_LEARN = 0,
    CANT_LEARN = 1,
    ALREADY_KNOWS = 2,
    IS_EGG = 3
};

enum move_comp get_pokemove_compatibility(struct pokemon* poke, u16 itemID, u8 movetutorID)
{
    if (get_attributes(poke, ATTR_IS_EGG, 0))
        return IS_EGG;
    itemID = valid_item_check(itemID);
    u16 wanted_move;
    if (itemID >= FIRST_TM_INDEX)
    {
        if (!check_TM_compatibility2(poke, itemID - FIRST_TM_INDEX))
            return CANT_LEARN;
        wanted_move = get_TM_move(itemID);
    }
    else //move tutor move
    {
        if (!check_TUTOR_compatibility(poke, movetutorID))
            return CANT_LEARN;
        wanted_move = get_tutor_move(movetutorID);
    }
    if (check_if_move_learnt(poke, wanted_move))
        return ALREADY_KNOWS;
    else
        return CAN_LEARN;
}

u8* bag_ptr_update(u8 max_new, u8 max_old, struct item_ptr* item, void* default_ptr, u8* expanded_ptr)
{
    item->max = max_new;
    if (max_new > max_old)
    {
        item->ptr = expanded_ptr;
        expanded_ptr += (max_new * sizeof(struct item_data));
    }
    else
        item->ptr = default_ptr;
    return expanded_ptr;
}

void update_bag_ptrs()
{
    u8* expanded_bag = (u8*) expanded_bag_items_ptr;
    //items pocket
    expanded_bag = bag_ptr_update(ITEM_POCKET_MAX_NEW, ITEM_POCKET_MAX_OLD, &bag_items_ptrs.items, &sav1_ptr->items_pocket, expanded_bag);
    //key items pocket
    expanded_bag = bag_ptr_update(KEY_POCKET_MAX_NEW, KEY_POCKET_MAX_OLD, &bag_items_ptrs.key_items, &sav1_ptr->key_items_pocket, expanded_bag);
    //balls pocket
    expanded_bag = bag_ptr_update(BALL_POCKET_MAX_NEW, BALL_POCKET_MAX_OLD, &bag_items_ptrs.balls, &sav1_ptr->balls_pocket, expanded_bag);
    //TM pocket
    expanded_bag = bag_ptr_update(TM_POCKET_MAX_NEW, TM_POCKET_MAX_OLD, &bag_items_ptrs.tms, &sav1_ptr->tms_pocket, expanded_bag);
    //berries pocket
    expanded_bag = bag_ptr_update(BERRY_POCKET_MAX_NEW, BERRY_POCKET_MAX_OLD, &bag_items_ptrs.berries, &sav1_ptr->berries_pocket, expanded_bag);
    return;
}

void get_ptrs_for_bag_strings()
{
    bag_info.pocket_strings_ptr1 = malloc((TM_POCKET_MAX_NEW + 1) * 8);
    bag_info.pocket_strings_ptr2 = malloc((TM_POCKET_MAX_NEW + 1) * 24);
}
