/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: ata_generic.c
 * Desc: Generic driver for ATA controllers (and PATA devices)
 * Note: This driver uses ATA/ATAPI-6 specification.
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

#include <tempos/kernel.h>
#include <tempos/timer.h>
#include <tempos/jiffies.h>
#include <tempos/delay.h>
#include <tempos/wait.h>
#include <fs/device.h>
#include <fs/dev_numbers.h>
#include <fs/partition.h>
#include <drv/ata_generic.h>
#include <drv/i8042.h>
#include <arch/irq.h>
#include <arch/io.h>
#include <linkedl.h>

#define SECTOR_HALF_SIZE (SECTOR_SIZE / 2)

#define TIMEOUT		(jiffies + (HZ / 40)) /* timeout in 40ms */

#define ATA_PRI_IRQ	14
#define ATA_SEC_IRQ	15

#define LBA_UHIGH(addr)		((addr >> 24) & 0x07)
#define LBA_BYTE(addr, n)	((addr >> (n * 8)) & 0xFF)

#define PRI_BUS		0
#define SEC_BUS		1

#define MASTER_DEV	0
#define SLAVE_DEV	1

#define REG_DATA	0
#define REG_FERR	1
#define REG_SC		2
#define REG_SADDR1	3
#define REG_SADDR2	4
#define REG_SADDR3	5
#define REG_DC		6
#define REG_CMD		7
#define REG_ASTATUS	8

#define BSY_BIT		0x80
#define DF_BIT		0x20
#define DRDY_BIT	0x40
#define DRQ_BIT     0x08
#define ABRT_BIT	0x04

#define CMD_RESET				0x08
#define CMD_IDENTIFY			0xEC
#define CMD_READ_SECTORS		0x20
#define CMD_READ_SECTORS_EXT	0x24
#define CMD_WRITE_SECTORS		0x30
#define CMD_WRITE_SECTORS_EXT	0x34
#define CMD_CACHE_FLUSH			0xE7


/** ATA devices information */
static ata_dev_info ata_devices[4];

/**
 * Linked list to blocks operation requests.
 * There is one list for each device:
 * 0 - hda
 * 1 - hdb
 * 2 - hdc
 * 3 - hdd
 */
static llist *blk_queue[4];

/** Hash queue indexes for each disk */
static int blk_hash_queue[4];

/** Default I/O ports for ATA controller */
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

static void ata_handler1(int id, pt_regs *regs);

static void ata_handler2(int id, pt_regs *regs);

static int read_hd_sector(int major, int device, uint64_t addr);

static int write_hd_sector(int major, int device, uint64_t addr, uint16_t *sector);

/**
 * Initialize the generic driver for ATA controllers.
 * This function will look for disks connected to the bus
 * and initialize them.
 */
