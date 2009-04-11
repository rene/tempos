/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: gdt.c
 * Desc: Functions to setup GDT table 
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

#include <x86/gdt.h>


/* GDT table */
gdt_t gdt_table[GDT_TABLE_SIZE];


/**
 * setup_GDT
 *
 * TempOS use a Protected Flat Model with paging for memory organization
 * and protection. The GDT have four entries:
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
 *           =====================
 *
 *
 */
void setup_GDT(void)
{
	gdt_cdseg_t *gdtentry;

	/* NULL descriptor */
	gdtentry = (gdt_cdseg_t *)&gdt_table[0];
	GDT_SET_BASE(gdtentry,  0x00000);
	GDT_SET_LIMIT(gdtentry, 0x00000);
	gdtentry->high.type        = 0;
	gdtentry->high.desc_type   = 0;
	gdtentry->high.DPL         = 0;
	gdtentry->high.present     = 0;
	gdtentry->high.avaliable   = 0;
	gdtentry->high.reserved    = 0;
	gdtentry->high.DB          = 0;
	gdtentry->high.granularity = 0;


	/* Kernel CS */
	gdtentry = (gdt_cdseg_t *)&gdt_table[1];
	GDT_SET_BASE(gdtentry,  0x00000);
	GDT_SET_LIMIT(gdtentry, 0xFFFFF);
	gdtentry->high.type        = CODE_SEG;
	gdtentry->high.desc_type   = STYPE_DC;
	gdtentry->high.DPL         = KERNEL_DPL;
	gdtentry->high.present     = 1;
	gdtentry->high.avaliable   = 0;
	gdtentry->high.reserved    = 0;
	gdtentry->high.DB          = 1; /* 32-bit segment */
	gdtentry->high.granularity = GDT_GR_4KB;

	/* Kernel DS */
	gdtentry = (gdt_cdseg_t *)&gdt_table[2];
	GDT_SET_BASE(gdtentry,  0x00000);
	GDT_SET_LIMIT(gdtentry, 0xFFFFF);
	gdtentry->high.type        = DATA_SEG;
	gdtentry->high.desc_type   = STYPE_DC;
	gdtentry->high.DPL         = KERNEL_DPL;
	gdtentry->high.present     = 1;
	gdtentry->high.avaliable   = 0;
	gdtentry->high.reserved    = 0;
	gdtentry->high.DB          = 1; /* 32-bit segment */
	gdtentry->high.granularity = GDT_GR_4KB;


	/* User CS */
	gdtentry = (gdt_cdseg_t *)&gdt_table[3];
	GDT_SET_BASE(gdtentry,  0x00000);
	GDT_SET_LIMIT(gdtentry, 0xFFFFF);
	gdtentry->high.type        = CODE_SEG;
	gdtentry->high.desc_type   = STYPE_DC;
	gdtentry->high.DPL         = USER_DPL;
	gdtentry->high.present     = 1;
	gdtentry->high.avaliable   = 0;
	gdtentry->high.reserved    = 0;
	gdtentry->high.DB          = 1; /* 32-bit segment */
	gdtentry->high.granularity = GDT_GR_4KB;

	/* User DS */
	gdtentry = (gdt_cdseg_t *)&gdt_table[4];
	GDT_SET_BASE(gdtentry,  0x00000);
	GDT_SET_LIMIT(gdtentry, 0xFFFFF);
	gdtentry->high.type        = DATA_SEG;
	gdtentry->high.desc_type   = STYPE_DC;
	gdtentry->high.DPL         = USER_DPL;
	gdtentry->high.present     = 1;
	gdtentry->high.avaliable   = 0;
	gdtentry->high.reserved    = 0;
	gdtentry->high.DB          = 1; /* 32-bit segment */
	gdtentry->high.granularity = GDT_GR_4KB;


	GDTR.table_limit = (GDT_TABLE_SIZE * sizeof(gdt_t)) - 1;
	GDTR.gdt_ptr     = (uint32_t)gdt_table;
	load_gdt();
}


inline void load_gdt(void)
{
	asm("lgdtl %0         \n"
		/* Reload DATA segment */
		"movw %1,   %%ax    \n"
		"movw %%ax, %%ds    \n"
		"movw %%ax, %%fs    \n"
		"movw %%ax, %%gs    \n"
		"movw %%ax, %%ss    \n"
		"movw %%ax, %%es    \n"
		/* Reload CODE segment */
		"ljmp %2, $reloadCS \n"
		"reloadCS:            "
			: : "m" (GDTR), "I" (KERNEL_DS), "I" (KERNEL_CS) : "eax");
}

