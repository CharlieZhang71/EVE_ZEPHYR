/**
 * This source code ("the Software") is provided by Bridgetek Pte Ltd
 * ("Bridgetek") subject to the licence terms set out
 *   http://brtchip.com/BRTSourceCodeLicenseAgreement/ ("the Licence Terms").
 * You must read the Licence Terms before downloading or using the Software.
 * By installing or using the Software you agree to the Licence Terms. If you
 * do not agree to the Licence Terms then do not download or use the Software.
 *
 * Without prejudice to the Licence Terms, here is a summary of some of the key
 * terms of the Licence Terms (and in the event of any conflict between this
 * summary and the Licence Terms then the text of the Licence Terms will
 * prevail).
 *
 * The Software is provided "as is".
 * There are no warranties (or similar) in relation to the quality of the
 * Software. You use it at your own risk.
 * The Software should not be used in, or for, any medical device, system or
 * appliance. There are exclusions of Bridgetek liability for certain types of loss
 * such as: special loss or damage; incidental loss or damage; indirect or
 * consequential loss or damage; loss of income; loss of business; loss of
 * profits; loss of revenue; loss of contracts; business interruption; loss of
 * the use of money or anticipated savings; loss of information; loss of
 * opportunity; loss of goodwill or reputation; and/or loss of, damage to or
 * corruption of data.
 * There is a monetary cap on Bridgetek's liability.
 * The Software may have subsequently been amended by another user and then
 * distributed by that other user ("Adapted Software").  If so that user may
 * have additional licence terms that apply to those amendments. However, Bridgetek
 * has no liability in relation to those amendments.
 */

#include "EVE_Util.h"
#include "EVE_Platform.h"
#include "EVE_HalImpl.h"

// BT820_DEBUG
#define TEST_BOOT_DDR 0

#if defined(EVE_GRAPHICS_IDP_3500_04A)
#ifndef ESD_SIMULATION
#include "EVE_KD2401.h"
#endif
#endif

EVE_HAL_EXPORT void EVE_CoDlImpl_resetCoState(EVE_HalContext *phost);

#if (EVE_SUPPORT_CHIPID < EVE_FT810) || defined(EVE_MULTI_GRAPHICS_TARGET)
static eve_progmem_const uint8_t c_DlCodeBootupFT80X[3 * 4] = {
	0, 0, 0, 2, // GPU instruction CLEAR_COLOR_RGB
	7, 0, 0, 0x26, // GPU instruction CLEAR
	0, 0, 0, 0, // GPU instruction DISPLAY
};
#endif

#if EVE_SUPPORT_CHIPID >= EVE_FT810
static eve_progmem_const uint8_t c_DlCodeBootup[3 * 4 + 16 * 3 * 4] = {
	0, 0, 0, 2, // GPU instruction CLEAR_COLOR_RGB
	7, 0, 0, 0x26, // GPU instruction CLEAR

	0, 0, 0, 5, // Bitmap_handle(0)
	0, 0, 0, 0x28, // Bitmap_layout_H(0,0)
	0, 0, 0, 0x29, // Bitmap_Size_H(0,0)
	1, 0, 0, 5, // Bitmap_handle(1)
	0, 0, 0, 0x28, // Bitmap_layout_H(0,0)
	0, 0, 0, 0x29, // Bitmap_Size_H(0,0)
	2, 0, 0, 5, // Bitmap_handle(2)
	0, 0, 0, 0x28, // Bitmap_layout_H(0,0)
	0, 0, 0, 0x29, // Bitmap_Size_H(0,0)
	3, 0, 0, 5, // Bitmap_handle(3)
	0, 0, 0, 0x28, // Bitmap_layout_H(0,0)
	0, 0, 0, 0x29, // Bitmap_Size_H(0,0)
	4, 0, 0, 5, // Bitmap_handle(4)
	0, 0, 0, 0x28, // Bitmap_layout_H(0,0)
	0, 0, 0, 0x29, // Bitmap_Size_H(0,0)
	5, 0, 0, 5, // Bitmap_handle(5)
	0, 0, 0, 0x28, // Bitmap_layout_H(0,0)
	0, 0, 0, 0x29, // Bitmap_Size_H(0,0)
	6, 0, 0, 5, // Bitmap_handle(6)
	0, 0, 0, 0x28, // Bitmap_layout_H(0,0)
	0, 0, 0, 0x29, // Bitmap_Size_H(0,0)
	7, 0, 0, 5, // Bitmap_handle(7)
	0, 0, 0, 0x28, // Bitmap_layout_H(0,0)
	0, 0, 0, 0x29, // Bitmap_Size_H(0,0)
	8, 0, 0, 5, // Bitmap_handle(8)
	0, 0, 0, 0x28, // Bitmap_layout_H(0,0)
	0, 0, 0, 0x29, // Bitmap_Size_H(0,0)
	9, 0, 0, 5, // Bitmap_handle(9)
	0, 0, 0, 0x28, // Bitmap_layout_H(0,0)
	0, 0, 0, 0x29, // Bitmap_Size_H(0,0)
	10, 0, 0, 5, // Bitmap_handle(10)
	0, 0, 0, 0x28, // Bitmap_layout_H(0,0)
	0, 0, 0, 0x29, // Bitmap_Size_H(0,0)
	11, 0, 0, 5, // Bitmap_handle(11)
	0, 0, 0, 0x28, // Bitmap_layout_H(0,0)
	0, 0, 0, 0x29, // Bitmap_Size_H(0,0)
	12, 0, 0, 5, // Bitmap_handle(12)
	0, 0, 0, 0x28, // Bitmap_layout_H(0,0)
	0, 0, 0, 0x29, // Bitmap_Size_H(0,0)
	13, 0, 0, 5, // Bitmap_handle(13)
	0, 0, 0, 0x28, // Bitmap_layout_H(0,0)
	0, 0, 0, 0x29, // Bitmap_Size_H(0,0)
	14, 0, 0, 5, // Bitmap_handle(14)
	0, 0, 0, 0x28, // Bitmap_layout_H(0,0)
	0, 0, 0, 0x29, // Bitmap_Size_H(0,0)
	15, 0, 0, 5, // Bitmap_handle(15)
	0, 0, 0, 0x28, // Bitmap_layout_H(0,0)
	0, 0, 0, 0x29, // Bitmap_Size_H(0,0)

	0, 0, 0, 0, // GPU instruction DISPLAY
};
#endif

/* NOTE: Update `EVE_DISPLAY_T` in `EVE_Util.h` when adding display presets. */
static const uint16_t s_DisplayResolutions[EVE_DISPLAY_NB][4] = {
	/* width, height, refreshRate, hsfWidth */

	{ 480, 272, 60, 0 }, /* EVE_DISPLAY_DEFAULT (values ignored) */

	/* Landscape */
	{ 320, 240, 56, 0 }, /* EVE_DISPLAY_QVGA_320x240_56Hz */
	{ 480, 272, 60, 0 }, /* EVE_DISPLAY_WQVGA_480x272_60Hz */
	{ 800, 480, 74, 0 }, /* EVE_DISPLAY_WVGA_800x480_74Hz */
	{ 1024, 600, 59, 0 }, /* EVE_DISPLAY_WSVGA_1024x600_59Hz */
	{ 1280, 720, 58, 0 }, /* EVE_DISPLAY_HDTV_1280x720_58Hz */
	{ 1280, 800, 57, 0 }, /* EVE_DISPLAY_WXGA_1280x800_57Hz */
	{ 1920, 1080, 60, 0 }, /* EVE_DISPLAY_FHD_1920x1080_60Hz */
	{ 1920, 1200, 90, 0 }, /* EVE_DISPLAY_WUXGA_1920x1200_90Hz */
	{ 1920, 1200, 111, 0 }, /* EVE_DISPLAY_WUXGA_1920x1200_111Hz */

	/* Portrait */
	{ 320, 480, 60, 0 }, /* EVE_DISPLAY_HVGA_320x480_60Hz */

	/* IDM2040-7A, AT‐T070SWH‐51CP, HX8264-E, 60 to 90Hz capable */
	{ 860, 480, 60, 800 }, /* EVE_DISPLAY_WVGA_800x480_W860_60Hz */

	/* IDM2040-21R(FT800), */
	{ 480, 480, 48, 0 }, /* EVE_DISPLAY_WQVGA_480x480_48Hz */

	/* Riverdi */
	{ 320, 240, 62, 0 }, /* EVE_DISPLAY_RIVERDI_IPS35_62Hz */
	{ 480, 272, 58, 0 }, /* EVE_DISPLAY_RIVERDI_IPS43_58Hz */
	{ 800, 480, 63, 0 }, /* EVE_DISPLAY_RIVERDI_IPS50_63Hz */
	{ 1024, 600, 59, 0 }, /* EVE_DISPLAY_RIVERDI_IPS70_59Hz */
	{ 1280, 800, 59, 0 }, /* EVE_DISPLAY_RIVERDI_IPS101_59Hz */

};

/** VERIFY: Can the emulator handle this? */
#if (!defined(BT8XXEMU_PLATFORM) || defined(EVE_MULTI_PLATFORM_TARGET)) \
    && EVE_SUPPORT_CHIPID_SPECIFIC(EVE_FT811) || EVE_SUPPORT_CHIPID_SPECIFIC(EVE_FT813)
#define TOUCH_DATA_81X_LEN 1172
static eve_progmem_const uint8_t c_TouchData81xU8[TOUCH_DATA_81X_LEN] = {
	26, 255, 255, 255, 32, 32, 48, 0, 4, 0, 0, 0, 2, 0, 0, 0, 34,
	255, 255, 255, 0, 176, 48, 0, 120, 218, 237, 84, 255, 107, 92,
	69, 16, 159, 125, 155, 107, 141, 201, 121, 247, 106, 130, 9,
	225, 244, 238, 37, 246, 146, 52, 63, 53, 98, 172, 53, 48, 243,
	8, 36, 166, 182, 63, 136, 216, 246, 7, 205, 219, 75, 122, 119,
	185, 139, 196, 128, 34, 33, 136, 123, 65, 240, 7, 31, 44, 105,
	41, 69, 72, 12, 210, 166, 6, 17, 12, 104, 64, 165, 73, 133, 134,
	98, 80, 74, 43, 148, 22, 20, 133, 40, 20, 34, 233, 15, 82, 250,
	131, 10, 113, 246, 229, 106, 197, 191, 161, 111, 217, 55, 179,
	59, 59, 59, 243, 153, 47, 251, 135, 15, 58, 204, 11, 109, 114,
	89, 149, 84, 169, 242, 172, 2, 109, 10, 73, 53, 167, 92, 158,
	89, 21, 6, 89, 230, 236, 126, 86, 89, 206, 242, 169, 178, 61,
	187, 115, 62, 85, 190, 199, 133, 129, 44, 132, 249, 90, 12, 243,
	124, 67, 49, 169, 76, 81, 22, 195, 60, 211, 113, 40, 133, 249,
	253, 104, 114, 113, 148, 197, 131, 105, 169, 158, 3, 19, 132,
	65, 28, 187, 240, 25, 112, 3, 147, 99, 235, 163, 80, 98, 63, 10,
	123, 181, 208, 73, 213, 29, 24, 246, 226, 121, 112, 199, 146,
	74, 98, 56, 22, 195, 135, 245, 32, 14, 194, 17, 180, 182, 100,
	46, 169, 220, 114, 244, 103, 171, 114, 252, 41, 222, 143, 97,
	150, 53, 64, 167, 202, 110, 240, 86, 186, 45, 16, 90, 14, 191,
	158, 110, 196, 163, 216, 130, 241, 232, 159, 42, 251, 88, 4,
	169, 90, 80, 42, 163, 226, 248, 168, 206, 97, 132, 136, 169, 69,
	113, 20, 27, 245, 203, 40, 131, 12, 54, 87, 37, 143, 253, 43,
	105, 97, 137, 9, 172, 103, 181, 26, 172, 79, 163, 102, 184, 142,
	57, 83, 0, 61, 139, 144, 159, 79, 135, 193, 62, 61, 0, 157, 250,
	5, 20, 186, 67, 91, 255, 133, 238, 140, 104, 167, 30, 192, 197,
	72, 2, 85, 137, 221, 89, 134, 101, 140, 177, 191, 238, 137, 75,
	105, 137, 23, 97, 18, 226, 31, 175, 98, 31, 174, 165, 141, 90,
	193, 21, 88, 194, 117, 60, 207, 243, 115, 248, 202, 254, 171,
	171, 37, 248, 20, 87, 112, 17, 59, 180, 59, 246, 35, 199, 177,
	83, 175, 35, 140, 93, 224, 187, 172, 45, 169, 172, 108, 29, 47,
	192, 59, 186, 70, 159, 1, 208, 155, 136, 40, 71, 190, 195, 101,
	222, 117, 131, 221, 90, 142, 110, 48, 221, 85, 165, 177, 42,
	173, 169, 82, 89, 165, 78, 149, 138, 136, 110, 224, 13, 108,
	200, 0, 132, 1, 50, 103, 114, 55, 176, 145, 254, 51, 132, 208,
	183, 177, 137, 64, 111, 128, 192, 13, 104, 162, 86, 145, 165,
	198, 255, 141, 86, 209, 68, 79, 146, 84, 87, 33, 163, 95, 132,
	39, 120, 182, 242, 76, 115, 78, 123, 40, 142, 7, 200, 227, 21,
	103, 50, 103, 171, 109, 123, 27, 242, 115, 193, 0, 197, 176, 70,
	63, 141, 125, 212, 35, 14, 8, 95, 28, 20, 246, 116, 155, 30,
	132, 152, 118, 3, 40, 29, 162, 62, 60, 158, 9, 131, 8, 197, 120,
	49, 99, 115, 35, 39, 228, 200, 168, 48, 81, 197, 2, 175, 221,
	224, 205, 140, 173, 148, 4, 87, 13, 148, 94, 19, 235, 216, 205,
	62, 212, 179, 14, 199, 221, 234, 21, 223, 207, 112, 254, 163,
	90, 169, 197, 247, 50, 110, 249, 16, 249, 156, 113, 137, 83, 98,
	146, 163, 23, 215, 115, 16, 146, 173, 102, 142, 158, 74, 160,
	205, 181, 28, 217, 67, 93, 156, 121, 40, 156, 202, 76, 130, 169,
	184, 193, 12, 9, 125, 82, 132, 149, 121, 209, 65, 219, 198, 12,
	47, 80, 7, 15, 182, 163, 97, 52, 171, 206, 211, 20, 186, 229,
	195, 148, 42, 15, 137, 45, 116, 244, 29, 188, 138, 105, 177, 76,
	139, 149, 18, 113, 141, 160, 192, 1, 218, 222, 14, 131, 101, 90,
	224, 17, 195, 239, 51, 113, 156, 194, 111, 245, 21, 49, 137,
	171, 100, 61, 238, 21, 166, 50, 67, 95, 83, 63, 154, 202, 71,
	116, 141, 45, 205, 208, 37, 94, 133, 188, 186, 201, 177, 134,
	82, 86, 117, 84, 109, 46, 137, 52, 237, 214, 102, 248, 22, 253,
	82, 41, 137, 91, 148, 37, 29, 97, 147, 195, 86, 126, 89, 156,
	165, 45, 214, 188, 43, 54, 43, 160, 63, 36, 139, 169, 142, 17,
	73, 175, 31, 255, 230, 232, 164, 175, 244, 161, 197, 38, 71,
	118, 121, 70, 109, 226, 43, 252, 14, 100, 249, 142, 135, 180,
	156, 48, 185, 45, 172, 176, 247, 71, 244, 29, 104, 240, 25, 3,
	37, 216, 78, 179, 159, 101, 171, 9, 63, 225, 95, 166, 6, 126,
	71, 24, 29, 156, 165, 199, 253, 126, 108, 119, 88, 51, 146, 216,
	19, 191, 137, 102, 191, 107, 186, 68, 109, 78, 247, 244, 95,
	130, 99, 94, 134, 49, 183, 128, 158, 84, 207, 58, 245, 129, 9,
	220, 145, 78, 16, 218, 85, 102, 226, 94, 102, 142, 121, 247, 51,
	243, 146, 119, 63, 51, 3, 14, 71, 157, 94, 245, 86, 169, 151,
	134, 156, 99, 212, 206, 189, 148, 208, 9, 109, 239, 43, 242,
	125, 57, 135, 249, 19, 111, 120, 245, 182, 79, 121, 218, 62,
	110, 231, 222, 203, 57, 220, 245, 52, 237, 217, 126, 246, 253,
	183, 89, 95, 59, 59, 145, 238, 165, 15, 28, 91, 45, 86, 239,
	140, 215, 231, 199, 56, 34, 59, 239, 154, 239, 159, 244, 38, 57,
	63, 167, 156, 41, 126, 95, 14, 83, 82, 189, 203, 183, 244, 51,
	186, 33, 39, 12, 234, 244, 105, 156, 79, 111, 161, 100, 186,
	232, 37, 25, 187, 195, 253, 252, 137, 35, 245, 2, 243, 54, 207,
	243, 8, 176, 143, 109, 175, 225, 57, 92, 131, 71, 248, 220, 151,
	222, 113, 255, 28, 26, 114, 131, 139, 17, 130, 111, 24, 65, 63,
	235, 157, 198, 90, 188, 238, 213, 251, 63, 120, 130, 59, 151,
	187, 130, 95, 193, 207, 252, 26, 214, 221, 175, 215, 240, 186,
	211, 85, 217, 197, 107, 67, 123, 113, 15, 217, 42, 252, 201,
	155, 245, 133, 94, 131, 122, 255, 87, 207, 80, 143, 215, 238,
	91, 47, 126, 247, 106, 248, 204, 159, 153, 187, 116, 219, 49,
	116, 45, 115, 147, 182, 48, 242, 4, 190, 240, 127, 118, 224,
	193, 247, 224, 3, 89, 247, 15, 148, 99, 211, 103, 26, 255, 255,
	255, 20, 33, 48, 0, 4, 0, 0, 0, 15, 0, 0, 0, 26, 255, 255, 255,
	32, 32, 48, 0, 4, 0, 0, 0, 0, 0, 0, 0
};
#endif

#if (!defined(BT8XXEMU_PLATFORM) || defined(EVE_MULTI_PLATFORM_TARGET)) \
    && EVE_SUPPORT_CHIPID_SPECIFIC(EVE_FT800)
