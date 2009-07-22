/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: ata_generic.c
 * Desc: Generic driver for ATA controllers
 *
 * This file is part of TempOS.
 *
 * TempOS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * TempOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <drv/ata_generic.h>
#include <tempos/kernel.h>
#include <tempos/timer.h>
#include <tempos/jiffies.h>
#include <tempos/delay.h>
#include <drv/i8042.h>
#include <x86/irq.h>
#include <x86/io.h>


#define TIMEOUT		(jiffies + (HZ / 40)) /* timeout in 40ms */


#define PRI_BUS 	0
#define SEC_BUS 	1

#define MASTER_DEV	0
#define SLAVE_DEV	1

#define REG_DATA	0
#define REG_FR		1
#define REG_SC		2
#define REG_SADDR1	3
#define REG_SADDR2	4
#define REG_SADDR3	5
#define REG_DC		6
#define REG_CMD		7
#define REG_ASTATUS	8

#define BSY_BIT		0x80
#define DRDY_BIT	0x40


/* ATA devices information */
ata_dev_info ata_devices[4];


/* Default I/O ports for ATA controller */
static uint16_t pio_ports[2][9] = {
		/* Primary Bus */
		{0x1F0, 0x1F1, 0x1F2, 0x1F3, 0x1F4, 0x1F5, 0x1F6, 0x1F7, 0x3F6},
		/* Secondary Bus */
		{0x170, 0x171, 0x172, 0x173, 0x174, 0x175, 0x176, 0x177, 0x376}
};


static void send_cmd(uchar8_t bus, uchar8_t command);

static void wait_bus(uchar8_t bus);

static void set_device(uchar8_t bus, uchar8_t device);

static int get_dev_info(uchar8_t bus, ata_dev_info *devinfo);


/**
 * init_ata_generic
 *
 * Initialize the generic driver for ATA controllers.
 * This function will look for disks connected to the bus
 * and initialize them.
 */
void init_ata_generic(void)
{
	int i;
	char drvl = 'a';
	uchar8_t dev, bus;

	kprintf(KERN_INFO "Initializing generic ATA controller...\n");

	/* Probe primary and secondary bus */
	/* We use polling just on initialization. Data transfers will use IRQ. */

	/* NOTE: The correct way it's use PCI bus to get the information about
	   ATA bus (PIO ports and so on), but we don't have PCI support yet :-( */

	/* Device IDENTIFY */
	bus = PRI_BUS;
	for(i=0; i<4; i++, drvl++) {
		ata_devices[i].present = 0;

		if(i >= 2) {
			/* Secondary BUS */
			bus = SEC_BUS;
		}

		/* Select device */
		if(i == 0 || i == 2) {
			dev = MASTER_DEV;
		} else {
			dev = SLAVE_DEV;
		}
		set_device(bus, dev);

		/* Identify */
		send_cmd(bus, 0xEC);
		wait_bus(bus);

		//kprintf("  DEBUG: 0x%x -- 0x%x\n", inb(pio_ports[bus][REG_SADDR2]), inb(pio_ports[bus][REG_SADDR3]));

		if(inb(pio_ports[bus][REG_SADDR2]) == 0x14 &&
					inb(pio_ports[bus][REG_SADDR3]) == 0xEB) {

			kprintf(" hd%c: CD/DVD-ROM detected.\n", drvl);

		} else {

			if( (pio_ports[bus][REG_ASTATUS] & 0x01) == 0 ) {


				if( get_dev_info(bus, &ata_devices[i]) ) {

					if( (ata_devices[i].type & ATA_DEVICE) == 0 ) {

						kprintf(" hd%c: Device found\n", drvl);
						ata_devices[i].present = 1;

						/* Show information */
						kprintf("       Model: %s\n", ata_devices[i].model);
					} else {
						kprintf(" hd%c: Device not found.\n", drvl);
					}
				} else {
					kprintf("Error on get device information\n");
					return;
				}
			} else {
				kprintf(" hd%c: Device not found.\n", drvl);
			}
		}
	}
}


/**
 * send_cmd
 *
 * Send a command
 */
static void send_cmd(uchar8_t bus, uchar8_t command)
{
	if(bus == PRI_BUS) {
		outb(command, pio_ports[PRI_BUS][REG_CMD]);
	} else if(bus == SEC_BUS) {
		outb(command, pio_ports[SEC_BUS][REG_CMD]);
	}
}


/**
 * wait_bus
 *
 * Wait while bus is busy (polling)
 */
static void wait_bus(uchar8_t bus)
{
	long32_t timeout = TIMEOUT;

	while( (inb(pio_ports[bus][REG_CMD]) & BSY_BIT) &&
					!time_after(jiffies, timeout) );
}


