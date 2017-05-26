#include "defines.h"

#define AFFECT_INDOORS      false
#define AFFECT_UNDERWATER   false

#define RED(val)((val & 0x1F))
#define GRN(val)(((val & 0x3E0)  >>  (5)))
#define BLU(val)(((val & 0x7C00) >> (10)))

inline u16 apply_color(u16 red, u16 green, u16 blue)
{
    return red | (green << 5) | (blue << 10);
}

static u16 blend(u32 ARGB, u16 RGB)
{
    u32 Alp8 = (ARGB & 0xFF000000) >> (24+3);
    u32 Red8 = (ARGB & 0x00FF0000) >> (16+3);
    u32 Grn8 = (ARGB & 0x0000FF00) >> (8+3);
    u32 Blu8 = (ARGB & 0x000000FF) >> (0+3);

    u16 red5 = RED(RGB), grn5 = GRN(RGB), blu5 = BLU(RGB);

    u8 rem = 0x1F - Alp8; // Remaining fraction

    u16 NewRed = ((red5*rem + Red8*Alp8) / 0x1F);
    u16 NewGrn = ((grn5*rem + Grn8*Alp8) / 0x1F);
    u16 NewBlu = ((blu5*rem + Blu8*Alp8) / 0x1F);

    return apply_color(NewRed, NewGrn, NewBlu);
}