#define TOUCH_DATA_80X_LEN 1620
static uint8_t c_TouchData80xU8[TOUCH_DATA_80X_LEN] = {
26,255,255,255,84,37,16,0,4,0,0,0,1,0,0,0,26,255,255,255,224,36,16,0,4,0,0,0,1,0,0,0,26,255,255,255,0,192,28,0,8,6,0,0,201,66,0,128,140,102,1,128,143,98,2,128,143,98,39,97,16,97,29,107,156,97,16,97,39,97,16,97,29,107,140,96,0,128,143,103,16,97,157,97,17,97,157,97,3,97,143,97,39,97,39,97,20,64,29,107,29,107,7,64,39,97,7,64,29,107,140,96,29,64,22,0,39,97,140,96,39,97,17,97,29,107,156,97,3,103,140,102,16,64,140,102,0,128,143,104,46,64,140,102,0,128,16,97,143,104,3,104,140,102,51,64,140,102,16,97,143,111,0,106,140,102,59,64,143,98,17,96,46,64,67,32,59,0,140,96,20,64,3,104,73,32,3,97,74,0,143,96,140,96,20,64,51,64,70,0,17,96,143,98,1,128,143,105,17,96,0,108,16,97,3,64,140,108,10,128,51,96,143,97,7,64,87,64,51,96,143,97,17,96,46,0,1,128,0,128,7,64,3,98,39,97,17,97,3,98,17,96,7,64,3,111,110,32,29,107,3,64,111,0,29,107,140,96,0,102,16,97,0,102,16,97,96,0,17,96,46,64,121,32,112,0,140,96,39,97,0,128,0,128,29,107,17,96,141,32,36,96,1,128,3,99,134,32,24,64,98,64,29,64,20,64,98,64,29,64,29,107,80,64,126,0,3,97,34,0,1,128,16,97,51,96,143,97,0,128,16,97,51,96,143,97,0,128,143,64,0,96,0,96,0,96,0,96,0,96,0,128,147,64,0,96,0,96,0,96,140,96,0,128,17,101,169,32,3,64,165,0,22,0,1,128,143,64,9,128,0,108,3,128,3,99,3,128,3,98,151,64,0,106,0,128,0,103,178,32,3,97,1,128,147,64,151,64,151,64,151,64,151,64,151,64,151,64,151,64,151,64,151,64,151,64,3,128,140,108,20,64,19,128,82,64,7,64,122,64,39,97,39,97,17,128,82,64,7,64,122,64,29,107,29,107,98,64,21,128,82,64,98,64,3,96,11,64,25,128,82,64,7,64,122,64,39,97,39,97,23,128,82,64,7,64,122,64,29,107,29,107,98,64,27,128,82,64,98,64,143,96,120,131,4,128,51,96,143,97,186,131,4,128,51,96,143,97,184,131,4,128,51,96,143,97,189,131,4,128,51,96,143,97,188,131,4,128,51,96,143,97,185,131,4,128,51,96,143,97,184,131,4,128,51,96,143,97,248,131,4,128,51,96,143,97,4,128,0,106,17,96,16,64,11,33,143,97,10,65,10,1,144,129,16,65,16,65,0,106,17,96,16,64,19,33,3,97,0,106,17,96,16,64,18,33,143,97,2,65,16,65,250,64,16,65,246,0,254,64,16,65,2,65,16,65,250,64,16,65,246,64,16,1,254,64,16,65,246,64,16,65,250,64,16,65,2,1,170,64,9,128,143,105,254,64,16,65,6,65,51,65,57,33,16,65,2,65,51,65,254,64,16,1,72,33,254,64,10,65,2,65,16,65,254,64,10,65,78,1,246,64,10,65,250,64,16,65,246,64,10,1,140,96,17,96,128,128,3,99,64,65,78,0,254,64,10,65,79,65,79,65,79,65,79,65,79,65,79,65,79,65,79,65,3,97,54,1,78,64,54,65,143,98,0,128,96,65,96,65,96,65,96,65,96,65,96,65,96,65,96,65,16,97,64,1,113,33,0,128,36,0,140,96,44,129,18,65,0,228,31,128,51,96,3,97,44,129,18,65,17,96,31,128,51,96,3,97,114,1,140,96,0,128,99,1,128,65,8,128,3,113,128,65,143,100,255,255,0,102,157,96,135,65,20,64,20,64,20,64,20,0,31,128,51,96,3,97,35,128,51,96,3,97,34,128,90,64,33,128,90,64,30,128,90,64,32,128,90,0,31,65,112,128,84,65,110,65,84,65,110,1,1,64,99,65,3,97,44,1,36,96,0,128,3,103,174,33,32,128,90,64,207,1,17,107,1,128,3,103,181,33,30,128,90,64,207,1,17,107,2,128,3,103,188,33,33,128,90,64,207,1,17,107,3,128,3,103,195,33,34,128,90,64,207,1,17,107,4,128,3,103,206,33,31,128,51,96,3,97,35,128,51,96,3,97,207,1,22,64,12,96,140,96,17,96,5,128,3,111,218,33,1,128,16,97,3,113,3,100,219,1,143,97,140,96,39,97,17,96,229,33,16,97,17,107,36,64,16,97,0,106,221,1,3,97,12,96,163,65,17,96,0,128,17,96,5,128,3,104,251,33,39,97,17,96,1,128,3,99,16,64,247,33,135,65,17,107,167,65,80,64,29,107,3,64,234,1,22,0,130,65,255,143,3,99,130,65,17,96,255,143,3,99,16,97,12,128,3,105,15,128,3,99,39,97,130,65,3,97,198,64,17,107,167,65,29,107,209,1,1,128,157,65,44,65,31,65,113,128,84,65,110,65,128,65,3,97,0,128,128,65,252,129,220,65,143,97,1,64,1,64,255,255,0,102,17,96,255,255,140,96,31,128,51,96,3,97,32,128,90,64,20,64,30,128,90,64,33,128,90,0,2,128,157,65,44,65,31,65,113,128,84,65,110,65,128,65,66,34,130,65,255,143,3,99,130,65,255,143,3,99,20,64,198,64,200,128,67,2,30,66,163,65,37,2,1,128,37,128,51,96,143,97,234,64,16,1,11,128,0,108,0,128,3,103,143,100,73,66,17,96,36,64,75,66,110,65,51,65,16,64,81,34,143,97,73,66,17,96,36,64,75,66,110,65,51,65,90,34,143,97,13,128,0,108,0,128,143,103,97,66,140,102,101,130,80,66,16,66,69,66,101,130,89,66,103,2,140,96,0,128,39,97,97,130,80,66,47,66,69,66,97,130,89,66,113,2,140,96,31,65,232,128,84,65,110,65,84,65,110,1,128,65,0,106,39,97,130,65,130,65,130,65,3,97,198,64,17,107,167,65,29,107,209,1,31,65,233,128,84,65,110,65,0,128,128,65,7,128,3,99,127,130,220,1,31,65,233,128,84,65,110,65,128,65,7,128,3,99,169,34,128,65,128,128,3,99,163,34,30,66,168,2,130,65,130,65,20,64,198,64,200,128,170,2,30,66,163,65,37,2,97,130,80,66,97,130,89,66,149,66,69,66,172,2,140,96,0,128,101,130,80,66,101,130,89,66,139,66,18,64,3,100,190,34,69,66,181,2,140,96,73,66,0,128,3,64,32,206,3,103,51,65,3,100,194,34,143,97,242,64,11,128,0,108,0,128,42,64,202,34,101,66,17,96,213,34,30,66,37,66,215,2,138,65,143,65,69,66,238,64,9,128,0,108,4,128,3,99,16,64,228,34,5,128,18,65,242,64,44,129,18,65,101,66,42,64,110,65,1,128,18,65,31,65,112,128,84,65,44,65,16,64,244,34,101,66,243,34,111,66,244,2,103,66,192,66,31,65,232,128,84,65,44,65,16,64,0,35,101,66,255,34,172,66,0,3,180,66,10,128,18,65,1,64,110,1,26,255,255,255,84,37,16,0,4,0,0,0,0,0,0,0,26,255,255,255,28,36,16,0,4,0,0,0,0,0,0,0
};
#endif

#if (!defined(BT8XXEMU_PLATFORM) || defined(EVE_MULTI_PLATFORM_TARGET)) \
    && (EVE_SUPPORT_CHIPID_SPECIFIC(EVE_FT800) || EVE_SUPPORT_CHIPID_SPECIFIC(EVE_FT811) || EVE_SUPPORT_CHIPID_SPECIFIC(EVE_FT813))
/**
 * @brief Upload touch firmware for FT800, FT811 and FT813 chip
 *
 * @param phost  Pointer to Hal context
 */
static inline void uploadTouchFirmware(EVE_HalContext *phost)
{
#if EVE_SUPPORT_CHIPID_SPECIFIC(EVE_FT800)
	if (EVE_CHIPID == EVE_FT800)
	{
		eve_assert_do(EVE_Cmd_wrProgMem(phost, c_TouchData80xU8, TOUCH_DATA_80X_LEN));
		eve_assert_do(EVE_Cmd_waitFlush(phost));
	}
	else
#endif
	{
#if EVE_SUPPORT_CHIPID_SPECIFIC(EVE_FT811) || EVE_SUPPORT_CHIPID_SPECIFIC(EVE_FT813)
		eve_assert_do(EVE_Cmd_wrProgMem(phost, c_TouchData81xU8, TOUCH_DATA_81X_LEN));
		eve_assert_do(EVE_Cmd_waitFlush(phost));
#endif
	}
}
#else
static inline void uploadTouchFirmware(EVE_HalContext *phost)
{
	/* no-op */
}
#endif

#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
#define patchLen 1732
uint8_t plot_patch_data [patchLen] = {
	0x7c, 0xda, 0x0, 0x50, 0x0, 0x20, 0xbc, 0x6, 0x78, 0x9c, 0xed, 0x56, 0x7b, 0x6c, 0x14, 0x45, 0x18, 0x9f, 0xee, 0xdc, 0xd1, 0x5e, 0x5f, 0xdc, 0x8a, 0xf, 0x20, 0x6d, 0xa9, 0x8f, 0x7a, 0x18, 0x95, 0x36, 0x7a, 0x18, 0xd, 0x44, 0xa7, 0xed, 0xde, 0xb5, 0x67, 0x2d, 0x62, 0xac, 0x8f, 0x28, 0x34, 0x56, 0x7a, 0x85, 0x33, 0xa5, 0x3d, 0xdb, 0x59, 0x34, 0x6, 0x71, 0x8a, 0x88, 0xa, 0x17, 0xef, 0xa0, 0xa5, 0x2, 0xa, 0x56, 0x63, 0x7c, 0xf1, 0x88, 0x31, 0x1a, 0x89, 0x8a, 0xee, 0x5e,
	0xa1, 0x5, 0xee, 0xb8, 0x14, 0x15, 0xbd, 0xd0, 0xa0, 0x1b, 0x8d, 0xff, 0x18, 0xa2, 0x97, 0x98, 0x68, 0x34, 0x92, 0xf1, 0x9b, 0xdb, 0xdd, 0x52, 0xf1, 0xf5, 0xaf, 0x7f, 0xec, 0x77, 0xd9, 0xfd, 0x7e, 0x33, 0xb3, 0xdf, 0x37, 0x73, 0xbf, 0xef, 0xb1, 0x8b, 0xb6, 0xa3, 0x49, 0x59, 0x5b, 0x3c, 0x88, 0x26, 0x1b, 0x35, 0xac, 0xa3, 0x81, 0xcd, 0x4a, 0x21, 0x1d, 0x53, 0x4a, 0xe8, 0xe3, 0xb, 0x39, 0xdf, 0xa7, 0x14, 0xd3, 0x70, 0x3f, 0x8d, 0x76, 0xd0, 0x15, 0xab, 0x16, 0xf1, 0x98, 0x16,
	0xa2, 0x58, 0x8f, 0xd3, 0xb7, 0xe8, 0x3b, 0xf4, 0x7b, 0xfa, 0x2b, 0xad, 0x50, 0x6f, 0x56, 0xa3, 0x6a, 0x4c, 0xe3, 0xfc, 0xb1, 0xdb, 0x5f, 0xbb, 0xa3, 0x8e, 0xc5, 0xee, 0x90, 0x58, 0x7c, 0x74, 0x65, 0x9b, 0x4c, 0x6f, 0xb8, 0xb5, 0xcc, 0xd8, 0xa7, 0xcc, 0xba, 0x53, 0xa9, 0xa6, 0x7d, 0x1d, 0x3d, 0xfd, 0x5d, 0xe1, 0xbe, 0x6a, 0x1a, 0x59, 0x1d, 0xee, 0x55, 0xe9, 0x49, 0x45, 0x62, 0x75, 0x6c, 0x9b, 0xcc, 0xf9, 0x59, 0x45, 0x4e, 0xca, 0x1a, 0x1a, 0x41, 0xa7, 0xeb, 0x8, 0xa, 0x8,
	0x8d, 0xc6, 0xb7, 0x96, 0x71, 0x5e, 0x15, 0x82, 0x1f, 0xdd, 0xed, 0xea, 0x1c, 0x40, 0x93, 0x51, 0x2d, 0x96, 0xbe, 0x92, 0xca, 0xda, 0x78, 0x3, 0xd6, 0x40, 0xeb, 0xb, 0x8, 0x1a, 0x13, 0x77, 0x3c, 0xfa, 0xcb, 0x8f, 0x58, 0xbf, 0x6f, 0x86, 0x57, 0x47, 0xac, 0x83, 0x98, 0xf7, 0x2d, 0x45, 0x9c, 0x7, 0x19, 0x7a, 0x1b, 0x67, 0x4e, 0x49, 0x73, 0xda, 0x23, 0xa4, 0xab, 0xea, 0xd, 0xc9, 0x85, 0x1e, 0xb8, 0x25, 0xb0, 0xae, 0xf8, 0x62, 0x37, 0x6b, 0x22, 0xa4, 0x6, 0x77, 0xc8, 0xdd,
	0xc5, 0x24, 0x54, 0x23, 0x77, 0x7, 0x6b, 0xae, 0x2a, 0x88, 0x3d, 0xb0, 0x8a, 0xef, 0x55, 0x7c, 0xfa, 0x5e, 0xe5, 0xab, 0xd6, 0xca, 0x54, 0x3c, 0x89, 0x12, 0x38, 0x99, 0x93, 0x38, 0xbf, 0x85, 0x36, 0x50, 0x31, 0x5b, 0x99, 0xf2, 0xea, 0x2f, 0xd7, 0xbf, 0xae, 0x60, 0x2d, 0x9e, 0xc4, 0xec, 0x54, 0x2, 0x67, 0xce, 0x2e, 0x57, 0xa8, 0x42, 0x2b, 0x5e, 0xc7, 0x6c, 0x29, 0x8c, 0xa2, 0xac, 0xb0, 0xfd, 0xa, 0xc0, 0x9, 0xc0, 0x15, 0x80, 0x6b, 0xdf, 0x95, 0x98, 0xf7, 0x29, 0x9c, 0x59,
	0x48, 0xe7, 0x3, 0x2a, 0xde, 0x21, 0xd0, 0xf5, 0x80, 0x9e, 0xdb, 0x6e, 0xa3, 0x57, 0x5, 0xf2, 0xef, 0x1d, 0x46, 0x93, 0xd9, 0x2, 0xce, 0x85, 0x8e, 0x6a, 0x28, 0x3d, 0x2, 0x3a, 0x9e, 0x34, 0xc7, 0x45, 0xa4, 0x82, 0x88, 0x39, 0x2f, 0x43, 0xac, 0x8d, 0xc8, 0x9a, 0x64, 0x2c, 0xa3, 0x82, 0x5f, 0x32, 0x68, 0x3e, 0xdb, 0x4e, 0x45, 0x8c, 0x5c, 0xec, 0x41, 0x5a, 0x4, 0xfc, 0x16, 0xf5, 0xae, 0x9, 0xf7, 0x75, 0x75, 0xf7, 0x3e, 0xc2, 0x4f, 0x2a, 0x31, 0xd, 0x33, 0x34, 0x66, 0xce,
	0x7b, 0xd4, 0x9e, 0x4e, 0x73, 0xe1, 0xa4, 0xd2, 0x4e, 0x85, 0x1d, 0xc9, 0x47, 0x37, 0xae, 0xe7, 0x2, 0x11, 0x7f, 0x1f, 0x3d, 0x18, 0x18, 0xa, 0xd, 0x85, 0x56, 0x13, 0x59, 0xc7, 0xc9, 0xf, 0x2, 0xe2, 0xfe, 0x7e, 0xe0, 0x27, 0x77, 0x14, 0xec, 0xd1, 0x78, 0x91, 0x81, 0xf5, 0x7d, 0xca, 0x8c, 0xbe, 0xf0, 0xca, 0x48, 0x6f, 0xf, 0x4f, 0xbb, 0x7a, 0xf3, 0xb3, 0x9e, 0xfc, 0xac, 0x27, 0xdc, 0xd3, 0x69, 0x2e, 0xa4, 0x5d, 0x5e, 0xff, 0x7a, 0x3f, 0xe7, 0xf3, 0xe1, 0xff, 0x9f, 0xd9,
	0x82, 0x33, 0x71, 0xf8, 0xff, 0xd5, 0x80, 0x73, 0x80, 0x13, 0x80, 0x31, 0xbb, 0x71, 0x2b, 0xce, 0x24, 0xb7, 0xa2, 0xc9, 0x3b, 0xe1, 0xbc, 0xcb, 0xfc, 0x46, 0x6b, 0xab, 0x64, 0xb4, 0xb6, 0x49, 0x5f, 0xb7, 0xde, 0x5, 0xfa, 0x6e, 0xb8, 0xee, 0x1, 0x7c, 0xaf, 0xf4, 0x24, 0xd9, 0xe1, 0xdf, 0xe9, 0x7f, 0xc1, 0xff, 0xa2, 0x7f, 0x97, 0x7f, 0x37, 0xf8, 0xf3, 0xe9, 0xfd, 0xc1, 0x2, 0xe6, 0x65, 0xf8, 0x44, 0xae, 0x71, 0x73, 0x7d, 0x65, 0xea, 0x25, 0x5a, 0xee, 0xc6, 0x2c, 0x6, 0xbe,
	0x46, 0xca, 0xae, 0x1, 0xff, 0x23, 0x80, 0xe6, 0xb1, 0xc2, 0xaa, 0x52, 0x4d, 0xd6, 0x6a, 0x89, 0xdb, 0x98, 0x58, 0x2, 0x19, 0x7b, 0xec, 0x37, 0x1e, 0xd7, 0x21, 0x57, 0x81, 0xe1, 0xce, 0x37, 0x5, 0xc3, 0x9c, 0x77, 0x91, 0x1, 0x22, 0x11, 0x9f, 0x2e, 0x1f, 0xf6, 0x18, 0x95, 0xa9, 0xe8, 0x17, 0x72, 0xda, 0xab, 0xfb, 0x74, 0xaf, 0xde, 0xf8, 0xd5, 0x2f, 0xbc, 0x54, 0xf3, 0x20, 0x9f, 0xfe, 0x6d, 0x83, 0x9c, 0x7a, 0x8f, 0x7e, 0xdb, 0x50, 0x99, 0x12, 0xfc, 0x1e, 0xa4, 0xa5, 0xa4,
	0x6, 0xf2, 0x7, 0x8d, 0x2c, 0x6b, 0xf2, 0x18, 0x3f, 0x4, 0x7f, 0xe, 0x4a, 0xc, 0x8f, 0x62, 0x63, 0x59, 0xb3, 0x94, 0xd3, 0x21, 0x2, 0x3, 0x2d, 0x37, 0x7, 0x77, 0x2, 0x67, 0xc0, 0xf1, 0xc7, 0x38, 0x33, 0x37, 0xf0, 0x45, 0xd3, 0xba, 0x96, 0x51, 0x3a, 0x87, 0x3c, 0x5b, 0xc2, 0x79, 0x43, 0x90, 0x1d, 0x40, 0xa7, 0x47, 0x5b, 0x76, 0xd5, 0x1f, 0xf, 0xcc, 0x21, 0x6b, 0x5a, 0x7c, 0xfa, 0x9e, 0xc0, 0x2e, 0x38, 0x7b, 0xac, 0x99, 0x37, 0x95, 0x1b, 0x8b, 0x83, 0xbb, 0xea, 0xb,
	0x80, 0x43, 0x59, 0xdb, 0x5d, 0xbf, 0xa9, 0x79, 0x11, 0x8c, 0x5c, 0xcc, 0xc4, 0x37, 0x5, 0x37, 0x35, 0xef, 0xf, 0xf0, 0x26, 0x6c, 0x94, 0x92, 0x87, 0x5b, 0x86, 0xb7, 0xe6, 0x23, 0xfc, 0x19, 0xda, 0x88, 0xf, 0xd, 0x5b, 0xbc, 0xd, 0x36, 0x6f, 0x69, 0x36, 0xf7, 0xf9, 0x6, 0xf6, 0x91, 0x58, 0xd1, 0x66, 0x9c, 0xc1, 0xec, 0x8a, 0x41, 0x9c, 0x29, 0x6, 0x8e, 0x25, 0xf6, 0xfb, 0x26, 0x31, 0x26, 0x30, 0x2e, 0x1, 0x4e, 0xda, 0xf2, 0xf5, 0x22, 0xa7, 0xcb, 0x49, 0x55, 0xc8, 0xa7,
	0x4f, 0x3f, 0xeb, 0xb6, 0x12, 0xdb, 0xf6, 0x8, 0x3c, 0xbb, 0x1, 0x9e, 0x2d, 0x87, 0x2c, 0xb9, 0x48, 0xed, 0xe9, 0x57, 0xa3, 0xd1, 0xde, 0x3e, 0x1a, 0xee, 0xac, 0xbe, 0x75, 0x69, 0xa0, 0xa9, 0xba, 0x3f, 0xf2, 0x58, 0x58, 0x64, 0x12, 0xe7, 0x33, 0x61, 0xfd, 0x92, 0xbf, 0xac, 0xaf, 0x58, 0xd5, 0xd7, 0xbb, 0xba, 0x43, 0x3c, 0xd1, 0x1f, 0xae, 0x30, 0x26, 0x6, 0xcc, 0x9c, 0x9c, 0x9, 0x4c, 0x5d, 0x4b, 0xce, 0xd2, 0xcf, 0xad, 0x71, 0x81, 0x35, 0xce, 0x5a, 0xe3, 0xcb, 0x5e, 0x81,
	0xca, 0x7e, 0x5, 0x76, 0x1f, 0x47, 0x63, 0x6e, 0xb5, 0x60, 0x66, 0xd9, 0x7b, 0x50, 0x1d, 0xcf, 0x9b, 0x71, 0x2a, 0x81, 0x7d, 0x66, 0x46, 0x7a, 0xd6, 0x74, 0x74, 0x47, 0x3a, 0xab, 0x3b, 0x3a, 0x21, 0xbb, 0xfa, 0xfb, 0x85, 0xff, 0xaa, 0x90, 0xac, 0x95, 0x32, 0x7c, 0x74, 0x1e, 0x43, 0x7, 0xc0, 0xf2, 0x70, 0x99, 0x21, 0x6b, 0xb3, 0x61, 0xfc, 0x4, 0xc3, 0x87, 0x5d, 0x90, 0x8b, 0xf3, 0x54, 0x9c, 0x13, 0xbe, 0xa5, 0x9c, 0xa7, 0xb1, 0xc6, 0x63, 0xf6, 0xc, 0xaf, 0xfe, 0x6f, 0x36,
	0x1e, 0x22, 0x6c, 0x44, 0xf5, 0x48, 0x39, 0xa9, 0x51, 0xfc, 0x47, 0x91, 0xaf, 0x43, 0x43, 0x38, 0x73, 0x89, 0x95, 0xbb, 0x87, 0x0, 0x6f, 0x4, 0x6e, 0xec, 0x48, 0x2c, 0x1d, 0xc1, 0xa3, 0x85, 0x44, 0x32, 0xae, 0xc, 0xc5, 0xb4, 0x55, 0x9, 0x93, 0xdb, 0xd, 0x24, 0x5d, 0x7e, 0xa4, 0xfc, 0x50, 0x39, 0x62, 0x90, 0x4b, 0xe0, 0x4b, 0x5f, 0xd2, 0xd, 0x2b, 0x8d, 0xba, 0xa2, 0x72, 0xe8, 0x7c, 0x9c, 0x87, 0xd4, 0x22, 0xc8, 0xdd, 0x90, 0x2a, 0xea, 0x39, 0xa4, 0xce, 0xce, 0x63, 0x91,
	0xcd, 0x38, 0x69, 0xae, 0x8, 0x2d, 0xd6, 0x7c, 0xba, 0xc4, 0x2e, 0xdc, 0x8f, 0x33, 0x5, 0xc, 0xd8, 0x3b, 0x21, 0x6b, 0xee, 0x76, 0x39, 0x85, 0xc7, 0x5d, 0x46, 0xae, 0x11, 0xaa, 0xb1, 0xfd, 0x6a, 0x22, 0x8f, 0xff, 0xfc, 0x63, 0x65, 0x6a, 0x5d, 0x59, 0xf, 0x11, 0x7d, 0x31, 0x7e, 0xa8, 0x7, 0x71, 0xbe, 0x52, 0xf5, 0xea, 0xe2, 0x5a, 0xe, 0x7d, 0x39, 0x2, 0x3a, 0x62, 0x61, 0xc8, 0x88, 0x6d, 0x38, 0x43, 0x3e, 0x7c, 0x48, 0xc5, 0xec, 0x36, 0x40, 0xe9, 0x3c, 0xba, 0x17, 0x50,
	0x36, 0x8f, 0xbc, 0x80, 0xb6, 0xbf, 0x7a, 0xe, 0xad, 0x6, 0x9b, 0xa7, 0x6f, 0x10, 0xbd, 0xdd, 0x13, 0xed, 0xee, 0xa5, 0xd7, 0xd6, 0x2d, 0xb8, 0x6e, 0x91, 0xe0, 0xa3, 0xcd, 0x8a, 0x57, 0x37, 0x11, 0x48, 0x74, 0xb, 0xce, 0x87, 0x42, 0x8, 0x72, 0x7b, 0x6e, 0x60, 0x67, 0x68, 0x2a, 0x3f, 0x13, 0x78, 0x54, 0x32, 0xbc, 0xfa, 0xa3, 0xd6, 0xfb, 0x62, 0xad, 0xfd, 0xde, 0x60, 0xcc, 0x42, 0x22, 0x16, 0xe7, 0x3f, 0x9d, 0xb0, 0xd6, 0x62, 0x53, 0xcf, 0x3c, 0x63, 0xa1, 0x42, 0xc6, 0x3f,
	0xc1, 0x19, 0x71, 0x89, 0x1a, 0x49, 0xf, 0x9b, 0xb3, 0x9f, 0x5b, 0x5a, 0x54, 0xce, 0xc4, 0xb0, 0xbd, 0x3, 0x1a, 0xcb, 0x5a, 0xf8, 0xd, 0x37, 0xe7, 0x53, 0x7b, 0x40, 0x9c, 0x42, 0x10, 0xeb, 0x9, 0xab, 0x9f, 0x66, 0x2d, 0x8d, 0xbf, 0xf4, 0xe6, 0xfd, 0x98, 0x18, 0x1f, 0x9f, 0xc2, 0xc9, 0xf4, 0xd4, 0xec, 0xfd, 0xe4, 0x19, 0xab, 0xb3, 0x5e, 0x47, 0xd8, 0xa0, 0x9d, 0xbf, 0x75, 0x44, 0xd4, 0xfe, 0x7a, 0x55, 0xce, 0xd9, 0x36, 0xf6, 0xb9, 0x6c, 0xdf, 0xf6, 0x99, 0x64, 0xcd, 0x3e,
	0xa9, 0x6d, 0x6d, 0x9f, 0x70, 0xad, 0x35, 0xb6, 0xe7, 0x63, 0x96, 0xc6, 0x27, 0x71, 0xd2, 0x64, 0x2f, 0xdf, 0xe7, 0xf3, 0xcc, 0xd8, 0xf3, 0xe7, 0xb3, 0x56, 0x1b, 0xb4, 0xad, 0x51, 0x96, 0xd, 0xda, 0x31, 0xb9, 0x94, 0x89, 0xbc, 0x2a, 0xf, 0xd8, 0x3b, 0xc8, 0xda, 0xdf, 0xed, 0x81, 0xb2, 0xe7, 0xf6, 0x90, 0x75, 0xce, 0x12, 0x7f, 0x5a, 0x9b, 0xbe, 0x93, 0x1b, 0x76, 0x9a, 0x43, 0xc4, 0xd5, 0x9c, 0x30, 0xcf, 0xae, 0x58, 0x9a, 0x58, 0x7a, 0xb1, 0xa5, 0x7f, 0x72, 0x57, 0x85, 0xe,
	0x6, 0x72, 0x81, 0xa7, 0xd5, 0x1, 0x32, 0xde, 0x20, 0x1f, 0x9e, 0x5, 0xac, 0x83, 0x97, 0x4f, 0xcf, 0x45, 0x2, 0x9d, 0x16, 0x35, 0xe3, 0x32, 0x38, 0xd4, 0x7, 0xfe, 0x72, 0xbf, 0x8a, 0xb2, 0x67, 0xb8, 0x87, 0x9c, 0x59, 0x68, 0x5a, 0x81, 0xd, 0x74, 0xed, 0xcb, 0xd, 0xd1, 0x9f, 0x5c, 0xac, 0x94, 0xe0, 0xe3, 0x73, 0xd, 0x74, 0xc, 0xde, 0xde, 0x70, 0x55, 0xa6, 0x64, 0x8d, 0x43, 0xe7, 0xf8, 0x67, 0x5f, 0x50, 0x3f, 0x47, 0x7d, 0xfa, 0x77, 0x1c, 0xeb, 0xa5, 0xda, 0x24, 0xdc,
	0xcf, 0x2c, 0x14, 0x75, 0x2f, 0xfa, 0x1c, 0xd4, 0x10, 0xf4, 0xb9, 0xaa, 0xd0, 0xf7, 0xc1, 0x53, 0x4d, 0xb2, 0x7e, 0x69, 0xd0, 0x27, 0x72, 0x5, 0x7e, 0x72, 0xa, 0xd, 0x5c, 0x10, 0x2c, 0x1, 0xdd, 0x45, 0xa, 0x58, 0x45, 0xb0, 0x12, 0xc6, 0x76, 0xbc, 0x64, 0xdd, 0x8e, 0xa1, 0xe0, 0x7, 0x27, 0xbb, 0x88, 0x1d, 0x39, 0xf3, 0x94, 0xeb, 0xa7, 0x4e, 0x59, 0xb, 0xdc, 0xd4, 0x42, 0x46, 0xac, 0x31, 0xfe, 0xeb, 0x8c, 0x5e, 0xdd, 0x3c, 0xa3, 0xbd, 0x6e, 0x56, 0x37, 0x1e, 0x5d, 0x6c,
	0xad, 0x8b, 0xaf, 0x14, 0xce, 0x96, 0x13, 0x33, 0x6b, 0xf2, 0xd1, 0x38, 0x52, 0x36, 0x2d, 0x73, 0x57, 0x58, 0x99, 0x2f, 0x70, 0xe9, 0x54, 0xee, 0x4d, 0x5f, 0x95, 0x35, 0xdb, 0xb7, 0xe8, 0xba, 0xd0, 0x69, 0x93, 0x38, 0x29, 0x3c, 0x66, 0xcf, 0xf3, 0x90, 0xaf, 0xac, 0xdc, 0x74, 0x4b, 0x9b, 0x67, 0x7b, 0xe, 0x65, 0x27, 0xce, 0xcb, 0x69, 0x94, 0xb5, 0x19, 0xd8, 0x98, 0xe7, 0x78, 0x2d, 0xbc, 0x55, 0xd7, 0xa9, 0x63, 0xca, 0x85, 0xd0, 0x1b, 0xaf, 0x81, 0x2f, 0x97, 0x3d, 0xd0, 0x27,
	0x67, 0x3, 0xee, 0x4, 0xbc, 0x17, 0xb0, 0xf8, 0xe, 0xfa, 0x8, 0xf0, 0x3e, 0xe8, 0x99, 0xdc, 0x11, 0x47, 0x1c, 0x71, 0xc4, 0x11, 0x47, 0x1c, 0x71, 0xc4, 0x11, 0x47, 0x1c, 0x71, 0xc4, 0x11, 0x47, 0x1c, 0x71, 0xc4, 0x11, 0x47, 0x1c, 0x71, 0xc4, 0x91, 0xff, 0x9d, 0xfc, 0x1, 0xf3, 0xf9, 0x27, 0x79, 0x0, 0x0
};

	static inline void uploadPlotPatch(EVE_HalContext *phost)
	{
		eve_assert_do(EVE_Cmd_wr32(phost, CMD_LOADPATCH));      
		eve_assert_do(EVE_Cmd_wr32(phost, 0));                                          
		uint32_t x = 0;
		while( x < patchLen) 
		{
			eve_assert_do(EVE_Cmd_wr32(phost, (plot_patch_data[x] | (plot_patch_data[x + 1]<<8) | (plot_patch_data[x + 2]<<16) | (plot_patch_data[x + 3]<<24))));  // 32-bits
	    	x = x + 4;
		}
		eve_assert_do(EVE_Cmd_waitFlush(phost));
	}	
