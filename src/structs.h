#ifndef B_STRUCTS
#define B_STRUCTS

#include "types.h"
#include "defines.h"

struct item_data{
    u16 itemID;
    u16 quantity;
};

struct saveblock1{
    u8 data[0x498];
    struct item_data PC_items[50];
    struct item_data items_pocket[ITEM_POCKET_MAX_OLD];
    struct item_data key_items_pocket[KEY_POCKET_MAX_OLD];
    struct item_data balls_pocket[BALL_POCKET_MAX_OLD];
    struct item_data tms_pocket[TM_POCKET_MAX_OLD];
    struct item_data berries_pocket[BERRY_POCKET_MAX_OLD];
};

extern struct saveblock1* sav1_ptr;

struct item_ptr{
    void* ptr;
    u8 max;
};

struct bag_items_pointers{
    struct item_ptr items;
    struct item_ptr balls;
    struct item_ptr tms;
    struct item_ptr berries;
    struct item_ptr key_items;
};

extern struct bag_items_pointers bag_items_ptrs;

struct movenames{
    u8 letter[13];
};

struct movenames_all{
    struct movenames movename[400];
};

extern struct movenames_all* move_name_table;

struct bag_info_struct{
    void* callback;
    u8 player_location;
    u8 curr_pocket;
    u8 field6;
    u8 field7;
    u16 cursor_pos_in_pckt[5];
    u16 scroll_pos_in_pckt[5];
    void* pocket_strings_ptr1;
    void* pocket_strings_ptr2;
};

extern struct bag_info_struct bag_info;

struct pokemon{
	u32 PID;
	u32 OTID;
	char name[10];
	u16 language;
	u8 OT_name[7];
	u8 markings;
	u16 checksum;
	u16 padding_maybe;
	u8 data[48];
	u32 ailment;
	u8 level;
	u8 pokerus;
	u16 current_hp;
	u16 total_hp;
	u16 attack;
	u16 defense;
	u16 speed;
	u16 sp_attack;
	u16 sp_defense;
};

extern struct pokemon party_opponent[6];
extern struct pokemon party_player[6];

struct task{
    void* function;
    u8 id;
    u8 previous;
    u8 next;
    u8 priority;
    u16 private[16];
};

extern struct task tasks[16];

struct frame {
  u16 data;
  u16 duration;
};

struct rotscale_frame {
  u16 scale_delta_x;
  u16 scale_delta_y;
  u8 rot_delta;
  u8 duration;
  u16 field_6;
};

struct sprite{
    u8 y;
    u8 flags1;
    u16 x_and_flag2;
    u16 attr2;
    u16 rotscaleinfo;
};

struct coords16{
    u16 x;
    u16 y;
};

struct coords8{
    u8 x;
    u8 y;
};

struct object;
typedef void (*object_callback)(struct object*);

struct template {
  u16 tiles_tag;
  u16 pal_tag;
  struct sprite *oam;
  struct frame **animation;
  u32 *graphics;
  struct rotscale_frame **rotscale;
  object_callback callback;
};

struct object{
    struct sprite final_oam;
    struct frame **animation_table;
    void* gfx_table;
    void* rotscale_table;
    struct template* template;
    u32 field_18;
    void* callback;
    struct coords16 pos1;
    struct coords16 pos2;
    struct coords8 pos_neg_center;
    u8 anim_number;
    u8 anim_frame;
    u8 anim_delay_countdown;
    u8 anim_unkown_counter;
    u16 private[8];
    u8 in_use : 1;
    u8 flag2 : 1;
    u8 invisible : 1;
    u8 flags : 5;
    u8 bitfield2;
    u16 anim_data_offset;
    u8 field_42;
    u8 y_height_related;
};

struct image_resource{
    void *image;
    u16 size;
    u16 tag;
};

extern struct object objects[64];

struct item_struct
{
    u8 name[14];
    u16 index;
    u16 price;
    u8 held_effect;
    u8 held_effect_quality;
    void *desc_pointer;
    u16 mystery_value;
    enum item_pocket pocket_no;
    u8 type_of_item;
    void *field_usage_code;
    u32 battle_usage;
    void *battle_usage_code;
    u32 extra_param;
};

#endif /* B_STRUCTS */