void init_ata_generic(void)
{
	int i;
	char drvl = 'a';
	uint64_t size;
	uchar8_t dev, bus;
	uchar8_t sc, saddr1, saddr2, saddr3;
	buff_header_t mbr;	

	kprintf(KERN_INFO "Initializing generic ATA controller...\n");


	/* Probe primary and secondary bus */
	/* We use polling just on initialization. Data transfers will use IRQ. */

	/* NOTE: The correct way it's use PCI bus to get the information about
	   ATA bus (PIO ports and so on), but we don't have PCI support yet :-( */

	/* Device IDENTIFY */
	bus = PRI_BUS;
	for(i=0; i<4; i++, drvl++) {
		ata_devices[i].flags = 0;

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

		/* Reset */
		send_cmd(bus, CMD_RESET);
		wait_bus(bus);

		/* Check driver type */
		sc     = inb(pio_ports[bus][REG_SC]);
		saddr1 = inb(pio_ports[bus][REG_SADDR1]);
		saddr2 = inb(pio_ports[bus][REG_SADDR2]);
		saddr3 = inb(pio_ports[bus][REG_SADDR3]);

		/*if(sc != 0x01 && saddr1 != 0x01) {
			kprintf(" hd%c: Device not found.\n", drvl);
			continue;
		}*/

		if(saddr2 == 0x14 && saddr3 == 0xEB) {
			kprintf(KERN_INFO " hd%c: CD/DVD-ROM detected.\n", drvl);
		} else {
			/* Identify */
			send_cmd(bus, CMD_IDENTIFY);
			wait_bus(bus);

			if( (pio_ports[bus][REG_ASTATUS] & 0x01) != 0 ||
				(pio_ports[bus][REG_FERR] & ABRT_BIT) != 0 ) {

					kprintf(KERN_INFO " hd%c: Device not found.\n", drvl);
					continue;
			}


			if( get_dev_info(bus, &ata_devices[i]) ) {

				if( (ata_devices[i].type & ATA_DEVICE) != 0 ) {
					kprintf(KERN_INFO " hd%c: Device not found.\n", drvl);
					continue;
				}

				/* Check for LBA */
				if( (ata_devices[i].capabilities[0] & SUPPORT_LBA) == 0 ) {
					kprintf(KERN_INFO " hd%c: Device not found.\n", drvl);
					continue;
				} else {
					kprintf(KERN_INFO " hd%c: Device found: ", drvl);

					/* Check for LBA48 */
					if( (ata_devices[i].cmds_supported[1] & SUPPORT_LBA48) != 0 ) {
						kprintf(KERN_INFO "LBA48");
						ata_devices[i].flags |= LBA48;

						size  = ata_devices[i].max_lba48[0];
						size |= (ata_devices[i].max_lba48[1] << 16);
						size |= ((uint64_t)ata_devices[i].max_lba48[2] << 32);
						size |= ((uint64_t)ata_devices[i].max_lba48[3] << 48);
					} else {
						kprintf(KERN_INFO "LBA");

						size  = ata_devices[i].max_secs[0];
						size |= (ata_devices[i].max_secs[1] << 16);
					}
					ata_devices[i].sectors = size;
				}

				/* Check for DMA */
				if( (ata_devices[i].capabilities[0] & SUPPORT_DMA) != 0 ) {
					kprintf(KERN_INFO ", DMA");
				}
				kprintf(KERN_INFO ", %ld sectors\n", ata_devices[i].sectors);

				ata_devices[i].flags |= PRESENT;

				/* Show information */
				kprintf(KERN_INFO "       Model: %s\n", ata_devices[i].model);
			} else {
				kprintf(KERN_WARNING "Error on get device information\n");
				continue;
			}
		}
	}

	/* Block requests lists */
	for (i = 0; i < 4; i++) {
		llist_create(&blk_queue[i]);
	}

	/* Register IRQs */
	if( request_irq(ATA_PRI_IRQ, ata_handler1, SA_SHIRQ, "ata-primary") < 0) {
		kprintf(KERN_ERROR "Error on register IRQ %d\n", ATA_PRI_IRQ);
	}
	if( request_irq(ATA_SEC_IRQ, ata_handler2, SA_SHIRQ, "ata-secondary") < 0) {
		kprintf(KERN_ERROR "Error on register IRQ %d\n", ATA_SEC_IRQ);
	}

	/* Now, parse partition table for each found device */
	/** FIXME: TempOS supports only one device per bus.
	 *  To support more devices we need to check PCI bus to
	 *  know which device generates the interrupt */
	mbr.addr = 0;
	if ((ata_devices[0].flags & PRESENT) != 0) {

		blk_hash_queue[0] = create_hash_queue(DEVMAJOR_ATA_PRI, ata_devices[0].sectors);
		if (blk_hash_queue[0] < 0) {
			panic("Could not create hash queue for disk blocks!");
		}

		read_ata_sector(DEVMAJOR_ATA_PRI, DEVNUM_HDA, &mbr);
		//parse_mbr(&mbr.data);

	}
	if ((ata_devices[2].flags & PRESENT) != 0) {
		
		blk_hash_queue[2] = create_hash_queue(DEVMAJOR_ATA_SEC, ata_devices[2].sectors);
		if (blk_hash_queue[2] < 0) {
			panic("Could not create hash queue for disk blocks!");
		}

		read_ata_sector(DEVMAJOR_ATA_SEC, DEVNUM_HDC, &mbr);
		write_hd_sector(DEVMAJOR_ATA_SEC, DEVNUM_HDC, 0, &mbr.data);
		//parse_mbr(&mbr);
	}
}


