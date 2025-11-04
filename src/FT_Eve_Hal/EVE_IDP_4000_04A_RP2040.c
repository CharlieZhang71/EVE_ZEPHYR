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
 *
 * Abstract: ILI9488 driver source
 *
 */

#include "EVE_Platform.h"

#if (defined(RP2040_PLATFORM) && defined(ENABLE_IDP_4000_04A_RP2040))

#define LCD_CS            22
#define DISP_DR_CS_LOW	  (gpio_put(LCD_CS, 0))
#define DISP_DR_CS_HIGH	  (gpio_put(LCD_CS, 1))
#define DISP_DR_CLK_LOW	  (gpio_put(EVE_DEFAULT_SPI0_SCK, 0))
#define DISP_DR_CLK_HIGH  (gpio_put(EVE_DEFAULT_SPI0_SCK, 1))
#define DISP_DR_MOSI_LOW  (gpio_put(EVE_DEFAULT_SPI0_MOSI, 0))
#define DISP_DR_MOSI_HIGH (gpio_put(EVE_DEFAULT_SPI0_MOSI, 1))

static void writeb(bool iscommand, uint8_t b)
{
	uint8_t i;
	DISP_DR_CS_LOW;
	DISP_DR_CLK_LOW;
	if (iscommand)
		DISP_DR_MOSI_LOW;
	else
		DISP_DR_MOSI_HIGH;
	EVE_sleep(1);
	DISP_DR_CLK_HIGH;
	for (i = 0; i < 8; i++) {
		DISP_DR_CLK_LOW;
		EVE_sleep(1);
		if (b & 0x80)
			DISP_DR_MOSI_HIGH;
		else
			DISP_DR_MOSI_LOW;
		DISP_DR_CLK_HIGH;
		EVE_sleep(1);
		b = (b << 1);
	}
	DISP_DR_CS_HIGH;
}

static void write_data(uint8_t w)
{
	writeb(false, w);
}

static void write_command(uint8_t y)
{
	writeb(true, y);
}