#endif

/**
 * @brief Clear the screen
 *
 * @param phost Pointer to Hal context
 */
EVE_HAL_EXPORT void EVE_Util_clearScreen(EVE_HalContext *phost)
{
#if (EVE_SUPPORT_CHIPID < EVE_FT810) || defined(EVE_MULTI_GRAPHICS_TARGET)
	if (EVE_CHIPID < EVE_FT810)
	{
		EVE_Hal_wrProgMem(phost, RAM_DL, c_DlCodeBootupFT80X, sizeof(c_DlCodeBootupFT80X));
	}
	else
#endif
	{
#if (EVE_SUPPORT_CHIPID >= EVE_FT810)
		EVE_Hal_wrProgMem(phost, RAM_DL, c_DlCodeBootup, sizeof(c_DlCodeBootup));
#endif
	}
	EVE_Hal_wr8(phost, REG_DLSWAP, DLSWAP_FRAME);
}

void EVE_Hal_getNormalizedTouchTransform(
    EVE_HalContext *phost,
    int32_t *outA, int32_t *outB, int32_t *outC,
    int32_t *outD, int32_t *outE, int32_t *outF)
{
	uint32_t rotate, hsize, vsize;
	bool swapXY, mirrorX, mirrorY;
	int32_t a, b, c, d, e, f;

#if EVE_SUPPORT_CHIPID_FROM(EVE_BT820)
	if (EVE_CHIPID >= EVE_BT820)
	{
		rotate = EVE_Hal_rd32(phost, REG_RE_ROTATE);
		hsize = EVE_Hal_rd32(phost, REG_RE_W);
		vsize = EVE_Hal_rd32(phost, REG_RE_H);
	}
	else
#endif
	{
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
		rotate = EVE_Hal_rd32(phost, REG_ROTATE);
		hsize = EVE_Hal_rd32(phost, REG_HSIZE);
		vsize = EVE_Hal_rd32(phost, REG_VSIZE);
#endif
	}

	swapXY = (rotate & 0x2) != 0;
	mirrorX = (((rotate & 0x1) != 0) ^ ((rotate & 0x4) != 0));
	mirrorY = (((rotate & 0x1) != 0) ^ ((rotate & 0x2) != 0));

	a = EVE_Hal_rd32(phost, REG_TOUCH_TRANSFORM_A);
	b = EVE_Hal_rd32(phost, REG_TOUCH_TRANSFORM_B);
	c = EVE_Hal_rd32(phost, REG_TOUCH_TRANSFORM_C);
	d = EVE_Hal_rd32(phost, REG_TOUCH_TRANSFORM_D);
	e = EVE_Hal_rd32(phost, REG_TOUCH_TRANSFORM_E);
	f = EVE_Hal_rd32(phost, REG_TOUCH_TRANSFORM_F);

	if (swapXY)
	{
		int32_t ta = a, tb = b, tc = c;
		a = d;
		b = e;
		c = f;
		d = ta;
		e = tb;
		f = tc;
	}

	if (mirrorY)
	{
		d = -d;
		e = -e;
		f = ((int32_t)(vsize - 1) << 16) - f;
	}

	if (mirrorX)
	{
		a = -a;
		b = -b;
		c = ((int32_t)(hsize - 1) << 16) - c;
	}

	*outA = a;
	*outB = b;
	*outC = c;
	*outD = d;
	*outE = e;
	*outF = f;
}

void EVE_Hal_setNormalizedTouchTransform(
    EVE_HalContext *phost,
    int32_t inA, int32_t inB, int32_t inC,
    int32_t inD, int32_t inE, int32_t inF)
{
	uint32_t rotate, hsize, vsize;
	bool swapXY, mirrorX, mirrorY;
	int32_t a, b, c, d, e, f;

#if EVE_SUPPORT_CHIPID_FROM(EVE_BT820)
	if (EVE_CHIPID >= EVE_BT820)
	{
		rotate = EVE_Hal_rd32(phost, REG_RE_ROTATE);
		hsize = EVE_Hal_rd32(phost, REG_RE_W);
		vsize = EVE_Hal_rd32(phost, REG_RE_H);
	}
	else
#endif
	{
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
		rotate = EVE_Hal_rd32(phost, REG_ROTATE);
		hsize = EVE_Hal_rd32(phost, REG_HSIZE);
		vsize = EVE_Hal_rd32(phost, REG_VSIZE);
#endif
	}

	swapXY = (rotate & 0x2) != 0;
	mirrorX = (((rotate & 0x1) != 0) ^ ((rotate & 0x4) != 0));
	mirrorY = (((rotate & 0x1) != 0) ^ ((rotate & 0x2) != 0));

	a = inA;
	b = inB;
	c = inC;
	d = inD;
	e = inE;
	f = inF;

	if (mirrorX)
	{
		a = -a;
		b = -b;
		c = ((int32_t)(hsize - 1) << 16) - c;
	}

	if (mirrorY)
	{
		d = -d;
		e = -e;
		f = ((int32_t)(vsize - 1) << 16) - f;
	}

	if (swapXY)
	{
		int32_t ta = a, tb = b, tc = c;
		a = d;
		b = e;
		c = f;
		d = ta;
		e = tb;
		f = tc;
	}

	EVE_Hal_wr32(phost, REG_TOUCH_TRANSFORM_A, a);
	EVE_Hal_wr32(phost, REG_TOUCH_TRANSFORM_B, b);
	EVE_Hal_wr32(phost, REG_TOUCH_TRANSFORM_C, c);
	EVE_Hal_wr32(phost, REG_TOUCH_TRANSFORM_D, d);
	EVE_Hal_wr32(phost, REG_TOUCH_TRANSFORM_E, e);
	EVE_Hal_wr32(phost, REG_TOUCH_TRANSFORM_F, f);
}

EVE_HAL_EXPORT void EVE_Util_bootupDefaults(EVE_HalContext *phost, EVE_BootupParameters *bootup)
{
	int32_t chipId = EVE_CHIPID;
	(void)chipId;

	memset(bootup, 0, sizeof(EVE_BootupParameters));

#if !defined(EVE_USE_INTERNAL_OSC)
	/* Board without external oscillator will not work when ExternalOsc is enabled */
	bootup->ExternalOsc = true;
#endif

#ifdef EVE_SYSTEM_CLOCK
	bootup->SystemClock = EVE_SYSTEM_CLOCK;
#else
	if (chipId >= EVE_FT800 && chipId <= EVE_BT820)
	{
		/* These are the default configured system clock settings for HAL, which may differ from the hardware defaults */
#if EVE_SUPPORT_CHIPID_FROM(EVE_BT815)
		if (chipId >= EVE_BT815)
		{
			bootup->SystemClock = EVE_SYSCLK_72M; /* 72Mhz is default for BT8xx */
			/* For BT81x the hardware default is 60MHz, the default HAL configuration is 72MHz */
		}
		else
#endif
#if EVE_SUPPORT_CHIPID_RANGE(EVE_FT810, BT815)
		    if (chipId >= EVE_FT810)
		{
			bootup->SystemClock = EVE_SYSCLK_60M; /* 60Mhz is default for FT81x */
		}
		else
#endif
		{
#if EVE_SUPPORT_CHIPID_TO(EVE_FT810)
			bootup->SystemClock = EVE_SYSCLK_48M; /* 48Mhz is default for FT80x */
#endif
		}
	}
	else
	{
		/* If the chipid is not known yet, this will reconfigure the system clock during `EVE_Util_bootup`.
		Additional default clock overrides need to be implemented there as well. */
		bootup->SystemClock = EVE_SYSCLK_DEFAULT;
	}
#endif

#if (EVE_SUPPORT_CHIPID >= EVE_FT810)
#if defined(EVE_MULTI_PLATFORM_TARGET)
	if (EVE_HOST == EVE_HOST_FT4222 && chipId >= EVE_FT810)
	{
		bootup->SpiChannels = EVE_SPI_QUAD_CHANNEL;
		bootup->SpiDummyBytes = 2;
	}
	else
	{
		bootup->SpiChannels = EVE_SPI_SINGLE_CHANNEL;
		bootup->SpiDummyBytes = 1;
	}
#elif defined(ENABLE_SPI_QUAD)
	bootup->SpiChannels = EVE_SPI_QUAD_CHANNEL;
	bootup->SpiDummyBytes = 2;
#elif defined(ENABLE_SPI_DUAL)
	bootup->SpiChannels = EVE_SPI_DUAL_CHANNEL;
	bootup->SpiDummyBytes = 2;
//#elif defined(ENABLE_SPI_SINGLE)
//	bootup->SpiChannels = EVE_SPI_SINGLE_CHANNEL;
//	bootup->SpiDummyBytes = 1;
//#endif
//
//#if (defined(EVE_SPICHANNEL_QUAD) && defined(FT9XX_PLATFORM)) // FIXME: This should set ENABLE_ flags in config, not override here
//	bootup->SpiChannels = EVE_SPI_QUAD_CHANNEL;
//	bootup->SpiDummyBytes = 2;
//	eve_printf_debug("ESD--QUAD SpiChannels == %d SpiDummyBytes == %d ===\n", (bootup->SpiChannels), (bootup->SpiDummyBytes));
//#endif
//#if (defined(EVE_SPICHANNEL_SINGLE) && defined(FT9XX_PLATFORM)) // FIXME: This should set ENABLE_ flags in config, not override here
#else
	bootup->SpiChannels = EVE_SPI_SINGLE_CHANNEL;
	bootup->SpiDummyBytes = 1;
//	eve_printf_debug("ESD--SINGLE SpiChannels == %d SpiDummyBytes == %d ===\n", (bootup->SpiChannels), (bootup->SpiDummyBytes));
#endif

#endif
}

#ifndef EVE_HARDCODED_DISPLAY_TIMINGS
#define EVE_HARDCODED_DISPLAY_TIMINGS 1
#endif

#if (EVE_SUPPORT_CHIPID >= EVE_BT817) && (EVE_SUPPORT_CHIPID < EVE_BT820 || defined(EVE_MULTI_GRAPHICS_TARGET))