/**
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
 * Wait while bus is busy (polling)
 */
static void wait_bus(uchar8_t bus)
{
	int32_t timeout = TIMEOUT;

	while( (inb(pio_ports[bus][REG_CMD]) & BSY_BIT) &&
					!time_after(jiffies, timeout) );
}


/**
 * Select active device (master or slave) on the bus (doing the proper delay)
 */
static void set_device(uchar8_t bus, uchar8_t device)
{
	switch(bus) {
		case PRI_BUS:
			if(device == MASTER_DEV) {
				outb(0x40, pio_ports[PRI_BUS][REG_DC]);
			} else if(device == SLAVE_DEV) {
				outb(0x50, pio_ports[PRI_BUS][REG_DC]);
			}
		break;

		case SEC_BUS:
			if(device == MASTER_DEV) {
				outb(0x40, pio_ports[SEC_BUS][REG_DC]);
			} else if(device == SLAVE_DEV) {
				outb(0x50, pio_ports[SEC_BUS][REG_DC]);
			}
		break;

	}
	udelay(400);
}


/**
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
	devinfo->mult_secs = inw(pio_ports[bus][REG_DATA]);

	/* Reserved */
	wait_bus(bus);
	inw(pio_ports[bus][REG_DATA]);

	/* Capabilities */
	for(i=49; i<=50; i++) {
		wait_bus(bus);
		devinfo->capabilities[i - 49] = inw(pio_ports[bus][REG_DATA]);
	}

	/* Not used */
	for(i=51; i<=58; i++) {
		wait_bus(bus);
		inw(pio_ports[bus][REG_DATA]);
	}

	/* Multiple sector setting */
	wait_bus(bus);
	devinfo->mult_sec = inw(pio_ports[bus][REG_DATA]);

	/* Total number of user addressable sectors */
	wait_bus(bus);
	devinfo->max_secs[0] = inw(pio_ports[bus][REG_DATA]);
	wait_bus(bus);
	devinfo->max_secs[1] = inw(pio_ports[bus][REG_DATA]);

	/* 62: Note used */
	wait_bus(bus);
	inw(pio_ports[bus][REG_DATA]);

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

	/* LBA48 max */
	for(i=100; i<=103; i++) {
		wait_bus(bus);
		devinfo->max_lba48[i - 100] = inw(pio_ports[bus][REG_DATA]);
	}

	/* Not used */
	for(i=104; i<=255; i++) {
		wait_bus(bus);
		inw(pio_ports[bus][REG_DATA]);
	}

	return(1);
}


/**
 * Read a sector from device (low level function)
 *
 * \param major Bus - Primary or Secondary IDE
 * \param device Master or Slave
 * \param addr LBA 48bit sector address
 *
 * \note This function will just request to read the sector.
 * ATA controller will generate a interrupt when it's done.
 */
static int read_hd_sector(int major, int device, uint64_t addr)
{
	uchar8_t dc;
	uchar8_t bus, dev;

	if (major == DEVMAJOR_ATA_PRI) {
		bus = PRI_BUS;

		switch(device) {
			case DEVNUM_HDA:
				dev = MASTER_DEV;
				break;

			case DEVNUM_HDB:
				dev = SLAVE_DEV;
				break;

			default:
				return -1;
		}
	} else if(major == DEVMAJOR_ATA_SEC) {
		bus = SEC_BUS;

		switch(device) {
			case DEVNUM_HDC:
				dev = MASTER_DEV;
				break;

			case DEVNUM_HDD:
				dev = SLAVE_DEV;
				break;

			default:
				return -1;
		}
	} else {
		return -1;
	}

	set_device(bus, dev);

	outb(0x00, pio_ports[bus][REG_SC]);
	outb(LBA_BYTE(addr, 4), pio_ports[bus][REG_SADDR1]);
	outb(LBA_BYTE(addr, 5), pio_ports[bus][REG_SADDR2]);
	outb(LBA_BYTE(addr, 6), pio_ports[bus][REG_SADDR3]);

	outb(0x01, pio_ports[bus][REG_SC]);
	outb(LBA_BYTE(addr, 0), pio_ports[bus][REG_SADDR1]);
	outb(LBA_BYTE(addr, 2), pio_ports[bus][REG_SADDR2]);
	outb(LBA_BYTE(addr, 3), pio_ports[bus][REG_SADDR3]);

	dc = (inb(pio_ports[bus][REG_DC]) & 0xF0) | 0x40;
	outb(dc, pio_ports[bus][REG_DC]);

	send_cmd(bus, CMD_READ_SECTORS_EXT);
	wait_bus(bus);

	if((inb(pio_ports[bus][REG_ASTATUS]) & DF_BIT) != 0 ||
			inb(pio_ports[bus][REG_FERR] & ABRT_BIT) != 0) {
		kprintf(KERN_ERROR "ATA_DRIVER ERROR: Could not read sectors\n");
	}

	return 0;
}


