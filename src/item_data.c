#include "types.h"
#include "defines.h"
#include "functions.h"
#include <stdlib.h>
#include "locations.h"


const struct item_struct item_table[ALL_ITEMS] =
{
    //0. NO ITEM
    {
        /*Name*/        {Interro, Interro, Interro, Interro, Interro, Interro, Interro, Interro, Termin, 0x0, 0x0, 0x0, 0x0, 0x0},
        /*Index*/       ITEM_NONE,
        /*Price*/       0x0,
        /*Held Effect*/ ITEM_EFFECT_NOEFFECT,
        /*Quality*/     0x0,
        /*Desc. Ptr.*/  (void *)(0x857FEA0),
        /*Mystery Val*/ 0x0,
        /*Pocket*/      POCKET_ITEMS,
        /*Item type*/   ITEM_USAGE_OTHER,
        /*Field. Ptr.*/ (void *)(0x8FE821|1),
        /*Battle Usage*/0x0,
        /*Battle Ptr.*/ (void *)(0xF8000000),
        /*Extra Param*/ 0x0
    },

    //1. MASTER BALL
    {
        /*Name*/        {M_, a_, s_, t_, e_, r_, Space, B_, a_, l_, l_, Termin, 0x0, 0x0},
        /*Index*/       ITEM_MASTERBALL,
        /*Price*/       0x0,
        /*Held Effect*/ ITEM_EFFECT_NOEFFECT,
        /*Quality*/     0x0,
        /*Desc. Ptr.*/  (void *)(0x857FEA6),
        /*Mystery Val*/ 0x0,
        /*Pocket*/      POCKET_BALLS,
        /*Item type*/   BALL_MASTER,
        /*Field. Ptr.*/ (void *)(0x8FE821|1),
        /*Battle Usage*/0x2,
        /*Battle Ptr.*/ (void *)(0xF8000000),
        /*Extra Param*/ 0x0
    },
};
