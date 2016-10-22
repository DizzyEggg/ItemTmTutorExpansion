#ifndef VANILLA_FUNCTIONS
#define VANILLA_FUNCTIONS

#include "types.h"
#include "structs.h"

u8 get_attributes(struct pokemon* poke_address, u8 request, void* destination);
void buffer_item(u16 itemID, void* text_buffer);
void* strcpy_xFF_terminated_0(void* dst, void* src);
void* int_to_string(void* dst, u32 number, u8 formatting, u8 max_digits);
void* fdecode_and_copy(void* dst, void* src);
u8 check_if_move_learnt(struct pokemon* poke_addres, u16 move);
void bag_print_msg(u8 taskID, u8 arg, void* text_ptr, void* callback);
void TM_boot_sound_and_print(u8 taskID);

#endif /* VANILLA_FUNCTIONS */