/**
 * Write sectors to device (low level function)
 *
 * \param major Bus - Primary or Secondary IDE
 * \param device Master or Slave
 * \param addr LBA 48bit sector address
 *
 * \note This function write a sector to device.
 */
static int write_hd_sector(int major, int device, uint64_t addr, uint16_t *sector)
{
	int i;
	uchar8_t dc;
	uchar8_t bus, dev;

	if (major == DEVMAJOR_ATA_PRI) {
		bus = PRI_BUS;

		switch(device) {
			case DEVNUM_HDA:
				dev = MASTER_DEV;
				break;

			case DEVNUM_HDB:
				dev = SLAVE_DEV;
				break;

			default:
				return -1;
		}
	} else if(major == DEVMAJOR_ATA_SEC) {
		bus = SEC_BUS;

		switch(device) {
			case DEVNUM_HDC:
				dev = MASTER_DEV;
				break;

			case DEVNUM_HDD:
				dev = SLAVE_DEV;
				break;

			default:
				return -1;
		}
	} else {
		return -1;
	}

	set_device(bus, dev);

	outb(0x00, pio_ports[bus][REG_SC]);
	outb(LBA_BYTE(addr, 4), pio_ports[bus][REG_SADDR1]);
	outb(LBA_BYTE(addr, 5), pio_ports[bus][REG_SADDR2]);
	outb(LBA_BYTE(addr, 6), pio_ports[bus][REG_SADDR3]);

	outb(0x01, pio_ports[bus][REG_SC]);
	outb(LBA_BYTE(addr, 0), pio_ports[bus][REG_SADDR1]);
	outb(LBA_BYTE(addr, 2), pio_ports[bus][REG_SADDR2]);
	outb(LBA_BYTE(addr, 3), pio_ports[bus][REG_SADDR3]);

	dc = (inb(pio_ports[bus][REG_DC]) & 0xF0) | 0x40;
	outb(dc, pio_ports[bus][REG_DC]);

	send_cmd(bus, CMD_WRITE_SECTORS_EXT);
	wait_bus(bus);

	if((inb(pio_ports[bus][REG_ASTATUS]) & DF_BIT) != 0 ||
			inb(pio_ports[bus][REG_FERR] & ABRT_BIT) != 0) {
		kprintf(KERN_ERROR "ATA_DRIVER ERROR: Could not write sectors\n");
		return -1;
	}
	
	for(i=0; i<SECTOR_HALF_SIZE; i++) {
		wait_bus(bus);
		outw(sector[i], pio_ports[bus][REG_DATA]);
		udelay(1);
	}
	send_cmd(bus, CMD_CACHE_FLUSH);
	wait_bus(bus);

	return 0;
}


/**
 * Handler for disk controller interrupts.
 * This function just receive the block from controller, remove
 * it from blocks queue and if necessary, tell the controller
 * to read the next block of the list.
 * \note If you are going to implement a arm's disk algorithm, DO NOT change
 * this function, but do it in read_ata_sector, so you can rearrange the list
 * on each block read request. This way makes more sense, and it's more sane.
 */