static uint16_t getPClkFreq(uint32_t freq)
{
	// uint32_t resFreq;
	uint32_t res;
	// eve_printf_debug("Want frequency %i\n", (int)wantFreq);
	int32_t nearestFreq = 0x7FFFFFFFL;
	int32_t nearestDiv = 0;
	int32_t nearestMul = 0;
	for (uint32_t div = 1; div < 15; ++div)
	{
		uint32_t mul = (freq * div) / 6000000;
		if (mul > 19)
			break;
		// eve_printf_debug("Div %i Mul %i\n", (int)div, (int)mul);
		if (mul > 1)
		{
			int32_t resFreq = 6000000 * mul / div;
			// eve_printf_debug("Res frequency %i\n", (int)resFreq);
			int32_t diffFreq = resFreq - (int32_t)freq;
			if (diffFreq >= 0 && diffFreq < (nearestFreq - (int32_t)freq))
			{
				nearestFreq = resFreq;
				nearestDiv = div;
				nearestMul = mul;
			}
		}
		if (mul && mul <= 18)
		{
			int32_t resFreq = 6000000 * (mul + 1) / div;
			// eve_printf_debug("Res frequency (mul + 1) %i\n", (int)resFreq);
			int32_t diffFreq = resFreq - (int32_t)freq;
			if (diffFreq >= 0 && diffFreq < (nearestFreq - (int32_t)freq))
			{
				nearestFreq = resFreq;
				nearestDiv = div;
				nearestMul = mul + 1;
			}
		}
	}
	if (nearestFreq != 0x7FFFFFFFL)
	{
		// resFreq = nearestFreq;
		// maxRate = resFreq / minCycles;
		uint32_t pllFreq = 12 * nearestMul;
		uint32_t range = pllFreq >= 160 ? 3 : (pllFreq >= 80 ? 2 : (pllFreq >= 40 ? 1 : 0));
		res = (nearestDiv & 0xF) | ((nearestMul & 0x3F) << 4) | (range << 10);
		// eve_printf_debug("Set PCLK Freq to %X (freq %i, mul %i, div %i)\n", (int)config->PClkFreq, (int)nearestFreq, (int)nearestMul, (int)nearestDiv);
		return res;
	}
	return 0;
}

static inline uint32_t getFreqFromPClkFreq(uint16_t pclkFreq)
{
	if (!pclkFreq)
		return 0;
	uint32_t mul = (pclkFreq >> 4) & 0x3F;
	uint32_t div = pclkFreq & 0xF;
	uint32_t res = mul * (12 * 1000 * 1000) / div / 2;
	return res;
}

#endif

static bool configDefaultsEx(EVE_HalContext *phost, EVE_ConfigParameters *config, uint32_t width, uint32_t height, uint32_t refreshRate, uint32_t hsfWidth, uint32_t freq)
{
	/* Only false if the output resolution will be wrong.
	Not affected by unsupported HSF or refreshRate */
	bool supportedResolution = true;

	uint32_t screenWidth;
	uint32_t pixels;
	uint32_t minCycles;
	uint32_t maxRate;
	uint32_t pclk;

	uint32_t cycles;
	uint32_t vcycle;
	uint32_t hcycle;
	uint32_t hoffset;
	uint32_t voffset;
	uint32_t hsync1;
	uint32_t vsync1;

#ifdef EVE_SUPPORT_LVDS
	uint32_t minTxClkDiv = 3; // LVDSTX clock is 576MHz / txClkDiv / 2, (divider is register value TXCLKDIV+1) (stored as PCLK in config)
	uint32_t ppcMul = 4; // Pixels per clock (REG_SO_MODE) - can be 1, 2 or 4 (prefer largest, lowered when lvds tx clock becomes too low)
#endif
#if defined(EVE_SUPPORT_HSF) || defined(EVE_SUPPORT_LVDS)
	uint32_t maxFreq;
	uint32_t maxRatePass;
#endif
#ifdef EVE_SUPPORT_HSF
	uint32_t pclkFreq = 0;
#endif

	memset(config, 0, sizeof(EVE_ConfigParameters));

	/*
	Refresh rate is as follows:
	REG_VCYCLE * REG_HCYCLE * REG_PCLK is the number of clocks each frame takes,
	Divide REG_FREQUENCY by this number to get the frame refresh rate in Hz.
	REG_HCYCLE must be higher than REG_HSIZE, REG_VCYCLE must be higher than REG_VSIZE.

	By default, FT800 is at 48MHz, FT810 is at 60MHz, and BT815 is increased to 72MHz.
	User may configure this differently in `EVE_Util_bootup`.

	Clocks per frame by clock frequency at 60Hz:
	- 84MHz: 1400k
	- 72MHz: 1200k
	- 60MHz: 1000k
	- 48MHz: 800k
	- 36MHz: 600k
	- 24MHz: 400k

	Clocks required per frame by display resolution at 60Hz:
	Max PCLK at frequency: 24  36  48  60  72  84MHz
	- 320x240: 76.8k        5   7  10  13  15  18
	- 480x272: 130.56k      3   4   6   7   9  10
	*/

	/* Trim oversize displays */
	if (EVE_CHIPID >= EVE_FT810)
	{
#ifdef EVE_SUPPORT_HSF
		if (hsfWidth >= width)
		{
			eve_printf_debug("Screen HSF width %i is too large, disabled\n", (int)config->HsfWidth);
			hsfWidth = 0;
			width = hsfWidth;
			supportedResolution = false;
		}
#endif
		if (width > 2047)
		{
			eve_printf_debug("Line buffer width %i is too large for this configuration\n", (int)config->Width);
			width = 2047;
			supportedResolution = false;
		}
		if (height > 2047)
		{
			eve_printf_debug("Screen height %i is too large for this configuration\n", (int)config->Height);
			height = 2047;
			supportedResolution = false;
		}
	}
	else
	{
		if (width > 493)
		{
			/* 494 and up are not reliable */
			eve_printf_debug("Screen width %i is too large for this configuration\n", (int)config->Width);
			width = 493;
			supportedResolution = false;
		}
		if (height > 511)
		{
			eve_printf_debug("Screen height %i is too large for this configuration\n", (int)config->Height);
			height = 511;
			supportedResolution = false;
		}
	}

#ifdef EVE_SUPPORT_HSF
	if (EVE_Hal_supportHsf(phost))
	{
		/* Render width */
		config->Width = (int16_t)width;

		/* Screen width */
		config->HsfWidth = (int16_t)hsfWidth;
	}
	else
#endif
	if (hsfWidth)
	{
		/* Use the screen width if HSF is not supported */
		config->Width = (int16_t)hsfWidth;
		eve_printf_debug("Display config specifies HSF but not supported on this platform\n");
	}
	else
	{
		config->Width = (int16_t)width;
	}
	config->Height = (int16_t)height;
	config->RefClkFreq = freq; /* Reference clock frequency */

	screenWidth = hsfWidth ? hsfWidth : width; /* Use screen width for calculation */
	pixels = screenWidth * height;

	/* Calculate maximum refresh rate */
	minCycles = pixels + (pixels >> 2); /* pixels * 1.25 */
#ifdef EVE_SUPPORT_LVDS
	if (EVE_Hal_supportLvds(phost))
	{
		// Parameter `freq` is 576MHz by default (38.4MHz * 15)
		maxFreq = (freq / minTxClkDiv) / 2; /* TXCLKDIV+1 = 4 */
		// maxRatePass = ((freq / 2) - 1) / minCycles;
		maxRatePass = ((freq * ppcMul) / 2) / minCycles;
		// Maximum frequency is 100MHz
		// Minimum frequency is 33.33MHz
		// TODO: Review documentation
		// 96MHz is not listed in the table, so limiting this below 96MHz
		while (maxFreq >= 96000000 && minTxClkDiv < 15) 
			maxFreq = (freq / ++minTxClkDiv) / 2;
		maxFreq = maxFreq * ppcMul; // 72MHz * 4 = 288MHz
		maxRate = maxFreq / minCycles;
	}
	else
#endif
#ifdef EVE_SUPPORT_HSF
	if (EVE_Hal_supportHsf(phost))
	{
		maxRatePass = (freq - 1) / minCycles;
		maxFreq = (freq << 1) < 96000000 ? (freq << 1) : 96000000;
		if (hsfWidth)
			maxRate = maxRatePass; // No 2X
		else
			maxRate = maxFreq / minCycles; // Support 2X
	}
	else
#endif
	{
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
		maxRate = freq / minCycles;
#endif
	}

	/* If the refresh rate is too low, try with tighter settings */
	if (refreshRate > maxRate)
	{
		minCycles = pixels + (pixels >> 3); /* pixels * 1.125 */
#ifdef EVE_SUPPORT_LVDS
		if (EVE_Hal_supportLvds(phost))
		{
			maxRate = maxFreq / minCycles;
			// maxRatePass = ((freq / 2) - 1) / minCycles;
			maxRatePass = ((freq * ppcMul) / 2) / minCycles;
		}
		else
#endif
#ifdef EVE_SUPPORT_HSF
		if (EVE_Hal_supportHsf(phost))
		{
			maxRatePass = (freq - 1) / minCycles;
			if (hsfWidth)
				maxRate = maxRatePass; // No 2X
			else
				maxRate = maxFreq / minCycles; // Support 2X
		}
		else
#endif
		{
			maxRate = freq / minCycles;
		}
	}

#ifdef EVE_SUPPORT_HSF
	if (EVE_Hal_supportHsf(phost) && (hsfWidth || refreshRate > maxRatePass))
	{
		/* Setup EXTSYNC with freq lower (not equal) than system frequency for HSF, up to 2X otherwise */
		/* LL2 frequency = 12MHz * REG_PCLK_FREQ[8:4] */
		/* PCLK frequency = PLL2 frequency / REG_PCLK_FREQ[3:0] / 2 */
		/* PCLK frequency = 6MHz * REG_PCLK_FREQ[8:4] / REG_PCLK_FREQ[3:0] */
		/* PCLK frequency = 6MHz * mul / div */
		int32_t wantFreq = minCycles * (refreshRate > maxRate ? maxRate : refreshRate); // 25.92m for 800x480
		config->PClkFreq = getPClkFreq(wantFreq);
		pclkFreq = getFreqFromPClkFreq(config->PClkFreq);
		if (pclkFreq)
			maxRate = pclkFreq / minCycles;
	}
#endif

#ifdef EVE_SUPPORT_HSF
	if (pclkFreq)
	{
		pclk = 1;
	}
	else
#endif
	if (maxRate < refreshRate)
	{
		/* Trim unsupported framerate */
		eve_printf_debug("Frame rate limited to %d\n", (unsigned int)maxRate);
		refreshRate = maxRate;
#ifdef EVE_SUPPORT_LVDS
		if (EVE_Hal_supportLvds(phost))
		{
			pclk = minTxClkDiv;
		}
		else
#endif
		{
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
			pclk = 1;
#endif
		}
	}
	else
	{
#ifdef EVE_SUPPORT_LVDS
		if (EVE_Hal_supportLvds(phost))
		{
			/* TODO: Verify if ppcMul needs to be minimized or maximized.
			Current strategy is to maximize it. Possibly it must be a fixed value instead? */

			/* Calculate a valid tx clk divider and pixel per clock multiplier */
			/* Initial proposal, maximum divider */
			uint32_t lvdxTxClk;
			pclk = maxRatePass / refreshRate;
			lvdxTxClk = (freq / pclk) / 2;
			/* While lvds tx clock at 33.3 MHz or lower, reduce pixels per clock */
			while (lvdxTxClk <= 33333333 && ppcMul > 1)
			{
				ppcMul >>= 1; /* Halve pixels per clock */
				maxRatePass = ((freq * ppcMul) / 2) / minCycles;
				pclk = maxRatePass / refreshRate;
				lvdxTxClk = (freq / pclk) / 2; /* Recalculate tx clock */
			}
			/* Safety limit */
			if (pclk > 8)
				pclk = 8;
		}
		else
#endif
#ifdef EVE_SUPPORT_HSF
		if (EVE_Hal_supportHsf(phost))
		{
			pclk = maxRatePass / refreshRate;
		}
		else
#endif
		{
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
			pclk = maxRate / refreshRate;
#endif
		}
	}
	config->PCLK = (uint8_t)pclk;
#ifdef EVE_SUPPORT_LVDS
	config->PixelsPerClock = ppcMul;
#endif

	/* Change PCLKPol if the right edge of the display is noisy,
	or if the display is showing jagged colors. Values 0 or 1.
	This toggles the polarity of the PCLK. */
	config->PCLKPol = 1; /* non-default */

	/* Approximate an average good setting */
#ifdef EVE_SUPPORT_LVDS
	if (EVE_Hal_supportLvds(phost))
	{
		cycles = freq * ppcMul / (refreshRate * pclk * 2);
	}
	else
#endif
	{
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
#ifdef EVE_SUPPORT_HSF
		cycles = (pclkFreq ? pclkFreq : freq) / (refreshRate * pclk);
#else
		cycles = freq / (refreshRate * pclk);
#endif
#endif
	}
#ifdef EVE_SUPPORT_LVDS
	uint32_t ppcMask = ~((uint32_t)(ppcMul - 1));
#endif
	vcycle = height * cycles / pixels; /* max vcycle without blanking, on average 1.125x height, or 1.25x */
	vcycle = (vcycle + ((vcycle + height) >> 1) + height + height) >> 2; /* vcycle is 1.5 max vcycle + 2.5 height / 4, so on average 1.046875x height, or 1.09375x */
	hcycle = cycles / vcycle; /* cycles div by vcycle, on average 1.07375x, or 1.1425x */
#ifdef EVE_SUPPORT_LVDS
	if (EVE_Hal_supportLvds(phost)) hcycle &= ppcMask; // REG_HCYCLE must be divisible by PPC multiplier
#endif
	hoffset = (hcycle - screenWidth) >> 1;
#ifdef EVE_SUPPORT_LVDS
	if (EVE_Hal_supportLvds(phost)) hoffset &= ppcMask; // REG_HOFFSET must be divisible by PPC multiplier
#endif
	voffset = (vcycle - height) >> 1;
	hsync1 = hoffset >> 1;
#ifdef EVE_SUPPORT_LVDS
	if (EVE_Hal_supportLvds(phost)) hsync1 &= ppcMask; // REG_HSYNC1 must be divisible by PPC multiplier
#endif
	vsync1 = voffset >> 1;
	hoffset += hsync1;
	voffset += vsync1;
#ifdef EVE_SUPPORT_HSF
	if (pclkFreq && pclkFreq >= freq)
	{
		hcycle &= ~1UL;
		hoffset &= ~1UL;
		hsync1 &= ~1UL;
	}
#endif
	config->HCycle = (int16_t)hcycle;
	config->HSync0 = 0;
	config->HSync1 = (int16_t)hsync1;
	config->HOffset = (int16_t)hoffset;
	config->VCycle = (int16_t)vcycle;
	config->VSync0 = 0;
	config->VSync1 = (int16_t)vsync1;
	config->VOffset = (int16_t)voffset;

	/* Verify */
	eve_assert(config->HSync1);
	eve_assert(config->HOffset > config->HSync1);
#ifdef EVE_SUPPORT_HSF
	eve_assert(config->HCycle > (config->HsfWidth ? config->HsfWidth : config->Width));
	eve_assert((config->HCycle - (config->HsfWidth ? config->HsfWidth : config->Width)) > config->HOffset);
#else
	eve_assert(config->HCycle > config->Width);
	eve_assert((config->HCycle - config->Width) > config->HOffset);
#endif
	eve_assert(config->VSync1);
	eve_assert(config->VOffset > config->VSync1);
	eve_assert(config->VCycle > config->Height);
	eve_assert((config->VCycle - config->Height) > config->VOffset);
	
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)

#if (EVE_SUPPORT_CHIPID >= EVE_BT815)
#ifdef EVE_ADAPTIVE_FRAMERATE
	config->AdaptiveFramerate = 1;
#else
	config->AdaptiveFramerate = 0;
#endif
#endif

#if (EVE_SUPPORT_CHIPID >= EVE_BT817)
	config->AhHCycleMax = 0;
#endif

	/* Other options */
	/* Toggle CSpread if you see red and blue fringing on black and white edges */
	config->CSpread = 0; /* non-default */
	/* Change this if RGB colors are swapped */
	config->Swizzle = 0;

	if (EVE_CHIPID == EVE_FT812
	    || EVE_CHIPID == EVE_FT813
	    || EVE_CHIPID == EVE_BT882
	    || EVE_CHIPID == EVE_BT883
	    || EVE_CHIPID >= EVE_BT815)
	{
		config->Dither = 0;
		config->OutBitsR = 8;
		config->OutBitsG = 8;
		config->OutBitsB = 8;
	}
	else
	{
		config->Dither = 1;
		config->OutBitsR = 6;
		config->OutBitsG = 6;
		config->OutBitsB = 6;
	}

#endif	  

	return supportedResolution;
}

static inline uint32_t getFreqOrPllOut(EVE_HalContext *phost)
{
	uint32_t freq;
#ifdef EVE_SUPPORT_LVDS
	if (EVE_Hal_supportLvds(phost))
	{
		// On BT82X, pass the SYSPLL output frequency instead of REG_FREQUENCY
		uint32_t sysStat0 = EVE_Hal_rd32(phost, SYS_STAT_0);
		int pllMultiplier = (sysStat0 >> 8) & 0x7F;
		freq = EVE_OSC_FREQ * pllMultiplier;
	}
	else
#endif
	{
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
		freq = EVE_Hal_rd32(phost, REG_FREQUENCY);
#endif
	}
	return freq;
}
EVE_HAL_EXPORT bool EVE_Util_configDefaultsEx(EVE_HalContext *phost, EVE_ConfigParameters *config, uint32_t width, uint32_t height, uint32_t refreshRate, uint32_t hsfWidth)
{
	uint32_t freq = getFreqOrPllOut(phost);
	bool res = configDefaultsEx(phost, config, width, height, refreshRate, hsfWidth, freq);
#ifdef _DEBUG
#ifdef EVE_SUPPORT_LVDS
	if (EVE_Hal_supportLvds(phost))
	{
		uint32_t pclkRes = (freq / config->PCLK) / 2; /* config->RefClkFreq should be same as freq */
		eve_printf_debug("Display refresh rate set to %f Hz using: ref clk %f MHz, txclkdiv+1 %i, LVDSTX clock %f MHz, ppc mul %i\n",
			(float)(((double)freq * (double)config->PixelsPerClock) / ((double)config->HCycle * (double)config->VCycle * (double)config->PCLK * 2.0)),
			(float)((double)freq * 0.000001), (int)config->PCLK, (float)((double)pclkRes * 0.000001), (int)config->PixelsPerClock);
	}
	else
#endif
	{
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
#if (EVE_SUPPORT_CHIPID >= EVE_BT817)
		if (config->PClkFreq)
		{
			uint32_t pclkFrequency = getFreqFromPClkFreq(config->PClkFreq);
			eve_printf_debug("Display refresh rate set to %f Hz using pclk %X freq %f MHz\n", (float)((double)pclkFrequency / ((double)config->HCycle * (double)config->VCycle)), (int)config->PClkFreq, (float)((double)pclkFrequency * 0.000001));
		}
		else
#endif
			eve_printf_debug("Display refresh rate set to %f Hz\n", (float)((double)freq / ((double)config->HCycle * (double)config->VCycle * (double)config->PCLK)));
#endif
	}
#endif
	return res;
}

