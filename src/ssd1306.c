#include "../include/ssd1306.h"

#include <stdlib.h>
#include <esp_task_wdt.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

static void send(u8 data, bool command)
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	ESP_ERROR_CHECK(i2c_master_start(cmd));
	ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x3C << 1 | I2C_MASTER_WRITE, true));  // address
	if (command) {
		ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0, true));  // control byte cmd
	}
	else {
		ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x40, true));  // control byte data
	}
	ESP_ERROR_CHECK(i2c_master_write_byte(cmd, data, true));  // data
	ESP_ERROR_CHECK(i2c_master_stop(cmd));
	ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 100 / portTICK_RATE_MS));  // send
	i2c_cmd_link_delete(cmd);
}

static void sendn(u8 *data, size_t len, bool command)
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	ESP_ERROR_CHECK(i2c_master_start(cmd));
	ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x3C << 1 | I2C_MASTER_WRITE, true));  // address
	if (command) {
		ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0, true));  // control byte cmd
	}
	else {
		ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x40, true));  // control byte data
	}
	ESP_ERROR_CHECK(i2c_master_write(cmd, data, len, true));  // data
	ESP_ERROR_CHECK(i2c_master_stop(cmd));
	ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 20 / portTICK_RATE_MS));  // send
	i2c_cmd_link_delete(cmd);
}

void ssd_update(ssd1306_t *self)
{
	for (u8 i = 0; i < self->height / 8; i++) {
		ssd_set_pos(self, i, 0);
		sendn(&self->fb[i*self->width], self->width, false);
	}
}

void ssd_clear(ssd1306_t *self)
{
	memset(self->fb, 0, self->width * self->height / 8);
	ssd_update(self);
}

void ssd_set_pos(ssd1306_t *self, u8 page, u8 col)
{
	send(SSD_SET_LCOL_ADDR | (col & 0xF), true);
	send(SSD_SET_RCOL_ADDR | ((col >> 4) & 0xF), true);
	send(SSD_SET_PAGE_ADDR + page, true);
}

void ssd_set_display_on(ssd1306_t *self, bool on)
{
	send(on ? SSD_DISPLAY_ON : SSD_DISPLAY_OFF, true);
}

void ssd_draw_pixel(ssd1306_t *self, u8 x, u8 y)
{
	size_t index = y / 8 * self->width + x;
	self->fb[index] = (1 << (y % 8));
	ssd_set_pos(self, y / 8, x);
	send(1 << (y % 8), false);
}

static void swap(size_t a, size_t b)
{
	size_t c = a;
	a = b;
	b = c;
}

void ssd_draw_line(ssd1306_t *self, u8 x0, u8 y0, u8 x1, u8 y1)
{
    
}

ssd1306_t *ssd_init(u8 width, u8 height, u8 scl, u8 sda)
{
	i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER);

	i2c_config_t i2c_conf = {
		.mode = I2C_MODE_MASTER,
		.scl_io_num = scl,
		.sda_io_num = sda,
		.scl_pullup_en = 1,
		.sda_pullup_en = 1,
		.clk_stretch_tick = 300,
	};

	ESP_ERROR_CHECK_WITHOUT_ABORT(i2c_param_config(I2C_NUM_0, &i2c_conf));

	ssd1306_t *self = malloc(sizeof(ssd1306_t));
	self->width = width;
	self->height = height;
	self->fb = malloc(width * height / 8);

	ssd_set_display_on(self, true);
	send(SSD_ADDR_MODE, true);
	send(SSD_PAGE_ADDRING, true);

	ssd_clear(self);

	ssd_set_pos(self, 0, 0);

	send(0xA0, true);
	send(0xC0, true);
	
	return self;
}

void ssd_free(ssd1306_t *self)
{
	free(self->fb);
	free(self);
}