void lcd40_init()
{
	// clang-format off
	const uint8_t st7701s_tab[] = {
		/* len including command-byte, command-byte, data... */
		6,	0xff, 0x77, 0x01, 0x00, 0x00, 0x10,	// Select Page 0
		3,	0xc0, 0x3b, 0x00,	// display line setting
		3,	0xc1, 0x0d, 0x02,	// porch control
		3,	0xc2, 0x37, 0x05,	// inversion set to 31 2-dot, 37-column
	//	3,	0x02, 0x00, 0x00,	// 00-DE mode, 80-HV mode, PCLK n
		17,	0xB0, 0x00, 0x11, 0x18, 0x0E, 0x11, 0x06, 0x07, 0x08, 0x07, 0x22,
			0x04, 0x12, 0x0F, 0xAA, 0x31, 0x18, // Pos Voltage Gamma Control
		17,	0xB1, 0x00, 0x11, 0x19, 0x0E, 0x12, 0x07, 0x08, 0x08, 0x08, 0x22,
			0x04, 0x11, 0x11, 0xA9, 0x32, 0x18, // Neg Voltage Gamma Control
		6,	0xFF, 0x77, 0x01, 0x00, 0x00, 0x11,	// Select Page 2 BK1 function
		2,	0xB0, 0x60,			// Vop Amplitude setting Vop=4.7375v
		2,	0xB1, 0x26,			// VCOM amplitude setting VCOM=32
		2,	0xB2, 0x07,			// VGH Voltage setting VGH=15v
		2,	0xB3, 0x80,			// TEST Command Setting
		2,	0xB5, 0x49,			// VGL Voltage setting VGL=-10.17v
		2,	0xB7, 0x85,			// Power Control 1
		2,	0xB8, 0x21,			// Power Control 2 AVDD=6.6 & AVCL=-4.6
		2,	0xC1, 0x78,			// Source pre_drive timing set1
		2,	0xC2, 0x78,			// Source EQ2 Setting
		//*********GIP SET*************//
		4,	0xE0, 0x00, 0x1B, 0x02,
		12,	0xE1, 0x08, 0xA0, 0x00, 0x00, 0x07, 0xA0, 0x00, 0x00, 0x00, 0x44,
			0x44,
		13,	0xE2, 0x11, 0x11, 0x44, 0x44, 0xED, 0xA0, 0x00, 0x00, 0xEC, 0xA0,
			0x00, 0x00,
		5,	0xE3, 0x00, 0x00, 0x11, 0x11,
		3,	0xE4, 0x44, 0x44,
		17,	0xE5, 0x0A, 0xE9, 0xD8, 0xA0, 0x0C, 0xEB, 0xD8, 0xA0, 0x0E, 0xED,
			0xD8, 0xA0, 0x10, 0xEF, 0xD8, 0xA0,
		5,	0xE6, 0x00, 0x00, 0x11, 0x11,
		3,	0xE7, 0x44, 0x44,
		17,	0xE8, 0x09, 0xE8, 0xD8, 0xA0, 0x0B, 0xEA, 0xD8, 0xA0, 0x0D, 0xEC,
			0xD8, 0xA0, 0x0F, 0xEE, 0xD8, 0xA0,
		8,	0xEB, 0x02, 0x00, 0xE4, 0xE4, 0x88, 0x00, 0x40,
		3,	0xEC, 0x3C, 0x00,
		17,	0xED, 0xAB, 0x89, 0x76, 0x54, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0x20, 0x45, 0x67, 0x98, 0xBA,
			//-----------VAP & VAN---------------
			6,	0xFF, 0x77, 0x01, 0x00, 0x00, 0x13,	// Select Page 3
			2,	0xE5, 0xE4,
			6,	0xFF, 0x77, 0x01, 0x00, 0x00, 0x00,	// Select Page 0 & no banking
			2,	0x36, 0x00,		// 10-180
			2,	0x3A, 0x70,		// 24 bit
			1,	0x29,			// Display on
			0,	0,0,
	};
	// clang-format on
	write_command(0x11);
	EVE_sleep(120);
	for (const uint8_t* p = st7701s_tab; *p != 0;) {
		int len = *p++;
		write_command(*p++);
		for (int i = 0; i < len - 1; i++)
			write_data(*p++);
	}
	EVE_sleep(25);
}

void EVE_IDP_4000_04A_RP2040_bootup()
{
	// LCD panel cannot setup first because LCD_RESET is FT8xx DISP pin, which
    // is now enabled above. But now, have to override EVE MOSI & SCK pins for
    // LCD data sending. There's no need to spi_deinit(spi0) here, but force
    // disable EVE_CS.
	gpio_init(LCD_CS);
	gpio_set_dir(LCD_CS, GPIO_OUT);
	gpio_put(LCD_CS, 1);
	// printf("%s(): Setting up SW SPI for LCD\n", __func__);
	gpio_put(EVE_DEFAULT_SPI0_CS, 1);
	gpio_init(EVE_DEFAULT_SPI0_SCK);
	gpio_init(EVE_DEFAULT_SPI0_MOSI);
	gpio_set_dir(EVE_DEFAULT_SPI0_SCK, GPIO_OUT);
	gpio_set_dir(EVE_DEFAULT_SPI0_MOSI, GPIO_OUT);
	gpio_put(EVE_DEFAULT_SPI0_SCK, 1);
	gpio_put(EVE_DEFAULT_SPI0_MOSI, 1);
	lcd40_init();
	// re-direct SPI0 pins back for EVE GPU
	gpio_set_function(EVE_DEFAULT_SPI0_SCK, GPIO_FUNC_SPI);
	gpio_set_function(EVE_DEFAULT_SPI0_MOSI, GPIO_FUNC_SPI);
}

#endif

/*end of file*/
