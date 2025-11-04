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

#if (defined(RP2040_PLATFORM) && (defined(ENABLE_IDM2040_21R_RP2040) || defined(ENABLE_IDP_4000_04A_RP2040)))

#define LCD_SPI1_CS    17
#define LCD_SPI1_SCK   10
#define LCD_SPI1_MOSI  11
#define LCD_RESET      22

#define BUTTON_PIN     21
#define ENCODER_PIN_A  23
#define ENCODER_PIN_B  24
static int16_t encoder = 0; // save the encoder change for polling
static int16_t button = 0; // save button change for polling

/********************************* LCD related *************************************/
void send_data(uint8_t data, bool cmd)
{
	uint16_t trans = 0;

	gpio_put(LCD_SPI1_CS, 0);
	EVE_sleep(1);
	if (cmd)
		trans = data << 7;
	else
		trans = 0x8000 | data << 7;
	spi_write16_blocking(spi1, &trans, 1);// 9 data bits 
	EVE_sleep(1);
	gpio_put(LCD_SPI1_CS, 1);
}

void spi_write(spi_inst_t* spi, uint8_t* data, uint32_t length)
{
	//send command
	send_data(data[0], true);

	if (length > 1)
	{
		for (int i = 1; i < length; i++)
			send_data(data[i], false);
	}
}