const u32 TimeBlendColors[] = {
    0x502A00A8,	//	00:00	0
    0x502A00A8,	//	00:10	1
    0x502A00A8,	//	00:20	2
    0x502A00A8,	//	00:30	3
    0x502A00A8,	//	00:40	4
    0x502A00A8,	//	00:50	5
    0x502A00A8,	//	01:00	6
    0x502A00A8,	//	01:10	7
    0x502A00A8,	//	01:20	8
    0x502A00A8,	//	01:30	9
    0x502A00A8,	//	01:40	10
    0x502A00A8,	//	01:50	11
    0x502A00A8,	//	02:00	12
    0x502A00A8,	//	02:10	13
    0x502A00A8,	//	02:20	14
    0x502A00A8,	//	02:30	15
    0x502A00A8,	//	02:40	16
    0x502A00A8,	//	02:50	17
    0x502A00A8,	//	03:00	18
    0x4D3102A4,	//	03:10	19
    0x4B3805A0,	//	03:20	20
    0x4940089C,	//	03:30	21
    0x47470B98,	//	03:40	22
    0x454E0E95,	//	03:50	23
    0x43561191,	//	04:00	24
    0x415D148D,	//	04:10	25
    0x3F641789,	//	04:20	26
    0x3D6C1A86,	//	04:30	27
    0x3B731D82,	//	04:40	28
    0x397A207E,	//	04:50	29
    0x3782237A,	//	05:00	30
    0x35892677,	//	05:10	31
    0x33902973,	//	05:20	32
    0x30982C6F,	//	05:30	33
    0x2E9F2F6B,	//	05:40	34
    0x2CA63268,	//	05:50	35
    0x2AAE3564,	//	06:00	36
    0x28B53860,	//	06:10	37
    0x26BC3B5C,	//	06:20	38
    0x24C43E59,	//	06:30	39
    0x22CB4155,	//	06:40	40
    0x20D24451,	//	06:50	41
    0x1EDA474D,	//	07:00	42
    0x1CE14A4A,	//	07:10	43
    0x1AE84D46,	//	07:20	44
    0x18F05042,	//	07:30	45
    0x16F7533E,	//	07:40	46
    0x14FF563B,	//	07:50	47
    0x14FF563B,	//	08:00	48
    0x13F75339,	//	08:10	49
    0x12F05137,	//	08:20	50
    0x12E94E35,	//	08:30	51
    0x11E14C34,	//	08:40	52
    0x11DA4932,	//	08:50	53
    0x10D34730,	//	09:00	54
    0x10CC442F,	//	09:10	55
    0x0FC4422D,	//	09:20	56
    0x0EBD3F2B,	//	09:30	57
    0x0EB63D2A,	//	09:40	58
    0x0DAE3A28,	//	09:50	59
    0x0DA73826,	//	10:00	60
    0x0CA03625,	//	10:10	61
    0x0C993323,	//	10:20	62
    0x0B913121,	//	10:30	63
    0x0A8A2E20,	//	10:40	64
    0x0A832C1E,	//	10:50	65
    0x097B291C,	//	11:00	66
    0x0974271A,	//	11:10	67
    0x086D2419,	//	11:20	68
    0x07652217,	//	11:30	69
    0x075E1F15,	//	11:40	70
    0x06571D14,	//	11:50	71
    0x06501B12,	//	12:00	72
    0x05481810,	//	12:10	73
    0x0541160F,	//	12:20	74
    0x043A130D,	//	12:30	75
    0x0332110B,	//	12:40	76
    0x032B0E0A,	//	12:50	77
    0x02240C08,	//	13:00	78
    0x021D0906,	//	13:10	79
    0x01150705,	//	13:20	80
    0x010E0403,	//	13:30	81
    0x00070201,	//	13:40	82
    0x00000000,	//	13:50	83
    0x00000000,	//	14:00	84
    0x01020000,	//	14:10	85
    0x03040000,	//	14:20	86
    0x05060000,	//	14:30	87
    0x06080000,	//	14:40	88
    0x080A0000,	//	14:50	89
    0x0A0C0000,	//	15:00	90
    0x0C0E0000,	//	15:10	91
    0x0D100000,	//	15:20	92
    0x0F120000,	//	15:30	93
    0x11140000,	//	15:40	94
    0x13160000,	//	15:50	95
    0x14190100,	//	16:00	96
    0x161B0100,	//	16:10	97
    0x181D0100,	//	16:20	98
    0x1A1F0100,	//	16:30	99
    0x1B210100,	//	16:40	100
    0x1D230100,	//	16:50	101
    0x1F250100,	//	17:00	102
    0x21270100,	//	17:10	103
    0x22290100,	//	17:20	104
    0x242B0100,	//	17:30	105
    0x262D0100,	//	17:40	106
    0x28300200,	//	17:50	107
    0x28300200,	//	18:00	108
    0x302E0121,	//	18:10	109
    0x382D0143,	//	18:20	110
    0x402C0064,	//	18:30	111
    0x482B0086,	//	18:40	112
    0x502A00A8,	//	18:50	113
    0x502A00A8,	//	19:00	114
    0x502A00A8,	//	19:10	115
    0x502A00A8,	//	19:20	116
    0x502A00A8,	//	19:30	117
    0x502A00A8,	//	19:40	118
    0x502A00A8,	//	19:50	119
    0x502A00A8,	//	20:00	120
    0x502A00A8,	//	20:10	121
    0x502A00A8,	//	20:20	122
    0x502A00A8,	//	20:30	123
    0x502A00A8,	//	20:40	124
    0x502A00A8,	//	20:50	125
    0x502A00A8,	//	21:00	126
    0x502A00A8,	//	21:10	127
    0x502A00A8,	//	21:20	128
    0x502A00A8,	//	21:30	129
    0x502A00A8,	//	21:40	130
    0x502A00A8,	//	21:50	131
    0x502A00A8,	//	22:00	132
    0x502A00A8,	//	22:10	133
    0x502A00A8,	//	22:20	134
    0x502A00A8,	//	22:30	135
    0x502A00A8,	//	22:40	136
    0x502A00A8,	//	22:50	137
    0x502A00A8,	//	23:00	138
    0x502A00A8,	//	23:10	139
    0x502A00A8,	//	23:20	140
    0x502A00A8,	//	23:30	141
    0x502A00A8,	//	23:40	142
    0x502A00A8, //	23:50	143
 };

inline u16 get_colorID(void)
{
    update_rtc();
    return (rtc_hex.hour * 60 + rtc_hex.minute) / 10;
}