EVE_HAL_EXPORT void EVE_Util_configDefaults(EVE_HalContext *phost, EVE_ConfigParameters *config, EVE_DISPLAY_T display)
{
	bool supportedResolution;
	uint32_t freq = getFreqOrPllOut(phost);
	uint32_t width;
	uint32_t height;
	uint32_t refreshRate;
	uint32_t hsfWidth;

	/* NOTE: Update `EVE_DISPLAY_T` in `EVE_Util.h` when adding display presets. */
	if (!display)
	{
		/* Default displays if none was explicitly chosen */
#if defined(DISPLAY_RESOLUTION_QVGA)
		display = EVE_DISPLAY_QVGA_320x240_56Hz;
#elif defined(DISPLAY_RESOLUTION_WQVGA)
		display = EVE_DISPLAY_WQVGA_480x272_60Hz;
#elif defined(DISPLAY_RESOLUTION_WQVGA_SQ)
		display = EVE_DISPLAY_WQVGA_480x480_48Hz;
#elif defined(DISPLAY_RESOLUTION_WVGA)
		display = EVE_DISPLAY_WVGA_800x480_74Hz;
#elif defined(DISPLAY_RESOLUTION_WVGA_NS)
		display = EVE_DISPLAY_WVGA_800x480_W860_60Hz;
#elif defined(DISPLAY_RESOLUTION_WSVGA)
		display = EVE_DISPLAY_WSVGA_1024x600_59Hz;
#elif defined(DISPLAY_RESOLUTION_HDTV)
		display = EVE_DISPLAY_HDTV_1280x720_58Hz;
#elif defined(DISPLAY_RESOLUTION_WXGA)
		display = EVE_DISPLAY_WXGA_1280x800_57Hz;
#elif defined(DISPLAY_RESOLUTION_FHD)
		display = EVE_DISPLAY_FHD_1920x1080_60Hz;
#elif defined(DISPLAY_RESOLUTION_WUXGA)
		display = EVE_DISPLAY_WUXGA_1920x1200_111Hz;
#elif defined(DISPLAY_RESOLUTION_HVGA_PORTRAIT)
		display = EVE_DISPLAY_HVGA_320x480_60Hz;
#else
		if (EVE_CHIPID >= EVE_BT820)
			display = EVE_DISPLAY_WUXGA_1920x1200_111Hz;
		else if (EVE_CHIPID >= EVE_BT817)
			display = EVE_DISPLAY_WXGA_1280x800_57Hz;
		else if (EVE_CHIPID >= EVE_BT815)
			display = EVE_DISPLAY_WVGA_800x480_74Hz;
		else if (EVE_CHIPID >= EVE_FT810)
			display = EVE_DISPLAY_WVGA_800x480_74Hz;
		else if (EVE_CHIPID >= EVE_FT800)
			display = EVE_DISPLAY_WQVGA_480x272_60Hz;
#endif
	}

	width = s_DisplayResolutions[display][0];
	height = s_DisplayResolutions[display][1];
#if EVE_DISPLAY_REFRESH
	refreshRate = EVE_DISPLAY_REFRESH;
#else
	refreshRate = s_DisplayResolutions[display][2];
#endif
	hsfWidth = s_DisplayResolutions[display][3];
	supportedResolution = configDefaultsEx(phost, config, width, height, refreshRate, hsfWidth, freq);

	/* Known values for specific display models */
#if EVE_DISPLAY_REFRESH
	if (EVE_DISPLAY_REFRESH != s_DisplayResolutions[display][2])
	{
		/* Don't use known preset values if the requested refresh rate differs from the default */
	}
	else
#endif
    if (display == EVE_DISPLAY_QVGA_320x240_56Hz && freq == 48000000)
	{
		/*
		FT800 known values:
		Resolution: 320x240
		Refresh rate: 55.916Hz
		*/
#if EVE_HARDCODED_DISPLAY_TIMINGS
		if (supportedResolution)
		{
			config->Width = 320;
			config->Height = 240;
			config->HCycle = 408;
			config->HOffset = 70;
			config->HSync0 = 0;
			config->HSync1 = 10;
			config->VCycle = 263;
			config->VOffset = 13;
			config->VSync0 = 0;
			config->VSync1 = 2;
			config->PCLK = 8;
#if EVE_SUPPORT_CHIPID_RANGE(EVE_BT817, EVE_BT820)
			config->PClkFreq = 0;
#endif
#ifdef EVE_SUPPORT_HSF
			config->HsfWidth = 0;
#endif
		}
#endif
		config->PCLKPol = 0;
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
		config->Swizzle = 2;
		config->CSpread = 1;
		config->Dither = 1;
#endif
	}
	else if (display == EVE_DISPLAY_WQVGA_480x272_60Hz && freq == 48000000)
	{
		/*
		FT800 known values:
		Display: LBL-T050BPH-01
		REG_FREQUENCY: 48MHz
		REG_PCLK: 5
		REG_HCYCLE: 548
		REG_VCYCLE: 292
		Resolution: 480x272
		Refresh rate: 59.994Hz
		*/
#if EVE_HARDCODED_DISPLAY_TIMINGS
		if (supportedResolution)
		{
			config->Width = 480;
			config->Height = 272;
			config->HCycle = 548;
			config->HOffset = 43;
			config->HSync0 = 0;
			config->HSync1 = 41;
			config->VCycle = 292;
			config->VOffset = 12;
			config->VSync0 = 0;
			config->VSync1 = 10;
			config->PCLK = 5;
#if EVE_SUPPORT_CHIPID_RANGE(EVE_BT817, EVE_BT820)
			config->PClkFreq = 0;
#endif
#ifdef EVE_SUPPORT_HSF
			config->HsfWidth = 0;
#endif
		}
#endif
	}
	else if (display == EVE_DISPLAY_WQVGA_480x480_48Hz && freq == 48000000) // REVIEW: ENABLE_IDP_4000_04A_RP2040 and freq
	{
		/*
		FT800 known values:
		Display: LBL-T050BPH-01
		REG_FREQUENCY: 48MHz
		REG_PCLK: 7
		REG_HCYCLE: 548
		REG_VCYCLE: 292
		Resolution: 480x480
		Refresh rate: 59.994Hz
		*/
#if EVE_HARDCODED_DISPLAY_TIMINGS
#if defined (ENABLE_IDP_4000_04A_RP2040)
		config->Width = 480;
		config->Height = 480;
		config->HCycle = 540;
		config->HOffset = 50;
		config->HSync0 = 0;
		config->HSync1 = 8;
		config->VCycle = 520;
		config->VOffset = 10;
		config->VSync0 = 0;
		config->VSync1 = 8;
		config->PCLK = 4;
		config->Swizzle = 0;

#else
		if (supportedResolution)
		{
			config->Width = 480;
			config->Height = 480;
			config->HCycle = 548;
			config->HOffset = 43;
			config->HSync0 = 0;
			config->HSync1 = 41;
			config->VCycle = 512;
			config->VOffset = 12;
			config->VSync0 = 0;
			config->VSync1 = 10;
			config->PCLK = 7;
#if EVE_SUPPORT_CHIPID_RANGE(EVE_BT817, EVE_BT820)
			config->PClkFreq = 0;
#endif
#ifdef EVE_SUPPORT_HSF
			config->HsfWidth = 0;
#endif
		}
#endif		
#endif
	}
#ifdef EVE_GRAPHICS_IDP_3500_04A
	else if (display == EVE_DISPLAY_HVGA_320x480_60Hz)
	{
		/*
		Resolution: 320x480
		*/

		if (supportedResolution)
		{
			config->Width = 320;
			config->Height = 480;
			config->HCycle = 400;
			config->HOffset = 40;
			config->HSync0 = 0;
			config->HSync1 = 40;
			config->VCycle = 500;
			config->VOffset = 10;
			config->VSync0 = 0;
			config->VSync1 = 5;
			config->PCLK = 5;
		}
		config->Swizzle = 0;
		config->PCLKPol = 0;
		config->CSpread = 1;
		config->Dither = 1;
	}
#endif
#if (EVE_SUPPORT_CHIPID >= EVE_BT817) && (EVE_SUPPORT_CHIPID < EVE_BT820 || defined(EVE_MULTI_GRAPHICS_TARGET))
	else if (display == EVE_DISPLAY_WQVGA_480x272_60Hz && EVE_CHIPID >= EVE_BT817 && EVE_CHIPID < EVE_BT820)
	{
		/* Same as above, but using PClkFreq */
#if EVE_HARDCODED_DISPLAY_TIMINGS
		if (supportedResolution)
		{
			config->Width = 480;
			config->Height = 272;
			config->HCycle = 548;
			config->HOffset = 43;
			config->HSync0 = 0;
			config->HSync1 = 41;
			config->VCycle = 292;
			config->VOffset = 12;
			config->VSync0 = 0;
			config->VSync1 = 10;
			config->PCLK = 1;
			config->PClkFreq = 0x885; // 9.6MHz
#ifdef EVE_SUPPORT_HSF
			config->HsfWidth = 0;
#endif
		}
#endif
	}
#endif
	else if (display == EVE_DISPLAY_WVGA_800x480_74Hz && (freq == 72000000 || freq == 576000000))
	{
		/*
		BT81X known values:
		Display: MTF070TN83-V1
		REG_FREQUENCY: 72MHz
		Resolution: 800x480
		Refresh rate: 73.892Hz
		BT82X test output hardcoded reference timings:
		> Display refresh rate set to 73.891624 Hz
		> ref clk 576.000000 MHz, txclkdiv+1 8, LVDSTX clock 36.000000 MHz, ppc mul 1
		BT82X test output without hardcoded timings:
		> Display refresh rate set to 74.030205 Hz
		> ref clk 576.000000 MHz, txclkdiv+1 8, LVDSTX clock 36.000000 MHz, ppc mul 1
		*/
#if EVE_HARDCODED_DISPLAY_TIMINGS
		if (supportedResolution)
		{
			config->Width = 800;
			config->Height = 480;
			config->HCycle = 928;
			config->HOffset = 88;
			config->HSync0 = 0;
			config->HSync1 = 48;
			config->VCycle = 525;
			config->VOffset = 32;
			config->VSync0 = 0;
			config->VSync1 = 3;
			config->PCLK = (freq == 576000000) ? 8 : 2;
#if EVE_SUPPORT_CHIPID_RANGE(EVE_BT817, EVE_BT820)
			config->PClkFreq = 0;
#endif
#ifdef EVE_SUPPORT_HSF
			config->HsfWidth = 0;
#endif
#ifdef EVE_SUPPORT_LVDS
			config->PixelsPerClock = 1;
#endif
		}
#endif
	}
	else if (display == EVE_DISPLAY_HDTV_1280x720_58Hz && (freq == 72000000 || freq == 576000000))
	{
		/*
		BT815 known values:
		REG_FREQUENCY: 72MHz
		Resolution: 1280x720
		Refresh rate: 58.182Hz
		BT820 with hardcoded timings:
		> Display refresh rate set to 58.181820 Hz
		> ref clk 576.000000 MHz, txclkdiv+1 4, LVDSTX clock 72.000000 MHz, ppc mul 1
		> ref clk 576.000000 MHz, txclkdiv+1 8, LVDSTX clock 36.000000 MHz, ppc mul 2
		BT820 without hardcoded timings:
		> Display refresh rate set to 58.034657 Hz
		> ref clk 576.000000 MHz, txclkdiv+1 8, LVDSTX clock 36.000000 MHz, ppc mul 2
		*/
#if EVE_HARDCODED_DISPLAY_TIMINGS
		if (supportedResolution)
		{
			config->Width = 1280;
			config->Height = 720;
			config->HCycle = 1650;
			config->HOffset = 40 + 220;
			config->HSync0 = 0;
			config->HSync1 = 40;
			config->VCycle = 750;
			config->VOffset = 5 + 20;
			config->VSync0 = 0;
			config->VSync1 = 5;
			config->PCLK = (freq == 576000000) ? 8 : 1; /* lvds tx clk div on BT820 */
#if EVE_SUPPORT_CHIPID_RANGE(EVE_BT817, EVE_BT820)
			config->PClkFreq = 0;
#endif
#ifdef EVE_SUPPORT_HSF
			config->HsfWidth = 0;
#endif
#ifdef EVE_SUPPORT_LVDS
			config->PixelsPerClock = 2;
#endif
		}
#endif
		config->PCLKPol = 0;
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
		config->CSpread = 0;
		config->Dither = 0;
		config->OutBitsR = 0;
		config->OutBitsG = 0;
		config->OutBitsB = 0;
#endif
	}
	else if (display == EVE_DISPLAY_WXGA_1280x800_57Hz && freq == 72000000)
	{
		/*
		BT817 known values:
		Resolution: 1280x800
		Refresh rate: 57.393Hz
		*/
#if EVE_SUPPORT_CHIPID_RANGE(EVE_BT817, EVE_BT820)
#if EVE_HARDCODED_DISPLAY_TIMINGS
#if defined(EVE_GRAPHICS_IDP_1000_04A) /* FIXME: This should probably have its own EVE_DISPLAY_WXGA_1280x800_57Hz entry instead, as it is a specific display and probably not 57Hz */
		if (EVE_CHIPID >= EVE_BT817 && EVE_CHIPID)
		{
			supportedResolution = true; /* Force true, known good settings */
		}
#endif
		if (EVE_CHIPID >= EVE_BT817 && EVE_CHIPID < EVE_BT820 && supportedResolution)
		{
#if defined(EVE_GRAPHICS_IDP_1000_04A)
			config->Width = 1280;
			config->Height = 800;
			config->HCycle = 1440;
			config->HOffset = 160;
			config->HSync0 = 0;
			config->HSync1 = 2;
			config->VCycle = 822;
			config->VOffset = 15;
			config->VSync0 = 0;
			config->VSync1 = 2;
			config->PCLK = 1;
			config->PClkFreq = 0x8C1; 
			config->Dither = 1;
#else
			config->Width = 1280;
			config->Height = 800;
			config->HCycle = 1411;
			config->HOffset = 120;
			config->HSync0 = 0;
			config->HSync1 = 100;
			config->VCycle = 815;
			config->VOffset = 14;
			config->VSync0 = 0;
			config->VSync1 = 10;
			config->PCLK = 1;
			config->PClkFreq = 0x8B1; // 66MHz
			config->Dither = 0;
#endif
#ifdef EVE_SUPPORT_HSF
			config->HsfWidth = 0;
#endif
		}
#endif
#endif
		config->PCLKPol = 0;
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
		config->CSpread = 0;
		config->OutBitsR = 0;
		config->OutBitsG = 0;
		config->OutBitsB = 0;
#endif
	}
	else if (display == EVE_DISPLAY_WSVGA_1024x600_59Hz && freq == 72000000)
	{
		/*
		BT817 known values:
		Resolution: 1024x600
		Refresh rate: 59.758Hz
		*/
#if EVE_SUPPORT_CHIPID_RANGE(EVE_BT817, EVE_BT820)
#if EVE_HARDCODED_DISPLAY_TIMINGS
		if (EVE_CHIPID >= EVE_BT817 && EVE_CHIPID < EVE_BT820 && supportedResolution)
		{
			config->Width = 1024;
			config->Height = 600;
			config->HCycle = 1344;
			config->HOffset = 160;
			config->HSync0 = 0;
			config->HSync1 = 100;
			config->VCycle = 635;
			config->VOffset = 23;
			config->VSync0 = 0;
			config->VSync1 = 10;
			config->PCLK = 1;
			config->PClkFreq = 0xD12; // 51Mhz
#ifdef EVE_SUPPORT_HSF
			config->HsfWidth = 0;
#endif
		}
#endif
#endif
		config->PCLKPol = 1;
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
		config->CSpread = 0;
		config->Dither = 1;
		config->OutBitsR = 0;
		config->OutBitsG = 0;
		config->OutBitsB = 0;
#endif
	}
	else if (display == EVE_DISPLAY_FHD_1920x1080_60Hz)
	{
		config->PCLKPol = 0;
	}
	else if (display == EVE_DISPLAY_WUXGA_1920x1200_90Hz)
	{
		config->PCLKPol = 0;
	}
	else if (display == EVE_DISPLAY_WUXGA_1920x1200_111Hz)
	{
		/*
		BT820 known values:		REG_FREQUENCY: 72MHz
		Resolution: 1920x1200
		Refresh rate: 111Hz
		*/
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
#if EVE_HARDCODED_DISPLAY_TIMINGS
		if (EVE_CHIPID >= EVE_BT820 && freq == 576000000 && supportedResolution)
		{
			config->Width = 1920;
			config->Height = 1200;
			config->HCycle = 2080;
			config->HOffset = 50;
			config->HSync0 = 0;
			config->HSync1 = 30;
			config->VCycle = 1244;
			config->VOffset = 10;
			config->VSync0 = 0;
			config->VSync1 = 3;
			config->PCLK = (freq == 576000000) ? 4 : 1; /* lvds tx clk div on BT820 */
#ifdef EVE_SUPPORT_HSF
			config->HsfWidth = 0;
#endif
#ifdef EVE_SUPPORT_LVDS
			config->PixelsPerClock = 4;
#endif
		}
#endif
#endif
		config->PCLKPol = 0;
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
		config->CSpread = 0;
		config->Dither = 1;
		config->OutBitsR = 0;
		config->OutBitsG = 0;
		config->OutBitsB = 0;
#endif
	}

#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
#if (EVE_SUPPORT_CHIPID >= EVE_BT817)
	/* Disable max cycle if incompatible hardcoded timings are used */
	if (config->AhHCycleMax && config->HCycle > config->AhHCycleMax)
		config->AhHCycleMax = 0;
#endif
#endif

#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
	config->SwapchainFormat = RGB8;
#endif

#ifdef _DEBUG
#ifdef EVE_SUPPORT_LVDS
	if (EVE_Hal_supportLvds(phost))
	{
		uint32_t pclkRes = (freq / config->PCLK) / 2; /* config->RefClkFreq should be same as freq */
		eve_printf_debug("Display refresh rate set to %f Hz using: ref clk %f MHz, txclkdiv+1 %i, LVDSTX clock %f MHz, ppc mul %i\n",
			(float)(((double)freq * (double)config->PixelsPerClock) / ((double)config->HCycle * (double)config->VCycle * (double)config->PCLK * 2.0)),
			(float)((double)freq * 0.000001), (int)config->PCLK, (float)((double)pclkRes * 0.000001), (int)config->PixelsPerClock);
	}
	else
#endif
	{
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
#if (EVE_SUPPORT_CHIPID >= EVE_BT817)
		if (config->PClkFreq)
		{
			uint32_t pclkFrequency = getFreqFromPClkFreq(config->PClkFreq);
			eve_printf_debug("Display refresh rate set to %f Hz using pclk %X freq %f MHz\n", (float)((double)pclkFrequency / ((double)config->HCycle * (double)config->VCycle)), (int)config->PClkFreq, (float)((double)pclkFrequency * 0.000001));
		}
		else
#endif
			eve_printf_debug("Display refresh rate set to %f Hz\n", (float)((double)freq / ((double)config->HCycle * (double)config->VCycle * (double)config->PCLK)));
#endif
	}
#endif

#if 0	
	eve_printf_debug("Width: %i\n", (int)config->Width);
	eve_printf_debug("Height: %i\n", (int)config->Height);
	eve_printf_debug("HCycle: %i\n", (int)config->HCycle);
	eve_printf_debug("HOffset: %i\n", (int)config->HOffset);
	eve_printf_debug("HSync0: %i\n", (int)config->HSync0);
	eve_printf_debug("HSync1: %i\n", (int)config->HSync1);
	eve_printf_debug("VCycle: %i\n", (int)config->VCycle);
	eve_printf_debug("VOffset: %i\n", (int)config->VOffset);
	eve_printf_debug("VSync0: %i\n", (int)config->VSync0);
	eve_printf_debug("VSync1: %i\n", (int)config->VSync1);
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
	eve_printf_debug("PCLK: %i\n", (int)config->PCLK);
	eve_printf_debug("Swizzle: %i\n", (int)config->Swizzle);
#endif
	eve_printf_debug("PCLKPol: %i\n", (int)config->PCLKPol);
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
	eve_printf_debug("CSpread: %i\n", (int)config->CSpread);
	eve_printf_debug("OutBitsR: %i\n", (int)config->OutBitsR);
	eve_printf_debug("OutBitsG: %i\n", (int)config->OutBitsG);
	eve_printf_debug("OutBitsB: %i\n", (int)config->OutBitsB);
	eve_printf_debug("Dither: %i\n", (int)config->Dither);
#if (EVE_SUPPORT_CHIPID >= EVE_BT815)
	eve_printf_debug("AdaptiveFramerate: %i\n", (int)config->AdaptiveFramerate);
#endif
#if (EVE_SUPPORT_CHIPID >= EVE_BT817)
	eve_printf_debug("PClkFreq: %X\n", (int)config->PClkFreq);
	eve_printf_debug("AhHCycleMax: %i\n", (int)config->AhHCycleMax);
#endif
#ifdef EVE_SUPPORT_HSF
	eve_printf_debug("HsfWidth: %i\n", (int)config->HsfWidth);
#endif
#endif
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
	eve_printf_debug("SwapchainFormat: %i\n", (int)config->SwapchainFormat);
#endif
#endif  
}

#define EXTRACT_CHIPID(romChipId) EVE_extendedChipId((((romChipId) >> 8) & 0xFF) | (((romChipId) & (0xFF)) << 8))

