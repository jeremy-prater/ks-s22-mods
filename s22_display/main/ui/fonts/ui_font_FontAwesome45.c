/*******************************************************************************
 * Size: 45 px
 * Bpp: 2
 * Opts: --bpp 2 --size 45 --font /Users/jeremy.prater/src/ks-s22-mods/s22_display/main/ui/assets/fa-solid-900.ttf -o /Users/jeremy.prater/src/ks-s22-mods/s22_display/main/ui/assets/ui_font_FontAwesome45.c --format lvgl -r 0xf241 -r 0xf2c9 -r 0xf0e7 -r 0xf1e6 --no-compress --no-prefilter
 ******************************************************************************/

#include "../ui.h"

#ifndef UI_FONT_FONTAWESOME45
#define UI_FONT_FONTAWESOME45 1
#endif

#if UI_FONT_FONTAWESOME45

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+F0E7 "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x14, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0xfc,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3f,
    0xf8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7,
    0xff, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x7f, 0xff, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x7, 0xff, 0xfd, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xbf, 0xff, 0xf0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xb, 0xff, 0xff, 0x40, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xff, 0xff, 0xfc, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xf, 0xff, 0xff, 0xe0, 0x0, 0x0,
    0x0, 0x0, 0x1, 0xff, 0xff, 0xff, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x1f, 0xff, 0xff, 0xf8, 0x0,
    0x0, 0x0, 0x0, 0x1, 0xff, 0xff, 0xff, 0xd0,
    0x0, 0x0, 0x0, 0x0, 0x2f, 0xff, 0xff, 0xff,
    0x0, 0x0, 0x0, 0x0, 0x2, 0xff, 0xff, 0xff,
    0xf4, 0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xff,
    0xff, 0xc0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xff,
    0xff, 0xfe, 0x0, 0x0, 0x0, 0x0, 0x7f, 0xff,
    0xff, 0xff, 0xf0, 0x0, 0x0, 0x0, 0x7, 0xff,
    0xff, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x7f,
    0xff, 0xff, 0xff, 0xfd, 0x0, 0x0, 0x0, 0xb,
    0xff, 0xff, 0xff, 0xff, 0xfa, 0xaa, 0xaa, 0x80,
    0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x4f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xf0, 0x2f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x40, 0x0, 0x0, 0x3, 0xff,
    0xff, 0xff, 0xff, 0xf0, 0x0, 0x0, 0x0, 0xf,
    0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0,
    0xbf, 0xff, 0xff, 0xff, 0xf0, 0x0, 0x0, 0x0,
    0x3, 0xff, 0xff, 0xff, 0xfe, 0x0, 0x0, 0x0,
    0x0, 0x1f, 0xff, 0xff, 0xff, 0xe0, 0x0, 0x0,
    0x0, 0x0, 0xff, 0xff, 0xff, 0xfd, 0x0, 0x0,
    0x0, 0x0, 0x7, 0xff, 0xff, 0xff, 0xd0, 0x0,
    0x0, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xfc, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0xff, 0xc0,
    0x0, 0x0, 0x0, 0x0, 0xb, 0xff, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xff,
    0x80, 0x0, 0x0, 0x0, 0x0, 0x2, 0xff, 0xff,
    0xf8, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0xff,
    0xff, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7f,
    0xff, 0xf4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3,
    0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xf, 0xff, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x7f, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x1, 0xff, 0xe0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x3, 0xfe, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x1, 0x90, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,

    /* U+F1E6 "" */
    0x0, 0x0, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x7f, 0x40, 0x0, 0x0, 0x2f, 0xd0,
    0x0, 0x0, 0xf, 0xfc, 0x0, 0x0, 0x7, 0xff,
    0x0, 0x0, 0x1, 0xff, 0xc0, 0x0, 0x0, 0x7f,
    0xf0, 0x0, 0x0, 0x1f, 0xfc, 0x0, 0x0, 0x7,
    0xff, 0x0, 0x0, 0x1, 0xff, 0xc0, 0x0, 0x0,
    0x7f, 0xf0, 0x0, 0x0, 0x1f, 0xfc, 0x0, 0x0,
    0x7, 0xff, 0x0, 0x0, 0x1, 0xff, 0xc0, 0x0,
    0x0, 0x7f, 0xf0, 0x0, 0x0, 0x1f, 0xfc, 0x0,
    0x0, 0x7, 0xff, 0x0, 0x0, 0x1, 0xff, 0xc0,
    0x0, 0x0, 0x7f, 0xf0, 0x0, 0x0, 0x1f, 0xfc,
    0x0, 0x0, 0x7, 0xff, 0x0, 0x0, 0x1, 0xff,
    0xc0, 0x0, 0x0, 0x7f, 0xf0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x6,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa9, 0x7,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xfe, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xd2, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xf8, 0x3, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfc, 0x0, 0x3f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xc0, 0x3, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xfc, 0x0, 0x3f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xc0, 0x3, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xfc, 0x0, 0x3f, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x80, 0x2, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xf4, 0x0, 0x1f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x0, 0xb,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0x0, 0x0,
    0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x0,
    0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0,
    0x0, 0x7, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x0,
    0x0, 0x0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0x0,
    0x0, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xff, 0x80,
    0x0, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xd0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x2f, 0xff, 0x40,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xe0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0xfe,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff,
    0xe0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf,
    0xfe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xff, 0xe0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xf, 0xfe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xff, 0xe0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x3, 0xfc, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x5, 0x0, 0x0, 0x0, 0x0,

    /* U+F241 "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf4,
    0x0, 0x0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfc, 0x0, 0x7, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfc, 0x0, 0x3f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x2,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf4, 0xf, 0xff, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xab, 0xff,
    0xe0, 0x3f, 0xfc, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x3, 0xff, 0x80, 0xff, 0xe0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xf, 0xff, 0x3, 0xff, 0x80, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x50, 0x0, 0x0, 0x3f, 0xfd, 0xf,
    0xfe, 0xb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0,
    0x0, 0x0, 0xff, 0xff, 0x3f, 0xf8, 0x2f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0x3, 0xff,
    0xfd, 0xff, 0xe0, 0xbf, 0xff, 0xff, 0xff, 0xff,
    0xfc, 0x0, 0x0, 0xf, 0xff, 0xfb, 0xff, 0x82,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0, 0x0,
    0x3f, 0xff, 0xef, 0xfe, 0xb, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xc0, 0x0, 0x0, 0xff, 0xff, 0xbf,
    0xf8, 0x2f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0,
    0x0, 0x3, 0xff, 0xfe, 0xff, 0xe0, 0xbf, 0xff,
    0xff, 0xff, 0xff, 0xfc, 0x0, 0x0, 0xf, 0xff,
    0xfb, 0xff, 0x82, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xf0, 0x0, 0x0, 0x3f, 0xff, 0xef, 0xfe, 0xb,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x0, 0x0,
    0xff, 0xff, 0xbf, 0xf8, 0x2f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x0, 0x0, 0x3, 0xff, 0xfc, 0xff,
    0xe0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x0,
    0x0, 0xf, 0xff, 0x93, 0xff, 0x80, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0xfc,
    0xf, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0xff, 0xf0, 0x3f, 0xfe, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaf,
    0xff, 0x80, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0x0, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xf0, 0x1, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x40,
    0x1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf4, 0x0, 0x0, 0xbf, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

    /* U+F2C9 "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x2, 0xff, 0xf8, 0x0, 0x0, 0x0, 0x1, 0xff,
    0xff, 0xf8, 0x0, 0x0, 0x0, 0xbf, 0xff, 0xff,
    0xe0, 0x0, 0x0, 0x2f, 0xff, 0xff, 0xff, 0x80,
    0x0, 0x7, 0xff, 0xff, 0xff, 0xfd, 0x0, 0x0,
    0xff, 0xfe, 0x5b, 0xff, 0xf0, 0x0, 0x1f, 0xff,
    0x40, 0xf, 0xff, 0x40, 0x2, 0xff, 0xd0, 0x0,
    0x7f, 0xf8, 0x0, 0x2f, 0xfc, 0x0, 0x3, 0xff,
    0xc0, 0x3, 0xff, 0xc0, 0x0, 0x2f, 0xfc, 0x0,
    0x3f, 0xfc, 0x0, 0x2, 0xff, 0xc0, 0x3, 0xff,
    0xc0, 0x0, 0x2f, 0xfc, 0x0, 0x3f, 0xfc, 0x0,
    0x2, 0xff, 0xc0, 0x3, 0xff, 0xc0, 0x0, 0x2f,
    0xfc, 0x0, 0x3f, 0xfc, 0x0, 0x2, 0xff, 0xc0,
    0x3, 0xff, 0xc0, 0x0, 0x2f, 0xfc, 0x0, 0x3f,
    0xfc, 0x5, 0x2, 0xff, 0xc0, 0x3, 0xff, 0xc0,
    0xf4, 0x2f, 0xfc, 0x0, 0x3f, 0xfc, 0x1f, 0x42,
    0xff, 0xc0, 0x3, 0xff, 0xc1, 0xf4, 0x2f, 0xfc,
    0x0, 0x3f, 0xfc, 0x1f, 0x42, 0xff, 0xc0, 0x3,
    0xff, 0xc1, 0xf4, 0x2f, 0xfc, 0x0, 0x3f, 0xfc,
    0x1f, 0x42, 0xff, 0xc0, 0x3, 0xff, 0xc1, 0xf4,
    0x2f, 0xfc, 0x0, 0x3f, 0xfc, 0x1f, 0x42, 0xff,
    0xd0, 0xf, 0xff, 0x81, 0xf4, 0x1f, 0xff, 0x1,
    0xff, 0xf4, 0x1f, 0x40, 0xff, 0xf4, 0x3f, 0xfe,
    0x1, 0xf4, 0x7, 0xff, 0xc3, 0xff, 0xc0, 0x7f,
    0xd0, 0x3f, 0xfc, 0x7f, 0xf8, 0x1f, 0xff, 0x41,
    0xff, 0xd7, 0xff, 0x3, 0xff, 0xfc, 0xf, 0xfe,
    0xbf, 0xf0, 0x3f, 0xff, 0xc0, 0xff, 0xeb, 0xff,
    0x3, 0xff, 0xfc, 0xf, 0xfe, 0x7f, 0xf0, 0x3f,
    0xff, 0xc0, 0xff, 0xe7, 0xff, 0x80, 0xff, 0xf4,
    0x1f, 0xfd, 0x3f, 0xfc, 0x3, 0xfd, 0x3, 0xff,
    0xc2, 0xff, 0xe0, 0x0, 0x0, 0xbf, 0xfc, 0x1f,
    0xff, 0x80, 0x0, 0x2f, 0xff, 0x40, 0xbf, 0xff,
    0x80, 0x1f, 0xff, 0xf0, 0x3, 0xff, 0xff, 0xff,
    0xff, 0xfc, 0x0, 0x1f, 0xff, 0xff, 0xff, 0xff,
    0x40, 0x0, 0x7f, 0xff, 0xff, 0xff, 0xd0, 0x0,
    0x0, 0xbf, 0xff, 0xff, 0xf0, 0x0, 0x0, 0x1,
    0xbf, 0xff, 0xf4, 0x0, 0x0, 0x0, 0x0, 0x5a,
    0x50, 0x0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 630, .box_w = 35, .box_h = 47, .ofs_x = 2, .ofs_y = -7},
    {.bitmap_index = 412, .adv_w = 540, .box_w = 34, .box_h = 46, .ofs_x = 0, .ofs_y = -6},
    {.bitmap_index = 803, .adv_w = 810, .box_w = 51, .box_h = 30, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 1186, .adv_w = 450, .box_w = 26, .box_h = 46, .ofs_x = 1, .ofs_y = -6}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0xff, 0x15a, 0x1e2
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 61671, .range_length = 483, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 4, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 2,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t ui_font_FontAwesome45 = {
#else
lv_font_t ui_font_FontAwesome45 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 47,          /*The maximum line height required by the font*/
    .base_line = 7,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -3,
    .underline_thickness = 2,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if UI_FONT_FONTAWESOME45*/