static u32 TimeToColor(void)
{
    damage_loc = get_colorID(); //I will store the current ID there, so an overworld will know when it should reload the palette
    return TimeBlendColors[damage_loc];
}

static void apply_pal(const struct palette* pal, u16 ID, u16 size)
{
    if ((AFFECT_INDOORS == false && curr_mapheader.type == MAP_INSIDE)
        || (AFFECT_UNDERWATER == false && curr_mapheader.type == MAP_UNDERWATER))
        gpu_pal_apply(pal, ID, size);
    else //change pal depending on the day time
    {
        struct palette* changed_pal = (struct palette*) &decompression_buffer;
        u32 color = TimeToColor();

        for (u16 i = 0; i < (size / 16 + 1); i++)
        {
            for (u16 col = 0; col < 16; col++)
            {
                changed_pal[i].c[col] = blend(color, pal[i].c[col]);
            }
        }
        gpu_pal_apply(changed_pal, ID, size);
    }
}

void blockset_load_pal(const struct blockset* block, u16 ID, u16 size)
{
    if (!block) {return;}
    switch (block->pal_mode_flag)
    {
    case 0:
        {
            u32 zero = 0;
            gpu_pal_apply((void*) &zero, ID, 2); //black
            apply_pal(block->pals + 2, ID + 1, size - 2);
        }
        break;
    case 1:
        apply_pal(block->pals + 0xC0, ID, size);
        break;
    case 2: //compressed
        LZ77UnCompWram(block->pals, decompressed_pal);
        apply_pal(decompressed_pal, ID, size);
        break;
    }
}

void npc_pal_patch(u16 npcID, u16 ID)
{
    apply_pal(pal_npc_tile_table[npc_pal_idx_for_given_tag(npcID)].ptr, 0x100 + (ID * 0x10), 32);
}

void ov_consider_palchange(void)
{
    if (get_colorID() == damage_loc) {return;} //it's the same time
    blockset_load_pal(curr_mapheader.map_footer->primary_blockset, 0, 192);
    blockset_load_pal(curr_mapheader.map_footer->secondary_blockset, 96, 224);
    //patch NPCs
    for (u8 i = 0; i < 16; i++)
    {
        struct npc_state* NPC = &npc_states[i];
        if (NPC->localID == 0xFF && NPC->map_bank == 0xFF && NPC->map_no == 0xFF) {continue;} //not valid NPC
        struct npc_type* Type = get_npc_type_ptr(NPC->spriteID);
        u32 fieldC = ((Type->field_C) << 0x1C) >> 0x1C;
        if (fieldC == 0xA)
            npc_load_two_palettes__and_record(Type->pal_tag, fieldC);
        else if (fieldC > 15)
            npc_pal_patch(Type->pal_tag, (u8)fieldC - 16);
        else
            npc_pal_patch(Type->pal_tag, fieldC);
    }
}

u8 oec_alloc_pal(struct tile_pal* Tile)
{
    u8 freeID = 0xFF;
    if (gpu_pal_tags_index_of(Tile->tile_ID) == 0xFF) //tile pal was not loaded
    {
        freeID = gpu_pal_tags_index_of(0xFFFF);
        if (freeID != 0xFF)
        {
            gpu_pal_tags[freeID] = Tile->tile_ID;
            apply_pal(Tile->ptr, freeID * 16 + 0x100, 0x20);
        }
    }
    return freeID;
}

void oec_pal_alloc2(u32* ptr)
{
    struct tile_pal* Tile = (struct tile_pal*)(oe_read_word(ptr));
    oec_alloc_pal(Tile);
    sub_80ABE18(gpu_pal_tags_index_of(Tile->tile_ID));
    *ptr += 4;
}

void oec_alloc_apply_pal(u32* ptr)
{
    oec_alloc_pal((struct tile_pal*)(oe_read_word(ptr)));
    *ptr += 4;
}