/**
* @brief Validates a boot status code format
* 
* @param bootStatus The 32-bit boot status code to validate
* @return true if format is valid, false otherwise
*/
bool EVE_Util_isValidBootStatus(uint32_t bootStatus)
{
	// First character is uppercase, second any alpha or period
	// Third and last characters are numeric or period
	const uint8_t highLevel = bootStatus >> 24;
	if (!(highLevel >= 'A' && highLevel <= 'Z')) return false;

	// Check second byte (alpha or period)
	const uint8_t secondByte = (bootStatus >> 16) & 0xFF;
	if (!((secondByte >= 'A' && secondByte <= 'Z') || 
		(secondByte >= 'a' && secondByte <= 'z') ||
		secondByte == '.')) return false;

	// Check third byte (numeric or period)
	const uint8_t thirdByte = (bootStatus >> 8) & 0xFF;
	if (!((thirdByte >= '0' && thirdByte <= '9') ||
		thirdByte == '.')) return false;

	// Check fourth byte (numeric or period)
	const uint8_t lastByte = bootStatus & 0xFF;
	if (!((lastByte >= '0' && lastByte <= '9') ||
		lastByte == '.')) return false;

	return true;
}

/**
 * @brief
 *
 * @param phost Pointer to Hal context
 * @param bootup
 * @return true True if successful
 * @return false False if error
 */
EVE_HAL_EXPORT bool EVE_Util_bootup(EVE_HalContext *phost, EVE_BootupParameters *bootup)
{
	/* IMPORTANT: Do not use EVE_CoCmd functions here, as they can be overridden by hooks */

	const uint32_t expectedChipId = EVE_CHIPID;
	uint8_t engineStatus;
	uint32_t chipId;
	uint8_t id;
	uint32_t freq;

#if EVE_SUPPORT_CHIPID_FROM(EVE_BT820)
	uint32_t sysPll; // this is using 'double' in reference code?
	uint32_t freqByte;

	EVE_CMD_SYS_CLK_DIV sysClkDiv = { 0 };
	EVE_CMD_BOOT_CFG bootCfg = { 0 };
	EVE_CMD_BOOT_CFG_EN bootCfgEn = { 0 };
	EVE_CMD_DDR_TYPE ddrType = { 0 };
	EVE_CMD_GPREG gpReg = { 0 };
#endif

#if (defined(_WIN32) || defined(__linux__))
	int totalTries = -1;
#endif
	do
	{
		int tries = 0;
#if (defined(_WIN32) || defined(__linux__))
		if (bootup->StatusResult)
		{
			*bootup->StatusResult = EVE_BOOTUP_STATUS_FAILED;
			if (bootup->Resume != EVE_BOOTUP_RESUME_CONTINUE && totalTries != -1)
			{
				if (totalTries >= 100)
				{
					*bootup->StatusResult = EVE_BOOTUP_STATUS_PENDING;
					return false;
				}
				++totalTries;
			}
		}
#endif

		/* EVE will be in SPI Single channel after POR */
		if (!EVE_Hal_powerCycle(phost, true))
		{
#ifdef EVE_MULTI_GRAPHICS_TARGET
			/* Restore expected */
			phost->ChipId = expectedChipId;
#endif
			return false;
		}

		/* Host commands for pre-BT820 generation.
		On BT820 when multi graphics target is used, these are hopefully ignored,
		since they are only 3 bytes which is invalid in the BT820 SPI protocol.
		Conversely, BT820 host commands start with 0xFF, which is invalid
		in the previous generation SPI protocol. */

#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
#ifdef EVE_MULTI_GRAPHICS_TARGET
		/* Enable FT8XX series host commands */
		phost->ChipId = EVE_FT810;
#endif
		if (bootup->ExternalOsc)
		{
			/* Set the clk to external clock. Must disable it when no external clock source on the board */
			EVE_Hal_hostCommand(phost, EVE_EXTERNAL_OSC);
			EVE_sleep(10);
		}
		else
		{
			/* Send CLKINT explicitly to play safe for all EVE series chip*/
			EVE_Hal_hostCommand(phost, EVE_INTERNAL_OSC);
			EVE_sleep(10);
		}
		/* Update system clock as per user selected */
		if (bootup->SystemClock)
		{
			EVE_Host_selectSysClk(phost, bootup->SystemClock);
		}

		EVE_Host_coreReset(phost);

#ifndef EVE_MULTI_GRAPHICS_TARGET
		/* Access address 0 to wake up the FT800, this transfers 3 zeroes.
		Under multi target we can skip this, since the effect of 5 zeroes should be the same */
		EVE_Hal_hostCommand(phost, EVE_ACTIVE_M);
#endif
#endif
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
#ifdef EVE_MULTI_GRAPHICS_TARGET
		/* Enable BT82X series host commands */
		phost->ChipId = EVE_BT820;
#endif
		//EVE_Hal_hostCommand5(phost, 0xE9, 0xC1, 0x00, 0x00);
		//EVE_Hal_hostCommand5(phost, 0xE8, 0xE0, 0x00, 0x00);
		//EVE_Hal_hostCommand5(phost, 0xEB, 0x00, 0x00, 0x00);
													  
		//EVE_Hal_hostCommand5(phost, 0xE9, 0xC0, 0x00, 0x00);
		//EVE_Hal_hostCommand5(phost, 0xE6, 0x17, 0x00, 0x00);

#if 0
		EVE_Hal_hostCommand5(phost, 0xE9, 0xC1, 0x00, 0x00);
		// EVE_Hal_hostCommand5(phost, 0xE8, 0xE0, 0x00, 0x00); // f0
		// EVE_Hal_hostCommand5(phost, 0xEB, 0x00, 0x00, 0x00); // 0a
		EVE_Hal_hostCommand5(phost, 0xE8, 0xF0, 0x00, 0x00);
		EVE_Hal_hostCommand5(phost, 0xEB, 0x08, 0x00, 0x00);
		// EVE_Hal_hostCommand5(phost, 0xEC, 0x00, 0x00, 0x00);
		EVE_Hal_hostCommand5(phost, 0xE9, 0xC0, 0x00, 0x00);
		EVE_Hal_hostCommand5(phost, 0xE6, 0x17, 0x00, 0x00);
#else
		/* REVIEW: Check if this matches the above settings!!! */
		bootCfgEn.boot = 1;
		bootCfgEn.ddrType = 1;
		bootCfgEn.gpReg = 0;
		bootCfgEn.enable = 1;
		EVE_Host_bootCfgEn(phost, bootCfgEn);
		bootCfg.ddr = 1;
		bootCfg.jt = 1;
		bootCfg.audio = 1;
		bootCfg.watchdog = 1;
		bootCfg.source = 0;
		EVE_Host_setBootCfg(phost, bootCfg); // REG_BOOT_CONTROL settings: DDR(0) JT(1) AUD(1) WD(0) reserved source(0)
		ddrType.speed = 0; // 1333
		ddrType.type = 1;
#if defined(EVE_GRAPHICS_VM820C)
		ddrType.size = 0; //128M Bytes for VM820C
#else
		ddrType.size = 2;
#endif
		EVE_Host_setDdrType(phost, ddrType);
		gpReg.mode = 0;
		EVE_Host_setGpReg(phost, gpReg); // SPI Master slow mode(0)
		bootCfgEn.boot = 1;
		bootCfgEn.ddrType = 1;
		bootCfgEn.gpReg = 0;
		bootCfgEn.enable = 0;
		EVE_Host_bootCfgEn(phost, bootCfgEn); // enable(0)
		sysPll = 576000000UL;
		freqByte = (uint32_t)((sysPll / EVE_FREQUENCY_DEFAULT) - 1); // TODO: Does this need to take bootConfig system clock option into account??? also EVE_SYSTEM_CLOCK
		sysClkDiv.sysPllCps = 1; // default b'001'
		sysClkDiv.freq = freqByte; // system clock: 72MHz or 36MHz
		EVE_Host_setClkDiv(phost, sysClkDiv);
#endif
	
		/* Transfer 5 zeroes to wake up BT820 */
		EVE_Hal_hostCommand5(phost, EVE_82X_ACTIVE, 0, 0, 0);
#endif
		EVE_sleep(300);
		
		/* Wait for valid chip ID */
		chipId = EVE_Hal_rd32(phost, ROM_CHIPID);
		while (EXTRACT_CHIPID(chipId) < EVE_FT800
		    || EXTRACT_CHIPID(chipId) > EVE_BT820)
		{
			eve_printf_debug("EVE ROM_CHIPID after wake up %lx\n", (unsigned long)chipId);
#if (defined(_WIN32) || defined(__linux__))
			if (totalTries == -1)
				totalTries = 0;
#endif

			++tries;
			EVE_sleep(20);
			if (phost->CbCmdWait && !phost->CbCmdWait(phost))
			{
#ifdef EVE_MULTI_GRAPHICS_TARGET
				/* Restore expected */
				phost->ChipId = expectedChipId;
#endif
				return false;
			}

#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
			// If we can read a valid boot status, assume BT820 - ROM_CHIPID is only set once the coprocessor is initialized
#if defined(EVE_MULTI_GRAPHICS_TARGET)
			const uint32_t bootStatus = phost->ChipId == EVE_BT820 ? EVE_Hal_rd32(phost, BOOT_STATUS) : 0;
			const bool validBootStatus = phost->ChipId == EVE_BT820 && EVE_Util_isValidBootStatus(bootStatus);
#else
			const uint32_t bootStatus = EVE_Hal_rd32(phost, BOOT_STATUS);
			const bool validBootStatus = EVE_Util_isValidBootStatus(bootStatus);
#endif
			eve_printf_debug("EVE BOOT_STATUS after wake up %lx\n", (unsigned long)bootStatus);
			if (validBootStatus)
				EVE_sleep(300); // Sleep longer, may be running emulator in debug mode
#endif

#if defined(EVE_MULTI_GRAPHICS_TARGET) && (EVE_SUPPORT_CHIPID >= EVE_BT820)
			/* Swap between BT820 and older to switch SPI protocol in backend */
			if (!validBootStatus && phost->ChipId == EVE_BT820)
				phost->ChipId = EVE_FT810;
			else
				phost->ChipId = EVE_BT820;
#endif

			chipId = EVE_Hal_rd32(phost, ROM_CHIPID);
																		   

#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
#ifdef EVE_MULTI_TARGET
			/* Turn off external clock if chipId reads incorrectly for 4 times */
			/* When not targeting multiple boards, set the EVE_USE_INTERNAL_OSC preprocessor variable, or configure the ExternalOsc parameter at runtime */
			if (
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
				!validBootStatus && 
#endif
				tries >= 4)
			{
#ifdef EVE_MULTI_GRAPHICS_TARGET
				/* Enable FT8XX series host commands */
				phost->ChipId = EVE_FT810;
#endif

				bootup->ExternalOsc = !bootup->ExternalOsc;
				eve_printf_debug("Reattempting bootup with ExternalOsc changed from %s to %s\n", bootup->ExternalOsc ? "OFF" : "ON", bootup->ExternalOsc ? "ON" : "OFF");

				EVE_Host_powerModeSwitch(phost, EVE_SLEEP_M); /* Put EVE into Sleep mode, prepare for clock selection */
				chipId = 0; // In case chipId is not zero
				break;
			}
#endif
#endif
		}
	} while (!chipId);
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
	phost->ReadTimeoutOccured = false;
#endif

	/* Validate chip ID to ensure the correct HAL is used */
	/* ROM_CHIPID is valid across all EVE devices */
	if (expectedChipId && EXTRACT_CHIPID(chipId) != expectedChipId)
		eve_printf_debug("Mismatching EVE chip id %lx, expect model %lx\n", (unsigned long)((chipId >> 8) & 0xFF) | ((chipId & 0xFF) << 8), (unsigned long)expectedChipId);
	eve_printf_debug("EVE chip id %lx %lx.%lx (EVE gen %i)\n", (unsigned long)EVE_shortChipId(EXTRACT_CHIPID(chipId)), (unsigned long)((chipId >> 16) & 0xFF), (unsigned long)((chipId >> 24) & 0xFF), EVE_gen(EXTRACT_CHIPID(chipId)));

	/* Switch to the proper chip ID if applicable */
#ifdef EVE_MULTI_GRAPHICS_TARGET
	phost->ChipId = EXTRACT_CHIPID(chipId);
	if (phost->ChipId >= EVE_BT820)
		phost->GpuDefs = &EVE_GpuDefs_BT82X;
	else if (phost->ChipId >= EVE_BT815)
		phost->GpuDefs = &EVE_GpuDefs_BT81X;
	else if (phost->ChipId >= EVE_BT880)
		phost->GpuDefs = &EVE_GpuDefs_BT88X;
	else if (phost->ChipId >= EVE_FT810)
		phost->GpuDefs = &EVE_GpuDefs_FT81X;
	else if (phost->ChipId >= EVE_FT800)
		phost->GpuDefs = &EVE_GpuDefs_FT80X;
#endif

	/* Turn off previous audio playback (in case powerdown is not connected) */
	EVE_Hal_wr32(phost, REG_PLAYBACK_LENGTH, 0);

#ifdef EVE_MULTI_GRAPHICS_TARGET
	/* The default clock configuration is only set when the graphics target is known at compile time */
	if (!bootup->SystemClock)
	{
		if (phost->ChipId >= EVE_BT820)
		{
			/* Assume the hardware default */
			bootup->SystemClock = EVE_SYSCLK_72M;
		}
		else if (phost->ChipId >= EVE_BT815)
		{
			/* No clock was set, but we want 72MHz default for BT81X and up */
			eve_printf_debug("Set system clock PLL to 72MHz\n");
			bootup->SystemClock = EVE_SYSCLK_72M;

			/* Sleep */
			EVE_Hal_hostCommand(phost, EVE_SLEEP_M);
			EVE_sleep(300);

			/* Update system clock */
			EVE_Host_selectSysClk(phost, bootup->SystemClock);

			/* Access address 0 to wake up the FT800 */
			EVE_Hal_hostCommand(phost, EVE_ACTIVE_M);
			EVE_sleep(300);
		}
		else if (phost->ChipId >= EVE_FT810)
		{
			/* Assume the default */
			bootup->SystemClock = EVE_SYSCLK_60M;
		}
		if (EVE_HOST == EVE_HOST_FT4222 && phost->ChipId >= EVE_FT810)
		{
			eve_printf_debug("Prefer Quad for FT4222\n");
			bootup->SpiChannels = EVE_SPI_QUAD_CHANNEL;
			bootup->SpiDummyBytes = 2;
		}
	}
#endif

	/* Read Register ID to check if EVE is ready. */
	while ((id = EVE_Hal_rd8(phost, REG_ID)) != 0x7C)
	{
		eve_printf_debug("EVE register ID after wake up %x\n", (unsigned int)id);

		EVE_sleep(20);
		if (phost->CbCmdWait && !phost->CbCmdWait(phost))
			return false;
	}
	eve_printf_debug("EVE register ID after wake up %x\n", (unsigned int)id);
	eve_assert(chipId == EVE_Hal_rd32(phost, ROM_CHIPID));

	/* Read REG_CPURESET to check if engines are ready.
	    Bit 0 for coprocessor engine,
	    Bit 1 for touch engine,
	    Bit 2 for audio engine.
	*/
	while ((engineStatus = EVE_Hal_rd8(phost, REG_CPURESET)) != 0x00)
	{
		if (engineStatus & 0x01)
		{
			eve_printf_debug("Coprocessor engine is not ready\n");
		}
		if (engineStatus & 0x02)
		{
			eve_printf_debug("Touch engine is not ready\n");
		}
		if (engineStatus & 0x04)
		{
			eve_printf_debug("Audio engine is not ready\n");
		}

		if (EVE_HOST == EVE_HOST_BT8XXEMU && engineStatus == 0x02)
		{
			// FIXME: Emulator behaviour of REG_CPURESET and touch engine
			break;
		}

		EVE_sleep(20);
		if (phost->CbCmdWait && !phost->CbCmdWait(phost))
			return false;
	}
	eve_printf_debug("All engines are ready\n");

	/* Update REG_FREQUENCY as per user selected */
	if (bootup->SystemClock != EVE_SYSCLK_DEFAULT)
	{
		uint32_t clockMHz = bootup->SystemClock * 12;
		freq = clockMHz * 1000 * 1000;
		EVE_Hal_wr32(phost, REG_FREQUENCY, freq);
		EVE_Hal_flush(phost);
		eve_printf_debug("EVE clock frequency set to %d MHz\n", (unsigned int)clockMHz);
	}
	else
	{
		freq = EVE_Hal_rd32(phost, REG_FREQUENCY);
		eve_printf_debug("EVE default clock is %d MHz\n", (unsigned int)(freq / 1000000));
	}

	/* Switch to configured default SPI channel mode */
#if (EVE_SUPPORT_CHIPID >= EVE_FT810)
	if (EVE_CHIPID >= EVE_FT810)
	{
		EVE_Hal_setSPI(phost, bootup->SpiChannels, bootup->SpiDummyBytes);
#ifdef _DEBUG
		const char *spiChannels = (phost->SpiChannels == EVE_SPI_QUAD_CHANNEL)
		    ? "Quad"
		    : ((phost->SpiChannels == EVE_SPI_DUAL_CHANNEL) ? "Dual" : "Single");
		const char *requested = (bootup->SpiChannels == EVE_SPI_QUAD_CHANNEL)
		    ? "Quad"
		    : ((bootup->SpiChannels == EVE_SPI_DUAL_CHANNEL) ? "Dual" : "Single");
		if (bootup->SpiChannels == phost->SpiChannels)
			eve_printf_debug("%s channel SPI\n", spiChannels);
		else
			eve_printf_debug("%s channel SPI (%s unsupported)\n", spiChannels, requested);
#endif
	}
#endif

	/* Sanity check after SPI change */
	if (EVE_Hal_rd32(phost, REG_FREQUENCY) != freq)
	{
		eve_printf_debug("SPI channel configuration change failed\n");
#if (EVE_SUPPORT_CHIPID >= EVE_FT810)
		if (EVE_CHIPID >= EVE_FT810)
		{
			if (bootup->SpiChannels > EVE_SPI_SINGLE_CHANNEL)
			{
				/* Try again with dual channel */
				--bootup->SpiChannels;
				eve_printf_debug("Retry with %s channel SPI\n", bootup->SpiChannels ? "Dual" : "Single");
				return EVE_Util_bootup(phost, bootup);
			}
		}
#endif
		return false;
	}

	/* In some cases, for example, QSPI on MM817EV board with MM930LITE,
	   the drive strength is required to adjust for better signal integrity.
	   Setting to max current 20mA here is to play safe.  Users may adjust the
	   corresponding bits of REG_GPIOX according to their own situation. */
#if (EVE_SUPPORT_CHIPID >= EVE_BT820 || defined(EVE_MULTI_GRAPHICS_TARGET))
	if (EVE_CHIPID >= EVE_BT820)
	{
		/* TODO_BT820: Verify if this is needed (also verify on other EVE gens) */
		// EVE_Hal_wr32(phost, REG_GPIO_DIR, 0xffffffff);
		// EVE_Hal_wr32(phost, REG_GPIO, 0xffffffff);
	}
	else
#endif
	{
#if (EVE_SUPPORT_CHIPID < EVE_BT820 || defined(EVE_MULTI_GRAPHICS_TARGET))
		if (EVE_CHIPID >= EVE_FT810)
		{
			EVE_Hal_wr16(phost, REG_GPIOX_DIR, 0xffff);
			EVE_Hal_wr16(phost, REG_GPIOX, 0xffff);
		}
		else
		{
			EVE_Hal_wr8(phost, REG_GPIO_DIR, 0xff);
			EVE_Hal_wr8(phost, REG_GPIO, 0xff);
		}
#endif
	}

	/* Update touch firmware */
#if EVE_SUPPORT_CHIPID_SPECIFIC(EVE_FT811) || EVE_SUPPORT_CHIPID_SPECIFIC(EVE_FT813)
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 6285)
#endif
	if ((EVE_CHIPID == EVE_FT811 || EVE_CHIPID == EVE_FT813) && (EVE_HOST != EVE_HOST_BT8XXEMU))
#ifdef _MSC_VER
#pragma warning(pop)
#endif
	{
#if defined(PANL70) || defined(PANL70PLUS)
		EVE_Hal_wr8(phost, REG_CPURESET, 2);
		EVE_Hal_wr16(phost, REG_TOUCH_CONFIG, 0x05d0);
#endif
		/* Download new firmware to fix pen up issue */
		/* It may cause resistive touch not working any more*/
		uploadTouchFirmware(phost);
#if defined(PANL70) || defined(PANL70PLUS)
		EVE_UtilImpl_bootupDisplayGpio(phost);
#endif
		EVE_Hal_flush(phost);
		EVE_sleep(100);
	}
#endif

#if EVE_SUPPORT_CHIPID_SPECIFIC(EVE_FT800)
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 6285)
#endif
	if ((EVE_CHIPID == EVE_FT800) && (EVE_HOST != EVE_HOST_BT8XXEMU))
#ifdef _MSC_VER
#pragma warning(pop)
#endif
	{
		uploadTouchFirmware(phost);
		EVE_Hal_flush(phost);
		EVE_sleep(100);
	}
