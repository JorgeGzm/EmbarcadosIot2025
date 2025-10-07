/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

static int sht3x_init(void);
static int sht3x_read(void);

const struct device *const dev = DEVICE_DT_GET_ONE(sensirion_sht3xd);

static int sht3x_init(void)
{
	if (!device_is_ready(dev)) {
		printk("Device %s is not ready\n", dev->name);
		k_sleep(K_MSEC(1000));
		return -EIO;
	}

	return 0;
}

static int sht3x_read(void)
{
	int rc;
	struct sensor_value temp, hum;

	rc = sensor_sample_fetch(dev);
	if (rc == 0) {
		rc = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP,
					&temp);
	}

	if (rc == 0) {
		rc = sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY,
					&hum);
	}

	if (rc != 0) {
		printk("SHT3XD: failed: %d\n", rc);
	}

	printk("SHT3XD: %.2f Cel ; %0.2f %%RH\n",
	       sensor_value_to_double(&temp),
	       sensor_value_to_double(&hum));

	return rc;
}

int main(void)
{
	int rc;

	rc = sht3x_init();
	if(rc)
	{
		return rc;
	}

	while (1) {

		sht3x_read();
		k_sleep(K_MSEC(1000));
	}

	return 0;
}

