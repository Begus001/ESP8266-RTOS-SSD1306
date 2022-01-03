#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>
#include <stdbool.h>
#include <driver/i2c.h>

#define SSD_ADDR_MODE      0x20
#define SSD_PAGE_ADDRING   0x02
#define SSD_SET_LCOL_ADDR  0x00
#define SSD_SET_RCOL_ADDR  0x10
#define SSD_SET_PAGE_ADDR  0xB0

#define SSD_DISPLAY_ON     0xAF
#define SSD_DISPLAY_OFF    0xAE

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef struct ssd1306 {
	u8 width;
	u8 height;
	u8 *fb;
} ssd1306_t;

typedef enum {
	SSDCOLOR_BLACK,
	SSDCOLOR_WHITE,
	SSDCOLOR_INVERT,
} ssd_color_t;

/**
 * @brief Send the whole framebuffer to the display;
 * Only use if the framebuffer of the ssd1306 object was manipulated manually
 * 
 * @param self Pointer to ssd1306 object
 */
void ssd_update(ssd1306_t *self);

/**
 * @brief Clear the enitre screen
 * 
 * @param self Pointer to ssd1306 object
 */
void ssd_clear(ssd1306_t *self);

/**
 * @brief Set the GDDRAM address of the display
 * 
 * @param self Pointer to ssd1306 object
 * @param page Page to address
 * @param col Column to address
 */
void ssd_set_pos(ssd1306_t *self, u8 page, u8 col);

/**
 * @brief Draw a pixel
 * 
 * @param self Pointer to ssd1306 object
 * @param x X coordinate of pixel to draw
 * @param y Y coordinate of pixel to draw
 */
void ssd_draw_pixel(ssd1306_t *self, u8 x, u8 y);

/**
 * @brief Draw a line
 * 
 * @param self Pointer to ssd1306 object
 * @param x0 X coordinate of line start
 * @param y0 Y coordinate of line start
 * @param x1 X coordinate of line end
 * @param y1 Y coordinate of line end
 */
void ssd_draw_line(ssd1306_t *self, u8 x0, u8 y0, u8 x1, u8 y1);

/**
 * @brief Create a new ssd1306 object
 * 
 * @param width Width of the screen
 * @param height Height of the screen
 * @param scl Pin for SCL line of I2C connection
 * @param sda Pin for SDA line of I2C connection
 * @return Pointer to new ssd1306 object
 */
ssd1306_t *ssd_init(u8 width, u8 height, u8 scl, u8 sda);

/**
 * @brief Free resources of ssd1306 object
 * 
 * @param self Pointer to ssd1306 object
 */
void ssd_free(ssd1306_t *self);

#endif