#endif

#if EVE_SUPPORT_CHIPID_RANGE(EVE_BT815, EVE_BT820)
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 6285)
#endif
	if ((EVE_CHIPID == EVE_BT815 || EVE_CHIPID == EVE_BT817) && (EVE_HOST != EVE_HOST_BT8XXEMU))
#ifdef _MSC_VER
#pragma warning(pop)
#endif
	{
#if defined(DISPLAY_RESOLUTION_WXGA)
		// Assume WXGA is using Goodix touch IC
		// To be fixed: use macro for touch configuration
		// TODO: Use EVE_TOUCH_GOODIX?
		eve_printf_debug("Setup touch for Goodix\n");
		EVE_Hal_wr8(phost, REG_CPURESET, 2);
		EVE_Hal_wr16(phost, REG_TOUCH_CONFIG, 0x05d0);
		EVE_Hal_wr8(phost, REG_CPURESET, 0);
#endif
	}
#endif

#if (EVE_SUPPORT_CHIPID >= EVE_BT815)
#ifdef EVE_TOUCH_GOODIX
	eve_printf_debug("Goodix configuration\n");
	EVE_Hal_wr8(phost, REG_CPURESET, 2);
	EVE_Hal_wr16(phost, REG_TOUCH_CONFIG, 0x05d0);
	EVE_Hal_wr8(phost, REG_CPURESET, 0);
#endif
#endif

#ifdef EVE_SUPPORT_DDR
	if (EVE_Hal_supportDdr(phost))
	{
		// Discover installed RAM_G size from DDR_TYPE register
		uint32_t ddrType = EVE_Hal_rd32(phost, DDR_TYPE) & 0x7;
		switch (ddrType)
		{
		case 0:
			phost->RamGSize = 128 * 1024 * 1024; // 1 Gbit (128 MByte)
			break;
		case 1:
			phost->RamGSize = 256 * 1024 * 1024; // 2 Gbit (256 MByte)
			break;
		case 2:
			phost->RamGSize = 512 * 1024 * 1024; // 4 Gbit (512 MByte)
			break;
		case 3:
			phost->RamGSize = 1024 * 1024 * 1024; // 8 Gbit (1 GByte)
			break;
		case 4:
			phost->RamGSize = 64 * 1024 * 1024; // 512 Mbit (64 MByte)
			break;
		case 5:
			phost->RamGSize = 32 * 1024 * 1024; // 256 Mbit (32 MByte)
			break;
		default:
			eve_printf_debug("Unknown DDR_TYPE %lu\n", (unsigned long)ddrType);
			phost->RamGSize = RAM_G_SIZE_MAX;
			break;
		}

		// TODO_BT820: Do we need to set up any other DDR stuff here?
		// ...
	}
	else
	{
		phost->RamGSize = RAM_G_SIZE_MAX;
	}
	phost->RamGAvailable = phost->RamGSize - RAM_G_RESERVED;
#endif

#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
	if (phost->ReadTimeoutOccured)
	{
		eve_printf_debug("Read timeout occured\n");
		return false;
	}
	eve_printf_debug("RamGSize: %d \n", phost->RamGSize);
#endif

#if (defined(_WIN32) || defined(__linux__))
	if (bootup->StatusResult)
		*bootup->StatusResult = EVE_BOOTUP_STATUS_SUCCESS;
#endif
	return true;
}

/**
 * @brief
 *
 * @param phost Pointer to Hal context
 * @param config
 * @return true True if successful
 * @return false False if error
 */
EVE_HAL_EXPORT bool EVE_Util_config(EVE_HalContext *phost, EVE_ConfigParameters *config)
{
	/* IMPORTANT: Do not use EVE_CoCmd functions here, as they can be overridden by hooks */

	bool reconfigure = phost->PCLK;
	if (reconfigure)
	{
		/* If PCLK is already set, we're reconfiguring. Quiet flush first */
		eve_printf_debug("Reconfiguring\n");
		EVE_Cmd_waitFlush(phost);
	}

	const bool swapXY =
#if EVE_SUPPORT_CHIPID_FROM(EVE_BT820)
	    (EVE_CHIPID >= EVE_BT820) ? EVE_Hal_rd8(phost, REG_RE_ROTATE) & 0x2 :
#endif
#if EVE_SUPPORT_CHIPID_RANGE(EVE_FT810, EVE_BT820)
	                              ((EVE_CHIPID >= EVE_FT810) ? EVE_Hal_rd8(phost, REG_ROTATE) & 0x2 : false);
#else
	    false;
#endif
	uint16_t wp;
	uint16_t rp;

#if EVE_SUPPORT_CHIPID_TO(EVE_FT810)
	if (EVE_CHIPID < EVE_FT810)
	{
		eve_assert(config->Width < 512);
		eve_assert(config->Height < 512);
	}
	else
#endif
	{
		eve_assert(config->Width < 2048);
		eve_assert(config->Height < 2048);
	}

	/* Turn off display output */
#if EVE_SUPPORT_CHIPID_FROM(EVE_BT820)
	if (EVE_CHIPID >= EVE_BT820)
	{
		EVE_Hal_wr32(phost, REG_DISP, 0);
		EVE_Hal_wr32(phost, REG_SO_EN, 0);
	}
	else
#endif
	{
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
		EVE_Hal_wr8(phost, REG_PCLK, 0);
#endif
	}
	phost->PCLK = 0;

	EVE_Hal_wr16(phost, REG_HCYCLE, config->HCycle);
	EVE_Hal_wr16(phost, REG_HOFFSET, config->HOffset);
	EVE_Hal_wr16(phost, REG_HSYNC0, config->HSync0);
	EVE_Hal_wr16(phost, REG_HSYNC1, config->HSync1);
	EVE_Hal_wr16(phost, REG_VCYCLE, config->VCycle);
	EVE_Hal_wr16(phost, REG_VOFFSET, config->VOffset);
	EVE_Hal_wr16(phost, REG_VSYNC0, config->VSync0);
	EVE_Hal_wr16(phost, REG_VSYNC1, config->VSync1);
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
	if (EVE_CHIPID < EVE_BT820)
	{
		EVE_Hal_wr8(phost, REG_SWIZZLE, config->Swizzle);
	}
#endif
	EVE_Hal_wr8(phost, REG_PCLK_POL, config->PCLKPol);
	EVE_Hal_wr16(phost, REG_HSIZE, config->Width);
	EVE_Hal_wr16(phost, REG_VSIZE, config->Height);
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
	if (EVE_CHIPID < EVE_BT820)
	{
		EVE_Hal_wr16(phost, REG_CSPREAD, config->CSpread);
		EVE_Hal_wr16(phost, REG_DITHER, config->Dither);
		EVE_Hal_wr16(phost, REG_OUTBITS,
		    (uint16_t)(((config->OutBitsR & 0x7) << 6)
		        | ((config->OutBitsG & 0x7) << 3)
		        | (config->OutBitsB & 0x7)));
	}
#endif
	if (swapXY)
	{
		phost->Width = config->Height;
		phost->Height = config->Width;
	}
	else
	{
		phost->Width = config->Width;
		phost->Height = config->Height;
	}

#if EVE_SUPPORT_CHIPID_RANGE(EVE_BT817, EVE_BT820)
	bool pclk2X = false;
	if (EVE_CHIPID >= EVE_BT817 && EVE_CHIPID < EVE_BT820)
	{
		uint32_t freq = EVE_Hal_rd32(phost, REG_FREQUENCY);
		uint16_t pclkFreq = config->PClkFreq;
		if (!pclkFreq)
		{
			/* If not configured, match system frequency, in case a legacy configuration with PClk 1 is specified */
			/* PClk Freq is 60MHz by default, but we raise the system frequency to 72MHz during bootup */
			uint32_t refMul = freq / 6000000;
			uint32_t pllFreq = 12 * refMul;
			uint32_t range = pllFreq >= 160 ? 3 : (pllFreq >= 80 ? 2 : (pllFreq >= 40 ? 1 : 0));
			pclkFreq = 1 | ((refMul & 0x3F) << 4) | (range << 10);
		}
		EVE_Hal_wr32(phost, REG_PCLK_FREQ, pclkFreq);
		if (config->PClk2X)
		{
			/* User forces 2X */
			EVE_Hal_wr8(phost, REG_PCLK_2X, 1);
		}
		else if (config->PCLK == 1)
		{
			/* Send two pixels per system clock to the EXTSYNC block if PCLK_frequency > SystemClock */
			uint32_t pclkFrequency = getFreqFromPClkFreq(pclkFreq);
			pclk2X = pclkFrequency > freq;
			EVE_Hal_wr8(phost, REG_PCLK_2X, pclk2X);
		}
		else
		{
			EVE_Hal_wr8(phost, REG_PCLK_2X, 0);
		}
	}
#endif

#if EVE_SUPPORT_CHIPID_RANGE(EVE_BT815, EVE_BT820)
	if (EVE_CHIPID >= EVE_BT815 && EVE_CHIPID < EVE_BT820)
	{
		EVE_Hal_wr8(phost, REG_ADAPTIVE_FRAMERATE, config->AdaptiveFramerate);
	}
#endif

#if EVE_SUPPORT_CHIPID_RANGE(EVE_BT817, EVE_BT820)
	if (EVE_CHIPID >= EVE_BT817 && EVE_CHIPID < EVE_BT820)
	{
		EVE_Hal_wr16(phost, REG_AH_HCYCLE_MAX, config->AhHCycleMax);
	}
#endif

#if defined(RESISTANCE_THRESHOLD) /* TODO: From config */ && EVE_SUPPORT_CHIPID_TO(EVE_BT820) /* TODO_BT820: RZTHRESH? */
	if (EVE_Hal_isScreenResistive(phost) && EVE_CHIPID < EVE_BT820)
	{
		/* Touch configuration - configure the resistance value to 1200 - this value is specific to customer requirement and derived by experiment */
		/* TODO: From config */
		EVE_Hal_wr16(phost, REG_TOUCH_RZTHRESH, RESISTANCE_THRESHOLD);
	}
#endif

	/* TODO: Check if this goes after the HSF SUPPORT section, rather than here!! */
	/* Check duplicate register options!!! */
#if EVE_SUPPORT_CHIPID_FROM(EVE_BT820)
	if (EVE_CHIPID >= EVE_BT820 && config->SwapchainFormat != -1)
	{
		/* Set up swapchain at the back,
		last 0x280000 bytes are reserved by coprocessor,
		store available RAM_G size somewhere too */

		int32_t scanoutSrcStride;
		uint32_t endAddr = phost->RamGSize - RAM_G_RESERVED;
		int swapchainFormat = config->SwapchainFormat;
		switch (swapchainFormat)
		{
		case RGB6:
			scanoutSrcStride = ((config->Width * 9) + 3) >> 2;
			break;
		case YCBCR:
			scanoutSrcStride = config->Width;
			break;
		default:
			swapchainFormat = RGB8;
		case RGB8:
			scanoutSrcStride = config->Width * 3;
			break;
		}

		int32_t bufferSize = scanoutSrcStride * config->Height;
		eve_printf_debug("Swapchain format: %i, stride: %i, buffer size: 2x %i bytes\n", swapchainFormat, scanoutSrcStride, bufferSize);
		uint32_t buffer0 = endAddr - bufferSize;
		uint32_t buffer1 = buffer0 - bufferSize;
		phost->RamGAvailable = buffer1;

		/* FIXME: Does this belong in the display configuration??? */
		EVE_Hal_wr32(phost, REG_I2S_CTL, 0);
    	EVE_Hal_wr32(phost, REG_I2S_CFG, 0x400);
    	EVE_Hal_wr32(phost, REG_I2S_EN, 1);
    	EVE_Hal_wr32(phost, REG_I2S_FREQ, 0x3CF0);

		/* Configure two buffers */
		EVE_Hal_wr32(phost, REG_SC0_SIZE, 2);
		EVE_Hal_wr32(phost, REG_SC0_PTR0, buffer0);
		EVE_Hal_wr32(phost, REG_SC0_PTR1, buffer1);
		eve_printf_debug("REG_SC0_PTR0: %x \n", buffer0);
    	eve_printf_debug("REG_SC0_PTR1: %x \n", buffer1);

		/* Set the RE (render engine) destination (REG_RE_DEST) to SWAPCHAIN_0 */
		EVE_Hal_wr32(phost, REG_RE_DEST, SWAPCHAIN_0);
		EVE_Hal_wr32(phost, REG_RE_FORMAT, swapchainFormat);
		EVE_Hal_wr32(phost, REG_RE_W, phost->Width);
		EVE_Hal_wr32(phost, REG_RE_H, phost->Height);
#if (EVE_CMD_CACHE_RENDER_TARGET)
		phost->CoRenderTargetDst = SWAPCHAIN_0;
		phost->CoRenderTargetFmt = swapchainFormat;
		phost->CoRenderTargetW = phost->Width;
		phost->CoRenderTargetH = phost->Height;
#endif

		/* Set the scanout source (REG_SO_SOURCE) to SWAPCHAIN_0 */
		EVE_Hal_wr32(phost, REG_SO_SOURCE, SWAPCHAIN_0);
		EVE_Hal_wr32(phost, REG_SO_FORMAT, swapchainFormat);
		// EVE_Hal_wr32(phost, REG_SO_MODE, 3); /* FIXME_BT820: DUPLICATE & calculated??? */
		// EVE_Hal_wr32(phost, REG_SO_EN, 1); /* FIXME_BT820: DUPLICATE */
		// EVE_Cmd_waitFlush(phost); /* FIXME: Duplicate??? */
	}
#endif

	EVE_Util_clearScreen(phost);

	/* Refresh fifo */
	wp = EVE_Cmd_wp(phost);
	rp = EVE_Cmd_rp(phost);
	EVE_Cmd_space(phost);
#if EVE_SUPPORT_CHIPID_FROM(EVE_FT810)
	if (EVE_CHIPID >= EVE_FT810)
	{
		phost->MediaFifoAddress = 0;
		phost->MediaFifoSize = 0;
	}
#endif

	/* Coprocessor needs a reset */
	if (reconfigure ? EVE_CMD_FAULT(rp) : (wp || rp))
	{
		/* If this occurs after powerdown and bootup, powerdown is not working */
		eve_printf_debug("Coprocessor fifo not empty\n");
		if (!EVE_Util_resetCoprocessor(phost))
			return false;
	}
	else
	{
		/* Setup coprocessor defaults */
		eve_printf_debug("Configure coprocessor defaults\n");
		EVE_Cmd_wr32(phost, CMD_DLSTART);
		EVE_Cmd_wr32(phost, CMD_COLDSTART);

		/* Set the latest API level.
		CMD_APILEVEL applicable since BT817. */
#if EVE_SUPPORT_CHIPID_RANGE(EVE_BT817, EVE_BT820)
		if (EVE_CHIPID >= EVE_BT817 && EVE_CHIPID < EVE_BT820)
		{
			EVE_Cmd_wr32(phost, CMD_APILEVEL);
			EVE_Cmd_wr32(phost, 2);
		}
#endif

#ifdef EVE_FLASH_AVAILABLE
		if (EVE_CHIPID >= EVE_BT815)
		{
			/* Reattach flash to avoid inconsistent state */
			EVE_Cmd_wr32(phost, CMD_FLASHDETACH);
			EVE_Cmd_wr32(phost, CMD_FLASHATTACH);
		}
#endif

		EVE_CoDlImpl_resetCoState(phost);
	}

#ifdef EVE_SUPPORT_HSF
	if (config->HsfWidth)
	{
		if (EVE_Hal_supportHsf(phost) && !pclk2X)
		{
			EVE_Cmd_startFunc(phost);
			EVE_Cmd_wr32(phost, CMD_HSF);
			EVE_Cmd_wr32(phost, config->HsfWidth);
			EVE_Cmd_endFunc(phost);
		}
		else
		{
			/* Set width to screen width if HSF is not supported here */
			eve_printf_debug("HSF specified but not supported on this device\n");
			EVE_Hal_wr16(phost, REG_HSIZE, config->HsfWidth);
			if (swapXY)
				phost->Height = config->HsfWidth;
			else
				phost->Width = config->HsfWidth;
		}
	}
#endif

	EVE_Hal_flush(phost);
	if (!EVE_Cmd_waitFlush(phost))
		return false;

#if EVE_SUPPORT_CHIPID_FROM(EVE_BT820)
	if (EVE_CHIPID >= EVE_BT820)
	{
		uint32_t lvdsPllCfg = EVE_Hal_rd32(phost, LVDSPLL_CFG);
		lvdsPllCfg &= ~0xFUL;
		lvdsPllCfg |= (config->PCLK - 1); /* PCLK is used to store TXCLKDIV+1 */
		uint32_t lvdsCtlClk = ((config->RefClkFreq / config->PCLK) / 2) * 7; /* TODO: Verify if value 7 is NS from bit 10-4 or hardcoded */
		uint32_t lvdsCks = lvdsCtlClk < 400000000 ? 1 : 2;
		lvdsPllCfg &= ~(0x3UL << 11);
		lvdsPllCfg |= (lvdsCks << 11); /* LVDS clock range selection */
		EVE_Hal_wr32(phost, LVDSPLL_CFG, lvdsPllCfg);
		/* In 2 pixel per clock mode, also possible to select between 1 and 2 (single and dual channel) but defaults to dual here */
		uint32_t scanoutMode = (config->PixelsPerClock == 1 ? 0 : (config->PixelsPerClock == 2 ? 2 : 3));
		EVE_Hal_wr32(phost, REG_SO_MODE, scanoutMode);	
		if (config->SwapchainFormat != -1)
		{
			EVE_Hal_wr32(phost, REG_SO_EN, 1); /* After this display is visible on the LCD */
			EVE_Hal_wr32(phost, REG_DISP, 1);
			EVE_Hal_wr32(phost, REG_PWM_DUTY, 128);
			phost->PCLK = 1;

			// uint8_t TXPLLDiv = 0x03;
			// EVE_Hal_wr32(phost, REG_LVDS_PLLCFG, (TXPLLDiv > 4 ? (0x00300870 + TXPLLDiv) : (0x00301070 + TXPLLDiv)));

			EVE_Hal_wr32(phost, REG_LVDS_EN, 0); /* TODO_REVIEW: Hardcoded configuration for BT812 */
			EVE_Hal_wr32(phost, REG_LVDS_EN, 7);
			// EVE_Hal_wr32(phost, REG_LVDSTX_EN, LVDS_CH1_EN | LVDS_CH0_EN); <--- this one?
			EVE_sleep(10);
		}


		/* !!!!!!!!!!! */
		/* TODO_REVIEW: Hardcoded configuration for BT812 */
		/* !!!!!!!!!!! */
		//eve_printf_debug("lvdsPllCfg: %x \n", lvdsPllCfg);
		eve_printf_debug("LVDS_EN: %x \n", EVE_Hal_rd32(phost, LVDS_EN));
    	eve_printf_debug("LVDSPLL_CFG: %x \n", EVE_Hal_rd32(phost, LVDSPLL_CFG));
   		eve_printf_debug("LVDS_CTRL_CH0: %x \n", EVE_Hal_rd32(phost, LVDS_CTRL_CH0));
    	eve_printf_debug("LVDS_CTRL_CH1: %x \n", EVE_Hal_rd32(phost, LVDS_CTRL_CH1));
		eve_printf_debug("LVDS_STAT is %x \n", EVE_Hal_rd32(phost, LVDS_STAT));
    	eve_printf_debug("LVDS_ERR_STAT is %x \n", EVE_Hal_rd32(phost, LVDS_ERR_STAT));
	}
	else
#endif
	{
#if EVE_SUPPORT_CHIPID_TO(EVE_BT820)
		EVE_Hal_wr8(phost, REG_PCLK, config->PCLK); /* After this display is visible on the LCD */
		phost->PCLK = config->PCLK;
#endif
	}

#ifdef EVE_SUPPORT_HSF
	if (config->HsfWidth)
	{
		if (EVE_Hal_supportHsf(phost))
		{
			eve_printf_debug("HSize: %i, HSF HSize: %i\n", EVE_Hal_rd32(phost, REG_HSIZE), EVE_Hal_rd32(phost, REG_HSF_HSIZE));
		}
	}
#endif

#if defined(ENABLE_ILI9488_HVGA_PORTRAIT) || (defined(ENABLE_KD2401_HVGA_PORTRAIT) && !defined(EVE_GRAPHICS_IDP_3500_04A))
	/* to cross check reset pin */
	EVE_Hal_wr8(phost, REG_GPIO, 0xff);
	EVE_Hal_flush(phost);
	EVE_sleep(120);
	EVE_Hal_wr8(phost, REG_GPIO, 0x7f);
	EVE_Hal_flush(phost);
	EVE_sleep(120);
	EVE_Hal_wr8(phost, REG_GPIO, 0xff);
	EVE_Hal_flush(phost);
	EVE_sleep(120);
#endif

#if defined(ENABLE_ILI9488_HVGA_PORTRAIT) || (defined(ENABLE_KD2401_HVGA_PORTRAIT) && !defined(EVE_GRAPHICS_IDP_3500_04A))
#if defined(RP2040_PLATFORM)
	EVE_ILI9488_RP2040_bootup(phost);
	eve_printf_debug("after ILI9488 RP2040 bootup\n");
	EVE_Hal_restoreSPI(phost);
#endif
#if defined(FT9XX_PLATFORM)
	EVE_ILI9488_bootup();
	eve_printf_debug("after ILI9488 bootup\n");
	EVE_Hal_restoreSPI(phost);
#endif
#endif

#if defined(ENABLE_IDM2040_21R_RP2040)
#if defined(RP2040_PLATFORM)
	EVE_IDM2040_21R_RP2040_bootup();
	eve_printf_debug("after ILI9488 RP2040 bootup\n");
	EVE_Hal_restoreSPI(phost);
#endif
#endif

#if 0
	eve_printf_debug("REG_HSIZE: %i\n", (int)EVE_Hal_rd16(phost, REG_HSIZE));
	eve_printf_debug("REG_VSIZE: %i\n", (int)EVE_Hal_rd16(phost, REG_VSIZE));
	eve_printf_debug("REG_HCYCLE: %i\n", (int)EVE_Hal_rd16(phost, REG_HCYCLE));
	eve_printf_debug("REG_HOFFSET: %i\n", (int)EVE_Hal_rd16(phost, REG_HOFFSET));
	eve_printf_debug("REG_HSYNC0: %i\n", (int)EVE_Hal_rd16(phost, REG_HSYNC0));
	eve_printf_debug("REG_HSYNC1: %i\n", (int)EVE_Hal_rd16(phost, REG_HSYNC1));
	eve_printf_debug("REG_VCYCLE: %i\n", (int)EVE_Hal_rd16(phost, REG_VCYCLE));
	eve_printf_debug("REG_VOFFSET: %i\n", (int)EVE_Hal_rd16(phost, REG_VOFFSET));
	eve_printf_debug("REG_VSYNC0: %i\n", (int)EVE_Hal_rd16(phost, REG_VSYNC0));
	eve_printf_debug("REG_VSYNC1: %i\n", (int)EVE_Hal_rd16(phost, REG_VSYNC1));
	eve_printf_debug("REG_PCLK: %i\n", (int)EVE_Hal_rd16(phost, REG_PCLK));
	eve_printf_debug("REG_SWIZZLE: %i\n", (int)EVE_Hal_rd16(phost, REG_SWIZZLE));
	eve_printf_debug("REG_PCLK_POL: %i\n", (int)EVE_Hal_rd16(phost, REG_PCLK_POL));
	eve_printf_debug("REG_CSPREAD: %i\n", (int)EVE_Hal_rd16(phost, REG_CSPREAD));
	eve_printf_debug("REG_OUTBITS: %i\n", (int)EVE_Hal_rd16(phost, REG_OUTBITS));
	eve_printf_debug("REG_DITHER: %i\n", (int)EVE_Hal_rd16(phost, REG_DITHER));
#if (EVE_SUPPORT_CHIPID >= EVE_BT815)
	eve_printf_debug("REG_ADAPTIVE_FRAMERATE: %i\n", (int)EVE_Hal_rd16(phost, REG_ADAPTIVE_FRAMERATE));
#endif
#if (EVE_SUPPORT_CHIPID >= EVE_BT817)
	eve_printf_debug("REG_PCLK_FREQ: %X\n", (int)EVE_Hal_rd16(phost, REG_PCLK_FREQ));
	eve_printf_debug("REG_AH_HCYCLE_MAX: %i\n", (int)EVE_Hal_rd16(phost, REG_AH_HCYCLE_MAX));
	eve_printf_debug("REG_HSF_HSIZE: %i\n", (int)EVE_Hal_rd16(phost, REG_HSF_HSIZE));
#endif
#endif

#if 1
#if EVE_SUPPORT_CHIPID_FROM(EVE_BT820)
	eve_printf_debug("LVDS_EN: %x \n", EVE_Hal_rd32(phost, REG_LVDS_EN));
	eve_printf_debug("LVDSPLL_CFG: %x \n", EVE_Hal_rd32(phost, REG_LVDS_PLLCFG));
	eve_printf_debug("LVDS_CTRL_CH0: %x \n", EVE_Hal_rd32(phost, REG_LVDS_CTRL_CH0));
	eve_printf_debug("LVDS_CTRL_CH1: %x \n", EVE_Hal_rd32(phost, REG_LVDS_CTRL_CH1));
	eve_printf_debug("LVDS_STAT: %x \n", EVE_Hal_rd32(phost, REG_LVDS_STAT));
	eve_printf_debug("LVDS_ERR_STAT: %x \n", EVE_Hal_rd32(phost, REG_LVDS_ERR_STAT));
	eve_printf_debug("REG_SO_EN: %x \n", EVE_Hal_rd32(phost, REG_SO_EN));
	eve_printf_debug("REG_SO_MODE: %x \n", EVE_Hal_rd32(phost, REG_SO_MODE));
#endif
#endif

#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
	if (EVE_CHIPID >= EVE_BT820)
	{
		/*
		EVE_Cmd_startFunc(phost);
		EVE_Cmd_wr32(phost, RAM_J1RAM + 828);
		EVE_Cmd_wr32(phost, RAM_HIMEM);
		uint8_t zeros[256] = {0};
		EVE_Cmd_wr32(phost, RAM_HIMEM);
		EVE_Cmd_wrMem(phost, zeros, sizeof(zeros));
		EVE_Cmd_endFunc(phost);
		*/

		eve_printf_debug("Load Plot Patch Data:\n");
		uploadPlotPatch(phost);    // Load Plot Patch Data
	
		char msg[128];
		EVE_Hal_rdMem(phost, msg, RAM_HIMEM, 128);  // read 128 bytes from RAM_REPORT
		eve_printf_debug("Read Plot Patch Data: %s \n", msg);
	}
#endif

	eve_printf_debug("EVE configuration ready\n");

	return true;
}