void LCD_init()
{
	uint8_t data1[6] = { 0xFF, 0x77, 0x01, 0x00, 0x00, 0x10 };
	uint8_t data2[3] = { 0xC0, 0x3B, 0x00 };
	uint8_t data3[3] = { 0xC1, 0x0B, 0x02 };
	uint8_t data4[3] = { 0xC2, 0x07, 0x02 };
	uint8_t data5[2] = { 0xCC, 0x10 };
	uint8_t data6[17] = { 0xB0, 0x00, 0x11, 0x16, 0x0E, 0x11, 0x06, 0x05, 0x09, 0x08, 0x21, 0x06, 0x13, 0x10, 0x29, 0x31, 0x18 };
	uint8_t data7[17] = { 0xB1, 0x00, 0x11, 0x16, 0x0E, 0x11, 0x07, 0x05, 0x09, 0x09, 0x21, 0x05, 0x13, 0x11, 0x2A, 0x31, 0x18 };
	uint8_t data8[6] = { 0xFF, 0x77, 0x01, 0x00, 0x00, 0x11 };
	uint8_t data9[2] = { 0xB0, 0x6D };
	uint8_t data10[2] = { 0xB1, 0x37 };
	uint8_t data11[2] = { 0xB2, 0x81 };
	uint8_t data12[2] = { 0xB3, 0x80 };
	uint8_t data13[2] = { 0xB5, 0x43 };
	uint8_t data14[2] = { 0xB7, 0x85 };
	uint8_t data15[2] = { 0xB8, 0x20 };
	uint8_t data16[2] = { 0xC1, 0x78 };
	uint8_t data17[2] = { 0xC2, 0x78 };
	uint8_t data39[2] = { 0xCD, 0x08 };
	uint8_t data38[2] = { 0xC3, 0x8C };
	uint8_t data18[2] = { 0xD0, 0x88 };
	uint8_t data19[4] = { 0xE0, 0x00, 0x00, 0x02 };
	uint8_t data20[6] = { 0xE1, 0x03, 0xA0, 0x00, 0x00, 0x04 };
	uint8_t data21[14] = { 0xE2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t data22[5] = { 0xE3, 0x00, 0x00, 0x11, 0x00 };
	uint8_t data23[3] = { 0xE4, 0x22, 0x00 };
	uint8_t data24[17] = { 0xE5, 0x05, 0xEC, 0xA0, 0xA0, 0x07, 0xEE, 0xA0, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t data25[5] = { 0xE6, 0x00, 0x00, 0x11, 0x00 };
	uint8_t data26[3] = { 0xE7, 0x22, 0x00 };
	uint8_t data27[17] = { 0xE8, 0x06, 0xED, 0xA0, 0xA0, 0x08, 0xEF, 0xA0, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t data28[8] = { 0xEB, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00 };
	uint8_t data29[17] = { 0xED, 0xFF, 0xFF, 0xFF, 0xBA, 0x0A, 0xBF, 0x45, 0xFF, 0xFF, 0x54, 0xFB, 0xA0, 0xAB, 0xFF, 0xFF,0xFF};
	uint8_t data30[7] = { 0xEF, 0x10, 0x0D, 0x04, 0x08, 0x3F, 0x1F };
	uint8_t data31[6] = { 0xFF, 0x77, 0x01, 0x00, 0x00, 0x13 };
	uint8_t data32[2] = { 0xEF, 0x08 };
	uint8_t data33[6] = { 0xFF, 0x77, 0x01, 0x00, 0x00, 0x00 };
	uint8_t data34[2] = { 0x36, 0x00 };
	uint8_t data35[2] = { 0x3A, 0x66,  };
	uint8_t data36 = 0x11;
	uint8_t data37 = 0x29;


	spi_init(spi1, 1000 * 1000);
	gpio_set_function(LCD_SPI1_SCK, GPIO_FUNC_SPI);
	gpio_set_function(LCD_SPI1_MOSI, GPIO_FUNC_SPI);
	spi_set_format(spi1, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

	/* Chip select is active-low, so we'll initialise it to a driven-high state */
	gpio_init(LCD_SPI1_CS);
	gpio_set_dir(LCD_SPI1_CS, GPIO_OUT);
	gpio_put(LCD_SPI1_CS, 1);

	// reset LCD
	gpio_init(LCD_RESET);
	gpio_set_dir(LCD_RESET, GPIO_OUT);
	gpio_put(LCD_RESET, 1);
	EVE_sleep(20);
	gpio_put(LCD_RESET, 0);
	EVE_sleep(20);
	gpio_put(LCD_RESET, 1);
	EVE_sleep(20);

	spi_write(spi1, data1, sizeof(data1));
	spi_write(spi1, data2, sizeof(data2));
	spi_write(spi1, data3, sizeof(data3));
	spi_write(spi1, data4, sizeof(data4));
	spi_write(spi1, data6, sizeof(data6));
	spi_write(spi1, data7, sizeof(data7));
	spi_write(spi1, data8, sizeof(data8));
	spi_write(spi1, data9, sizeof(data9));
	spi_write(spi1, data10, sizeof(data10));
	spi_write(spi1, data11, sizeof(data11));
	spi_write(spi1, data12, sizeof(data12));
	spi_write(spi1, data13, sizeof(data13));
	spi_write(spi1, data14, sizeof(data14));
	spi_write(spi1, data15, sizeof(data15));
	spi_write(spi1, data16, sizeof(data16));
	spi_write(spi1, data17, sizeof(data17));
	spi_write(spi1, data38, sizeof(data38));
	spi_write(spi1, data39, sizeof(data39));
	spi_write(spi1, data18, sizeof(data18));
	spi_write(spi1, data19, sizeof(data19));
	spi_write(spi1, data20, sizeof(data20));
	spi_write(spi1, data21, sizeof(data21));
	spi_write(spi1, data22, sizeof(data22));
	spi_write(spi1, data23, sizeof(data23));
	spi_write(spi1, data24, sizeof(data24));
	spi_write(spi1, data25, sizeof(data25));
	spi_write(spi1, data26, sizeof(data26));
	spi_write(spi1, data27, sizeof(data27));
	spi_write(spi1, data28, sizeof(data28));
	spi_write(spi1, data29, sizeof(data29));
	spi_write(spi1, data30, sizeof(data30));
	spi_write(spi1, data31, sizeof(data31));
	spi_write(spi1, data32, sizeof(data32));
	spi_write(spi1, data33, sizeof(data33));
	spi_write(spi1, data34, sizeof(data34));
	spi_write(spi1, data35, sizeof(data35));
	EVE_sleep(100);
	spi_write(spi1, &data36, 1);
	EVE_sleep(100);
	spi_write(spi1, &data37, 1);

}

/*************************** GPIO event process (encoder & button) *******************************/
void gpio_callback(uint gpio, uint32_t events)
{
	static uint32_t prev_time = 0;
	static int prev_level_A = 0;
	int level_A = gpio_get(ENCODER_PIN_A);
	int level_B = gpio_get(ENCODER_PIN_B);
	int level_btn = gpio_get(BUTTON_PIN);
	static int prev_level_btn = 0;

	if (gpio == ENCODER_PIN_A)
	{
		uint32_t time = EVE_millis();
		if (time - prev_time > 5)
		{
			prev_time = time;
			if ((level_A == 0) && (prev_level_A == 1))
			{
				if (level_B == 1)
				{
					encoder--;
					eve_printf_debug("left\n");
				}
				else
				{
					encoder++;
					eve_printf_debug("right\n");
				}
			}
		}
		prev_level_A = level_A;
	}
	else if (gpio == BUTTON_PIN)
	{
		if ((level_btn == 1) && (prev_level_btn == 0))
		{
			button--;
			eve_printf_debug("release\n");
		}
		else if ((level_btn == 0) && (prev_level_btn == 1))
		{
			button++;
			eve_printf_debug("press\n");
		}
		else
		{
			button = 0;
			eve_printf_debug("no action\n");
		}
		prev_level_btn = level_btn;
	}
}

/********************************* encoder related *************************************/
void encoder_read(int* data)
{
	static int16_t prev = 0; // save the value from last polling

	if (encoder != prev)
	{
		if (encoder > prev)
			*data = 1;
		else
			*data = -1;
		prev = encoder;
	}
	else
		*data = 0;
}


void encoder_init()
{
	gpio_set_irq_enabled_with_callback(ENCODER_PIN_A, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

}

/********************************* button related *************************************/
void button_read(int* data)
{
	static int16_t prev = 0; // save the value from last polling

	if (button != prev)
	{
		if (button == -1)
			*data = 2;
		else
			*data = button;
		prev = button;
	}
}

void button_init()
{
	gpio_init(BUTTON_PIN);
	gpio_set_dir(BUTTON_PIN, GPIO_IN);
	gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
}

void EVE_IDM2040_21R_RP2040_bootup()
{
	LCD_init();
	encoder_init();
	button_init();
}

#endif

/*end of file*/