static void ata_handler1(int id, pt_regs *regs)
{
	uint16_t data, i;
	buff_header_t *buf;

	cli();

	/* DRQ bit is set when disk has PIO data to transfer */
	
	/* Primary */
	if( (inb(pio_ports[PRI_BUS][REG_CMD]) & DRQ_BIT) != 0 ) {
		buf = (buff_header_t*)blk_queue[0]->element;
		for(i=0; i<SECTOR_SIZE; i+=2) {
			wait_bus(PRI_BUS);
			data = inw(pio_ports[PRI_BUS][REG_DATA]);
			buf->data[i+1] = (uchar8_t)((data >> 0x08) & 0xFF);
			buf->data[i]   = (uchar8_t)(data & 0xFF);  
		}
		buf->status = BUFF_ST_VALID;
		llist_remove(&blk_queue[0], buf);

		/* Process the next block on queue */
		if (blk_queue[0] != NULL) {
			buf = (buff_header_t*)blk_queue[0]->element;
			read_hd_sector(DEVMAJOR_ATA_PRI, 0, buf->addr);
		}
	}

	/* Wakeup process waiting for this interrupt */
	sti();
	wakeup(WAIT_INT_IDE_PRI);
}

static void ata_handler2(int id, pt_regs *regs)
{
	uint16_t data, i;
	buff_header_t *buf;

	cli();

	/* DRQ bit is set when disk has PIO data to transfer */
	kprintf("HANDLER2\n");

	/* Secondary */
	if( (inb(pio_ports[SEC_BUS][REG_CMD]) & DRQ_BIT) != 0 ) {
		buf = (buff_header_t*)blk_queue[2]->element;
		for(i=0; i<SECTOR_SIZE; i+=2) {
			wait_bus(SEC_BUS);
			data = inw(pio_ports[SEC_BUS][REG_DATA]);
			buf->data[i+1] = (uchar8_t)((data >> 0x08) & 0xFF);
			buf->data[i]   = (uchar8_t)(data & 0xFF);  
		}
		buf->status = BUFF_ST_VALID;
		llist_remove(&blk_queue[2], buf);

		/* Process the next block on queue */
		if (blk_queue[2] != NULL) {
			buf = (buff_header_t*)blk_queue[2]->element;
			read_hd_sector(DEVMAJOR_ATA_SEC, 0, buf->addr);
		}
	}

	/* Wakeup process waiting for this interrupt */
	sti();
	wakeup(WAIT_INT_IDE_PRI);
}

/**
 * Read a sector from hard disk.
 *
 * \param major Bus - Primary or Secondary IDE
 * \param device Master or Slave
 * \param buf Buffer structure that should contains block address, 
 *            and space for block data.
 * \note This function will sleep until the block becomes available.
 */
int read_ata_sector(int major, int device, buff_header_t *buf)
{
	uchar8_t dev;

	if (major == DEVMAJOR_ATA_PRI) {
		switch(device) {
			case DEVNUM_HDA:
				dev = 0;
				break;

			case DEVNUM_HDB:
				dev = 1;
				break;

			default:
				return -1;
		}
	} else if(major == DEVMAJOR_ATA_SEC) {
		switch(device) {
			case DEVNUM_HDC:
				dev = 2;
				break;

			case DEVNUM_HDD:
				dev = 3;
				break;

			default:
				return -1;
		}
	} else {
		return -1;
	}


	cli();
	/* First, mark block as busy */
	buf->status = BUFF_ST_BUSY;

	if (blk_queue[dev] == NULL) {
		/** The queue is empty, so we can process this block now! */
		llist_add(&blk_queue[dev], buf); 
		read_hd_sector(major, device, buf->addr);
	} else {
		/** The queue is not empty, we will just add the block to the queue,
		 *  so it will be process later, by interrupt handler */
		llist_add(&blk_queue[dev], buf);
	}
	sti();
	

	/** Wait block to become available */
	if (major == DEVMAJOR_ATA_PRI) {
		while(buf->status == BUFF_ST_BUSY)
			sleep_on(WAIT_INT_IDE_PRI);
	} else {
		while(buf->status == BUFF_ST_BUSY)
			sleep_on(WAIT_INT_IDE_SEC);
	}

	return 0;
}

int write_async_ata_block(int major, int device, buff_header_t *buf);

int write_sync_ata_block(int major, int device, buff_header_t *buf);