EVE_HAL_EXPORT void EVE_Util_shutdown(EVE_HalContext *phost)
{
#if (EVE_SUPPORT_CHIPID >= EVE_BT820 || defined(EVE_MULTI_GRAPHICS_TARGET))
	if (EVE_CHIPID >= EVE_BT820)
	{
		/* TODO_BT820: Verify if this is needed (also verify on other EVE gens */
		EVE_Hal_wr16(phost, REG_GPIO_DIR, 0xffff);
		EVE_Hal_wr16(phost, REG_GPIO, 0xffff);
	}
	else
#endif
	{
#if (EVE_SUPPORT_CHIPID < EVE_BT820 || defined(EVE_MULTI_GRAPHICS_TARGET))
		if (EVE_CHIPID >= EVE_FT810)
		{
			EVE_Hal_wr16(phost, REG_GPIOX_DIR, 0xffff);
			EVE_Hal_wr16(phost, REG_GPIOX, 0xffff);
		}
		else
		{
			EVE_Hal_wr8(phost, REG_GPIO_DIR, 0xff);
			EVE_Hal_wr8(phost, REG_GPIO, 0xff);
		}
#endif
	}
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
	if (EVE_CHIPID >= EVE_BT820)
	{
		EVE_Hal_wr8(phost, REG_SO_EN, 0);								
	}
	else
#endif
	{
#if (EVE_SUPPORT_CHIPID < EVE_BT820 || defined(EVE_MULTI_GRAPHICS_TARGET))
		EVE_Hal_wr8(phost, REG_PCLK, 0);
#endif
	}
	EVE_Hal_flush(phost);
	EVE_Hal_powerCycle(phost, false);
}

#if (EVE_SUPPORT_CHIPID < EVE_BT820 || defined(EVE_MULTI_GRAPHICS_TARGET))

/*
Patch: OTP needs to be reactivated when the coprocessor is reset during CMD_LOGO
Applicable to: FT81X-series
*/
#define EVE_SUBPATCH_PTR 0x7ffeU /* NOTE: This address is only valid for FT81X */
static inline bool EVE_Util_needsSubPatch(EVE_HalContext *phost)
{
	return (EVE_CHIPID >= EVE_FT810) && (EVE_CHIPID <= EVE_FT813);
}

/*
Patch: Video patch from OTP needs to be reapplied after coprocessor reset
Applicable to: BT81X-series
*/
#define EVE_VIDEOPATCH_ADDR 0x309162UL /* NOTE: This address is only valid for BT815 and BT816 */
static inline bool EVE_Util_needsVideoPatch(EVE_HalContext *phost)
{
	return (EVE_CHIPID >= EVE_BT815) && (EVE_CHIPID <= EVE_BT816);
}

#endif

#if defined(_DEBUG)
/* Backup the last 128 bytes of RAM_G, which may be used for an error message */
void debugBackupRamG(EVE_HalContext *phost)
{
	if (!phost->DebugMessageVisible)
	{
		EVE_Hal_rdMem(phost, phost->DebugBackup, RAM_G + RAM_G_SIZE - sizeof(phost->DebugBackup), sizeof(phost->DebugBackup));
		phost->DebugMessageVisible = true;
	}
}
static void debugRestoreRamG(EVE_HalContext *phost)
{
	if (phost->DebugMessageVisible)
	{
		EVE_Hal_wrMem(phost, RAM_G + RAM_G_SIZE - sizeof(phost->DebugBackup), phost->DebugBackup, sizeof(phost->DebugBackup));
		phost->DebugMessageVisible = false;
	}
}
#endif

/**
 * @brief Reset Coprocessor
 *
 * @param phost  Pointer to Hal context
 * @return true True if successful
 * @return false False if error
 */
EVE_HAL_EXPORT bool EVE_Util_resetCoprocessor(EVE_HalContext *phost)
{
	/* IMPORTANT: Do not use EVE_CoCmd functions here, as they can be overridden by hooks */

#if (EVE_SUPPORT_CHIPID < EVE_BT820 || defined(EVE_MULTI_GRAPHICS_TARGET))
	const bool needsVideoPatch = EVE_Util_needsVideoPatch(phost);
	uint16_t videoPatchVector;
#endif
	bool ready;
	uint16_t rd;
	uint16_t wr;
	(void)rd;
	(void)wr;

	eve_printf_debug("Reset coprocessor\n");

#if (EVE_SUPPORT_CHIPID < EVE_BT820 || defined(EVE_MULTI_GRAPHICS_TARGET))
	if (needsVideoPatch)
	{
		/* BT81X video patch */
		videoPatchVector = EVE_Hal_rd16(phost, EVE_VIDEOPATCH_ADDR);
	}
#endif

	/* Set REG_CPURESET to 1, to hold the coprocessor in the reset condition */
	EVE_Hal_wr8(phost, REG_CPURESET, 1);
	EVE_Hal_flush(phost);
	EVE_sleep(100);

	/* Set REG_CMD_READ and REG_CMD_WRITE to zero */
	EVE_Hal_wr16(phost, REG_CMD_READ, 0);
	EVE_Hal_wr16(phost, REG_CMD_WRITE, 0);
	EVE_Hal_wr16(phost, REG_CMD_DL, 0);
#if (EVE_SUPPORT_CHIPID >= EVE_BT820)
	if (EVE_CHIPID >= EVE_BT820)
	{
		EVE_Hal_wr8(phost, REG_SO_EN, phost->PCLK);
	}
	else
#endif
	{
#if (EVE_SUPPORT_CHIPID < EVE_BT820 || defined(EVE_MULTI_GRAPHICS_TARGET))
		EVE_Hal_wr8(phost, REG_PCLK, phost->PCLK); /* j1 will set the pclk to 0 for that error case */
#endif
	}

	/* Stop playing audio in case video with audio was playing during reset */
	EVE_Hal_wr8(phost, REG_PLAYBACK_PLAY, 0);

#if (EVE_SUPPORT_CHIPID < EVE_BT820 || defined(EVE_MULTI_GRAPHICS_TARGET))
	if (EVE_Util_needsSubPatch(phost))
	{
		/* Enable patched rom in case the reset is requested while CMD_LOGO is running.
		This is necessary as CMD_LOGO may swap to the other rom page */
		EVE_Hal_wr8(phost, REG_ROMSUB_SEL, 3);
	}
#endif

	/* Default */
	phost->CmdFault = false;

	/* Set REG_CPURESET to 0, to restart the coprocessor */
	EVE_Hal_wr8(phost, REG_CPURESET, 0);
	EVE_Hal_flush(phost);
	EVE_sleep(100);
	eve_assert((wr = EVE_Hal_rd16(phost, REG_CMD_WRITE)) == 0);
	eve_assert((rd = EVE_Hal_rd16(phost, REG_CMD_READ)) == 0);

	/* Refresh fifo */
	EVE_Cmd_waitFlush(phost);
#if (EVE_SUPPORT_CHIPID >= EVE_FT810)
	if (EVE_CHIPID >= EVE_FT810)
	{
		phost->MediaFifoAddress = 0;
		phost->MediaFifoSize = 0;
	}
#endif

#if (EVE_SUPPORT_CHIPID < EVE_BT820 || defined(EVE_MULTI_GRAPHICS_TARGET))
	if (EVE_Util_needsSubPatch(phost))
	{
		/* Clear cmd with CMD_STOP, exiting CMD_EXECUTE may loop over, depending on OTP */
		EVE_Hal_startTransfer(phost, EVE_TRANSFER_WRITE, RAM_CMD);
		for (unsigned int i = 0; i < EVE_CMD_FIFO_SIZE; i += 4)
			EVE_Hal_transfer32(phost, CMD_STOP);
		EVE_Hal_endTransfer(phost);

		/* Go back into the patched coprocessor main loop */
		EVE_Hal_wr8(phost, REG_ROMSUB_SEL, 3);
		EVE_Cmd_startFunc(phost);
		EVE_Cmd_wr32(phost, (4294967047UL));
		EVE_Cmd_wr32(phost, EVE_SUBPATCH_PTR);
		EVE_Cmd_wr32(phost, 0);
		EVE_Cmd_endFunc(phost);
		EVE_Hal_flush(phost);
		eve_assert((wr = EVE_Hal_rd16(phost, REG_CMD_WRITE)) == 12);
		eve_assert((rd = EVE_Hal_rd16(phost, REG_CMD_READ)) == 0);

		/* Difficult to check when CMD_EXECUTE is processed when there's an OTP,
		since the read pointer keeps looping back to 0. */
		EVE_sleep(100);
		eve_assert((wr = EVE_Hal_rd16(phost, REG_CMD_WRITE)) == 12);
		eve_assert((rd = EVE_Hal_rd16(phost, REG_CMD_READ)) == 0);

		/* Need to manually stop previous command from repeating infinitely,
		however, this may cause the coprocessor to overshoot the command fifo,
		hence it's been filled with harmless CMD_STOP commands. */
		EVE_Hal_wr16(phost, REG_CMD_WRITE, 0);
		EVE_Hal_flush(phost);
		EVE_sleep(100);

		/* Refresh fifo */
		EVE_Cmd_waitFlush(phost);
		eve_assert((wr = EVE_Hal_rd16(phost, REG_CMD_WRITE)) == 0);
		eve_assert((rd = EVE_Hal_rd16(phost, REG_CMD_READ)) == 0);
	}

	if (needsVideoPatch)
	{
		/* BT81X video patch */
		EVE_Hal_wr16(phost, EVE_VIDEOPATCH_ADDR, videoPatchVector);
	}
#endif

	/* Cold start. Ensure that the coprocessor is ready. */
	EVE_Cmd_wr32(phost, CMD_DLSTART);
	EVE_Cmd_wr32(phost, CMD_COLDSTART);

	/* Set the latest API level.
	CMD_APILEVEL applicable since BT817. */
#if (EVE_SUPPORT_CHIPID >= EVE_BT817) && (EVE_SUPPORT_CHIPID < EVE_BT820 || defined(EVE_MULTI_GRAPHICS_TARGET))
	if (EVE_CHIPID >= EVE_BT817 && EVE_CHIPID < EVE_BT820)
	{
		EVE_Cmd_wr32(phost, CMD_APILEVEL);
		EVE_Cmd_wr32(phost, 2);
	}
#endif

#ifdef EVE_FLASH_AVAILABLE
	if (EVE_CHIPID >= EVE_BT815)
	{
		/* Reattach flash to avoid inconsistent state */
		EVE_Cmd_wr32(phost, CMD_FLASHATTACH);
	}
#endif

	/* Wait for coprocessor to be ready */
	ready = EVE_Cmd_waitFlush(phost);
	if (phost->CbCoprocessorReset) /* Notify */
		phost->CbCoprocessorReset(phost, !ready);

#if defined(_DEBUG)
	debugRestoreRamG(phost);
#endif
	EVE_CoDlImpl_resetCoState(phost);
	return ready;
}

/**
 * @brief Bootup Coprocessor
 *
 * @param phost  Pointer to Hal context
 * @return true True if successful
 * @return false False if error
 */
EVE_HAL_EXPORT bool EVE_Util_bootupConfig(EVE_HalContext *phost)
{
	EVE_BootupParameters bootup;
	EVE_ConfigParameters config;

	EVE_Util_bootupDefaults(phost, &bootup);
	
	if (!EVE_Util_bootup(phost, &bootup))
	{
		return false;
	}

	EVE_Util_configDefaults(phost, &config, EVE_DISPLAY_DEFAULT);
	if (!EVE_Util_config(phost, &config))
	{
		EVE_Util_shutdown(phost);
		return false;
	}
	return true;
}

void EVE_Util_forceFault(EVE_HalContext *phost, const char *err)
{
	/* Check if we're already in fault state */
	if (!EVE_Cmd_waitSpace(phost, 0))
		return;
#if 1
	/* Go into reset state and mimic a fault */
	EVE_Hal_wr8(phost, REG_CPURESET, 1);
	EVE_Hal_flush(phost);
	EVE_sleep(100);
	EVE_Hal_wr16(phost, REG_CMD_READ, 0xFFF);
	EVE_Hal_wr16(phost, REG_CMD_WRITE, 0xFFF);
#else
	/* TODO: Clean this up a bit */
	if (!EVE_Util_resetCoprocessor(phost))
		return;
	EVE_CoCmd_dlStart(phost);
	for (int i = 0; i < EVE_DL_COUNT; ++i)
		EVE_Cmd_wr32(phost, DISPLAY());
	EVE_Cmd_waitFlush(phost);
	EVE_Cmd_wr32(phost, DISPLAY());
	EVE_Hal_flush(phost);
	EVE_sleep(100);
#endif
#if (EVE_SUPPORT_CHIPID >= EVE_BT815)
	if (EVE_CHIPID >= EVE_BT815 && (EVE_Cmd_rp(phost) & 0x3))
	{
		/* Write our own error to the error report area */
		uint32_t errLen = (uint32_t)strlen(err);
		EVE_Hal_wrMem(phost, RAM_ERR_REPORT, (const uint8_t *)err, errLen);
		if (errLen < RAM_ERR_REPORT_MAX)
		{
			EVE_Hal_startTransfer(phost, EVE_TRANSFER_WRITE, RAM_ERR_REPORT + errLen);
			for (int i = (int)errLen; i < RAM_ERR_REPORT_MAX; ++i)
				EVE_Hal_transfer8(phost, 0);
			EVE_Hal_endTransfer(phost);
		}
	}
#endif
	/* Fault state now */
	EVE_Cmd_waitFlush(phost);
#if 0
#if defined(_DEBUG)
#if (EVE_SUPPORT_CHIPID >= EVE_BT815)
	if (EVE_CHIPID >= EVE_BT815
	    && phost->DebugMessageVisible)
	{
		uint32_t errLen = (uint32_t)strlen(err);
		EVE_Hal_wrMem(phost, RAM_G + RAM_G_SIZE - sizeof(phost->DebugBackup), (const uint8_t *)err, errLen);
		if (errLen < sizeof(phost->DebugBackup))
		{
			EVE_Hal_startTransfer(phost, EVE_TRANSFER_WRITE, RAM_G + RAM_G_SIZE - sizeof(phost->DebugBackup) + errLen);
			for (int i = (int)errLen; i < sizeof(phost->DebugBackup); ++i)
				EVE_Hal_transfer8(phost, 0);
			EVE_Hal_endTransfer(phost);
		}
	}
#endif
#endif
#endif

}

/* end of file */
