/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: gdt.c
 * Desc: Functions to setup GDT table 
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <x86/gdt.h>

/**
 * setup_GDT
 *
 * TempOS use a Protected Flat Model with paging for memory organization,
 * the GDT have four entries:
 * 		
 * 		Kernel CS (Code Segment, Ring 0) <-- For kernel code
 * 		Kernel DS (Data Segment, Ring 0) <-- For kernel data and stack
 * 		User CS   (Code Segment, Ring 3) <-- For user code
 * 		User DS   (Data Segment, Ring 3) <-- FOr user data and stack
 *
 * A null descriptor at 0 position is mandatory, so the final GDT layout is:
 *
 *           =====================
 *          |   NULL Descriptor   |
 *          |---------------------|
 *          |      KERNEL_CS      |    
 *          |---------------------|
 *          |      KERNEL_DS      |
 *          |---------------------|
 *          |       USER_CS       |
 *          |---------------------|
 *          |       USER_DS       |
 *          |---------------------|
 *          |                     |
 *           =====================
 *
 *
 */
void setup_GDT(void)
{
	gdt_t gdt_table[GDT_TABLE_SIZE];

	/* NULL descriptor */
	GDT_SET_BASE(gdt_table[0],  0x00000);
	GDT_SET_LIMIT(gdt_table[0], 0x00000);
	gdt_table[0].high.type        = 0;
	gdt_table[0].high.desc_type   = 0;
	gdt_table[0].high.DPL         = 0;
	gdt_table[0].high.present     = 0;
	gdt_table[0].high.avaliable   = 0;
	gdt_table[0].high.reserved    = 0;
	gdt_table[0].high.DB          = 0;
	gdt_table[0].high.granularity = 0;


	/* Kernel CS */
	GDT_SET_BASE(gdt_table[1],  0x00000);
	GDT_SET_LIMIT(gdt_table[1], 0xFFFFF);
	gdt_table[1].high.type        = CODE_SEG;
	gdt_table[1].high.desc_type   = STYPE_DC;
	gdt_table[1].high.DPL         = KERNEL_DPL;
	gdt_table[1].high.present     = 1;
	gdt_table[1].high.avaliable   = 0;
	gdt_table[1].high.reserved    = 0;
	gdt_table[1].high.DB          = 1; /* 32-bit segment */
	gdt_table[1].high.granularity = GDT_GR_4KB;

	/* Kernel DS */
	GDT_SET_BASE(gdt_table[2],  0x00000);
	GDT_SET_LIMIT(gdt_table[2], 0xFFFFF);
	gdt_table[2].high.type        = DATA_SEG;
	gdt_table[2].high.desc_type   = STYPE_DC;
	gdt_table[2].high.DPL         = KERNEL_DPL;
	gdt_table[2].high.present     = 1;
	gdt_table[2].high.avaliable   = 0;
	gdt_table[2].high.reserved    = 0;
	gdt_table[2].high.DB          = 1; /* 32-bit segment */
	gdt_table[2].high.granularity = GDT_GR_4KB;


	/* User CS */
	GDT_SET_BASE(gdt_table[1],  0x00000);
	GDT_SET_LIMIT(gdt_table[1], 0xFFFFF);
	gdt_table[3].high.type        = CODE_SEG;
	gdt_table[3].high.desc_type   = STYPE_DC;
	gdt_table[3].high.DPL         = USER_DPL;
	gdt_table[3].high.present     = 1;
	gdt_table[3].high.avaliable   = 0;
	gdt_table[3].high.reserved    = 0;
	gdt_table[3].high.DB          = 1; /* 32-bit segment */
	gdt_table[3].high.granularity = GDT_GR_4KB;

	/* User DS */
	GDT_SET_BASE(gdt_table[2],  0x00000);
	GDT_SET_LIMIT(gdt_table[2], 0xFFFFF);
	gdt_table[4].high.type        = DATA_SEG;
	gdt_table[4].high.desc_type   = STYPE_DC;
	gdt_table[4].high.DPL         = USER_DPL;
	gdt_table[4].high.present     = 1;
	gdt_table[4].high.avaliable   = 0;
	gdt_table[4].high.reserved    = 0;
	gdt_table[4].high.DB          = 1; /* 32-bit segment */
	gdt_table[4].high.granularity = GDT_GR_4KB;


	GDTR.table_limit = (GDT_TABLE_SIZE * sizeof(gdt_t));
	GDTR.gdt_ptr     = (uint32_t)&(gdt_table);
	load_gdt();
}


inline void load_gdt()
{
	asm("lgdtl (%0)        \n"
		/* Reload DATA segment */
		"movw $0x10, %%ax      \n"
		"movw %%ax, %%ds       \n"
		"movw %%ax, %%es       \n"
		"movw %%ax, %%fs       \n"
		"movw %%ax, %%gs       \n"
		"movw %%ax, %%ss       \n"
		/* Reload CODE segment */
		"ljmp $0x08, $reloadCS \n"
		"reloadCS:             \n"
			: : "r" (&GDTR) : "eax");
}