/**
 * set_device
 *
 * Select active device (master or slave) on the bus (doing the proper delay)
 */
static void set_device(uchar8_t bus, uchar8_t device)
{
	switch(bus) {
		case PRI_BUS:
				if(device == MASTER_DEV) {
					outb(0x00, pio_ports[PRI_BUS][REG_DC]);
				} else if(device == SLAVE_DEV) {
					outb(0x10, pio_ports[PRI_BUS][REG_DC]);
				}

		case SEC_BUS:
				if(device == MASTER_DEV) {
					outb(0x00, pio_ports[SEC_BUS][REG_DC]);
				} else if(device == SLAVE_DEV) {
					outb(0x10, pio_ports[SEC_BUS][REG_DC]);
				}

	}
	udelay(400);
}


/**
 * get_dev_info
 *
 * Get and parse device information
 */
static int get_dev_info(uchar8_t bus, ata_dev_info *devinfo)
{
	int i, p;
	uint16_t tmp;

	/* Type */
	wait_bus(bus);
	devinfo->type = inw(pio_ports[bus][REG_DATA]);

	/* Not used */
	for(i=1; i<=9; i++) {
		wait_bus(bus);
		inw(pio_ports[bus][REG_DATA]);
	}

	/* Serial */
	p = 0;
	for(i=10; i<=19; i++) {
		wait_bus(bus);
		tmp = inw(pio_ports[bus][REG_DATA]);

		devinfo->serial[p] = (uchar8_t)(tmp >> 8);
		p++;
		devinfo->serial[p] = (uchar8_t)(tmp & 0x00FF);
		p++;
	}
	devinfo->serial[19] = '\0';

	/* Not used */
	for(i=20; i<=22; i++) {
		wait_bus(bus);
		inw(pio_ports[bus][REG_DATA]);
	}

	/* Firmware revision */
	p = 0;
	for(i=23; i<=26; i++) {
		wait_bus(bus);
		tmp = inw(pio_ports[bus][REG_DATA]);

		devinfo->firmware_rev[p] = (uchar8_t)(tmp >> 8);
		p++;
		devinfo->firmware_rev[p] = (uchar8_t)(tmp & 0x00FF);

		p++;
	}
	devinfo->firmware_rev[7] = '\0';

	/* Model */
	p = 0;
	for(i=27; i<=46; i++) {
		wait_bus(bus);
		tmp = inw(pio_ports[bus][REG_DATA]);

		devinfo->model[p] = (uchar8_t)(tmp >> 8);
		p++;
		devinfo->model[p] = (uchar8_t)(tmp & 0x00FF);
		p++;
	}
	devinfo->model[39] = '\0';

	/* Maximum number of sectors that shall be transferred 
	   per interrupt on READ/WRITE MULTIPLE commands */
	wait_bus(bus);
	devinfo->maxt_secs = inw(pio_ports[bus][REG_DATA]);

	/* Reserved */
	wait_bus(bus);
	inw(pio_ports[bus][REG_DATA]);

	/* Capabilities */
	for(i=49; i<=50; i++) {
		wait_bus(bus);
		devinfo->capabilities[i - 49] = inw(pio_ports[bus][REG_DATA]);
	}

	/* Not used */
	for(i=51; i<=62; i++) {
		wait_bus(bus);
		inw(pio_ports[bus][REG_DATA]);
	}

	/* Multiword DMA */
	wait_bus(bus);
	devinfo->mword_dma = inw(pio_ports[bus][REG_DATA]);

	/* Not used */
	for(i=64; i<=79; i++) {
		wait_bus(bus);
		inw(pio_ports[bus][REG_DATA]);
	}

	/* Major version number */
	wait_bus(bus);
	devinfo->major_ver = inw(pio_ports[bus][REG_DATA]);

	/* Minor version number */
	wait_bus(bus);
	devinfo->minor_ver = inw(pio_ports[bus][REG_DATA]);

	/* Command set supported */
	for(i=82; i<=87; i++) {
		wait_bus(bus);
		devinfo->cmds_supported[i - 82] = inw(pio_ports[bus][REG_DATA]);
	}

	/* Ultra DMA */
	wait_bus(bus);
	devinfo->ultra_dma = inw(pio_ports[bus][REG_DATA]);

	/* Not used */
	for(i=89; i<=99; i++) {
		wait_bus(bus);
		inw(pio_ports[bus][REG_DATA]);
	}

	/* Not used */
	for(i=100; i<=255; i++) {
		wait_bus(bus);
		inw(pio_ports[bus][REG_DATA]);
	}

	return(1);
}

