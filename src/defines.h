#ifndef DEFINES_H
#define DEFINES_H

#include "./defines/pokes.h"
#include "./defines/get_attr.h"
#include "./defines/items.h"
#include "./defines/item_effects.h"

#define ALL_ITEMS 602
#define ALL_POKES 412
#define ALL_TMS 102
#define ALL_HMS 8
#define FIRST_TM_INDEX 378

#define BALL_POCKET_MAX_OLD 16
#define ITEM_POCKET_MAX_OLD 30
#define KEY_POCKET_MAX_OLD 30
#define TM_POCKET_MAX_OLD 64
#define BERRY_POCKET_MAX_OLD 46

#define BALL_POCKET_MAX_NEW 16
#define ITEM_POCKET_MAX_NEW 30
#define KEY_POCKET_MAX_NEW 30
#define TM_POCKET_MAX_NEW ALL_HMS + ALL_TMS
#define BERRY_POCKET_MAX_NEW 46

#define TUTOR_MOVES 64

#define expanded_bag_items_ptr 33804132

enum item_pocket{
    POCKET_ITEMS = 1,
    POCKET_BALLS, //0x2
    POCKET_TMS, //0x3
    POCKET_BERRIES, //0x4
    POCKET_KEYITEMS, //0x5
};

enum item_usage{
    ITEM_USAGE_MAIL = 0,
    ITEM_USAGE_OUTOFBATTLE, //0x1
    ITEM_USAGE_SELECT_BUTTON, //0x2
    ITEM_USAGE_POKEBLOCK, //0x3
    ITEM_USAGE_OTHER, //0x4
};

enum ball_index{
    BALL_MASTER, //0x0
    BALL_ULTRA, //0x1
    BALL_GREAT, //0x2
    BALL_POKE, //0x3
    BALL_SAFARI, //0x4
    BALL_NET, //0x5
    BALL_DIVE, //0x6
    BALL_NEST, //0x7
    BALL_REPEAT, //0x8
    BALL_TIMER, //0x9
    BALL_LUXURY, //0xA
    BALL_PREMIER, //0xB
    //new balls
    #if EXPANDED_POKEBALLS == true
    BALL_LEVEL, //0xC
    BALL_LURE, //0xD
    BALL_MOON, //0xE
    BALL_FRIEND, //0xF
    BALL_LOVE, //0x10
    BALL_HEAVY, //0x11
    BALL_FAST, //0x12
    BALL_SPORT, //0x13
    BALL_DUSK, //0x14
    BALL_QUICK, //0x15
    BALL_HEAL, //0x16
    BALL_CHERISH, //0x17
    BALL_PARK, //0x18
    BALL_DREAM, //0x19
    BALL_BEAST, //0x1A
    #endif // EXPANDED_POKEBALLS
};

#define Space 0x00
#define JumpLine 0xFE
#define BuffCharac 0xFD
#define Apos 0xB4
#define Comma 0xB8
#define Dot 0xAD
#define Exclam 0xAB
#define Interro 0xAC
#define Termin 0xFF
#define Dash 0xAE
#define Slash 0xBA

#define wait_for_pressed_key 0xFC, 0x09

#define Poke_e 0x1B

#define A_ 0xBB
#define B_ 0xBC
#define C_ 0xBD
#define D_ 0xBE
#define E_ 0xBF
#define F_ 0xC0
#define G_ 0xC1
#define H_ 0xC2
#define I_ 0xC3
#define J_ 0xC4
#define K_ 0xC5
#define L_ 0xC6
#define M_ 0xC7
#define N_ 0xC8
#define O_ 0xC9
#define P_ 0xCA
#define Q_ 0xCB
#define R_ 0xCC
#define S_ 0xCD
#define T_ 0xCE
#define U_ 0xCF
#define V_ 0xD0
#define W_ 0xD1
#define X_ 0xD2
#define Y_ 0xD3
#define Z_ 0xD4

#define a_ 0xD5
#define b_ 0xD6
#define c_ 0xD7
#define d_ 0xD8
#define e_ 0xD9
#define f_ 0xDA
#define g_ 0xDB
#define h_ 0xDC
#define i_ 0xDD
#define j_ 0xDE
#define k_ 0xDF
#define l_ 0xE0
#define m_ 0xE1
#define n_ 0xE2
#define o_ 0xE3
#define p_ 0xE4
#define q_ 0xE5
#define r_ 0xE6
#define s_ 0xE7
#define t_ 0xE8
#define u_ 0xE9
#define v_ 0xEA
#define w_ 0xEB
#define x_ 0xEC
#define y_ 0xED
#define z_ 0xEE

#endif /* DEFINES_H */





