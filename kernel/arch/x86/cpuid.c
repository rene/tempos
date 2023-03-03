/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: cpuid.c
 * Written by: Julio Faracco
 * Desc: Get CPU information and features
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
#include <x86/cpuid.h>

#define __cpuid(level, a, b, c, d)                      \
	asm("cpuid\n\t"                                 \
			: "=a" (a), "=b" (b), "=c" (c), "=d" (d)        \
			: "0" (level))

#define __cpuid_count(level, count, a, b, c, d)         \
	asm("cpuid\n\t"                                 \
			: "=a" (a), "=b" (b), "=c" (c), "=d" (d)        \
			: "0" (level), "2" (count))

static uint32_t
__get_cpuid_max (uint32_t ext, uint32_t *sig)
{
	uint32_t eax, ebx, ecx, edx;
      	asm("pushf{l|d}\n\t"
	    "pushf{l|d}\n\t"
	    "pop{l}\t%0\n\t"
	    "mov{l}\t{%0, %1|%1, %0}\n\t"
	    "xor{l}\t{%2, %0|%0, %2}\n\t"
	    "push{l}\t%0\n\t"
	    "popf{l|d}\n\t"
	    "pushf{l|d}\n\t"
	    "pop{l}\t%0\n\t"
	    "popf{l|d}\n\t"
	    : "=&r" (eax), "=&r" (ebx)
	    : "i" (0x00200000));
	
	if (!((eax ^ ebx) & 0x00200000))
		return 0;

	__cpuid(ext, eax, ebx, ecx, edx);
	
	if (sig)
		*sig = ebx;

    	return eax;
}

int
cpuid (uint32_t leaf, uint32_t *eax, uint32_t *ebx,
	   	uint32_t *ecx, uint32_t *edx)
{
  	uint32_t ext = leaf & 0x80000000;
      	uint32_t maxlevel = __get_cpuid_max(ext, 0);
	
	if (maxlevel == 0 || maxlevel < leaf)
		return 0;
	
	__cpuid(leaf, *eax, *ebx, *ecx, *edx);
      	return 1;
}

int
cpuid_count (uint32_t leaf, uint32_t subleaf,
		   uint32_t *eax, uint32_t *ebx,
		   uint32_t *ecx, uint32_t *edx)
{
      	uint32_t ext = leaf & 0x80000000;
      	uint32_t maxlevel = __get_cpuid_max (ext, 0);
      
	if (maxlevel == 0 || maxlevel < leaf)
	    	return 0;
      
	__cpuid_count(leaf, subleaf, *eax, *ebx, *ecx, *edx);
      
	return 1;
}

uint32_t cpuid_eax(unsigned int leaf)
{
	uint32_t eax, ebx, ecx, edx;

	cpuid(leaf, &eax, &ebx, &ecx, &edx);

	return eax;
}

uint32_t cpuid_ebx(unsigned int leaf)
{
	uint32_t eax, ebx, ecx, edx;

	cpuid(leaf, &eax, &ebx, &ecx, &edx);

	return edx;
}

uint32_t cpuid_ecx(unsigned int leaf)
{
	uint32_t eax, ebx, ecx, edx;

	cpuid(leaf, &eax, &ebx, &ecx, &edx);

	return ecx;
}

uint32_t cpuid_edx(unsigned int leaf)
{
	uint32_t eax, ebx, ecx, edx;

	cpuid(leaf, &eax, &ebx, &ecx, &edx);

	return edx;
}


void __u32_to_str(uint32_t word, char *buffer)
{
	buffer[0] = word & 0xFF;
	buffer[1] = (word >> 0x08) & 0xFF;
	buffer[2] = (word >> 0x10) & 0xFF;
	buffer[3] = (word >> 0x18) & 0xFF;
}

void __regs_to_str(uint32_t eax, uint32_t ebx, uint32_t ecx,
		   uint32_t edx, char *buffer)
{
	__u32_to_str(eax, &buffer[0]);
	__u32_to_str(ebx, &buffer[4]);
	__u32_to_str(ecx, &buffer[8]);
	__u32_to_str(edx, &buffer[12]);
}

unsigned int x86_family(unsigned int sig)
{
	unsigned int x86;
	
	x86 = (sig >> 8) & 0xf;
	if (x86 == 0xf)
		x86 += (sig >> 20) & 0xff;
	return x86;
}

unsigned int x86_model(unsigned int sig)
{
	unsigned int fam, model;
	
	fam = x86_family(sig);
	model = (sig >> 4) & 0xf;
	
	if (fam >= 0x6)
		model += ((sig >> 16) & 0xf) << 4;
	return model;
}

unsigned int x86_stepping(unsigned int sig)
{
	return sig & 0xf;
}

void get_cpu_cap(struct cpuinfo_x86 *c)
{
	uint32_t eax, ebx, ecx, edx;
	
	if (c->cpuid_level >= CPUID_GETFEATURES) {
		cpuid(CPUID_GETFEATURES, &eax, &ebx, &ecx, &edx);
		c->x86_capability[CPUID_1_ECX] = ecx;
		c->x86_capability[CPUID_1_EDX] = edx;
	}

	if (c->cpuid_level >= CPUID_GETPOWER)
		c->x86_capability[CPUID_6_EAX] = cpuid_eax(CPUID_GETPOWER);
	
	if (c->cpuid_level >= CPUID_INTELADDITIONAL) {
		cpuid_count(CPUID_INTELADDITIONAL, 0, &eax, &ebx, &ecx, &edx);
		c->x86_capability[CPUID_7_0_EBX] = ebx;
		c->x86_capability[CPUID_7_ECX] = ecx;
		c->x86_capability[CPUID_7_EDX] = edx;
	}

	if (c->cpuid_level >= 0x0000000d) {
		cpuid_count(0x0000000d, 1, &eax, &ebx, &ecx, &edx);
		c->x86_capability[CPUID_D_1_EAX] = eax;
	}

	if (c->cpuid_level >= 0x0000000F) {
		cpuid_count(0x0000000F, 0, &eax, &ebx, &ecx, &edx);
		c->x86_capability[CPUID_F_0_EDX] = edx;
	}

	eax = cpuid_eax(CPUID_EXTENDED);
	c->extended_cpuid_level = eax;

	if ((eax & 0xffff0000) == CPUID_EXTENDED) {
		if (eax >= CPUID_FEATURES) {
			cpuid(CPUID_FEATURES, &eax, &ebx, &ecx, &edx);
			c->x86_capability[CPUID_8000_0001_ECX] = ecx;
			c->x86_capability[CPUID_8000_0001_EDX] = edx;
		}
	}

	if (c->extended_cpuid_level >= CPUID_ADVANCEDPOWER) {
		cpuid(CPUID_ADVANCEDPOWER, &eax, &ebx, &ecx, &edx);
		c->x86_capability[CPUID_8000_0007_EBX] = ebx;
		c->x86_power = edx;
	}

	if (c->extended_cpuid_level >= CPUID_VIRTPHYADDR) {
		cpuid(CPUID_VIRTPHYADDR, &eax, &ebx, &ecx, &edx);
		c->x86_virt_bits = (eax >> 8) & 0xff;
		c->x86_phys_bits = eax & 0xff;
		c->x86_capability[CPUID_8000_0008_EBX] = ebx;
	}

	if (c->extended_cpuid_level >= 0x8000000a)
		c->x86_capability[CPUID_8000_000A_EDX] = cpuid_edx(0x8000000a);

}

void cpu_detect_cache_sizes(struct cpuinfo_x86 *c)
{
	unsigned int n, dummy, ebx, ecx, edx, l2size;

	n = c->extended_cpuid_level;

	if (n >= 0x80000005) {
		cpuid(0x80000005, &dummy, &ebx, &ecx, &edx);
		c->x86_cache_size = (ecx>>24) + (edx>>24);

		/* For 64 bits processor. See cpuid.h. */
		//c->x86_tlbsize = 0;
	}

	if (n < 0x80000006)     /* Some chips just has a large L1. */
		return;

	cpuid(0x80000006, &dummy, &ebx, &ecx, &edx);
	l2size = ecx >> 16;

	/* For 64 bits processor. See cpuid.h. */
	//c->x86_tlbsize += ((ebx >> 16) & 0xfff) + (ebx & 0xfff);

	if (l2size == 0)
		return;         /* Again, no L2 cache is possible */

	c->x86_cache_size = l2size;
}

void cpu_detect(struct cpuinfo_x86 *c)
{
	cpuid(CPUID_GETVENDORSTRING,
		(unsigned int *)&c->cpuid_level,
		(unsigned int *)&c->x86_vendor_id[0],
		(unsigned int *)&c->x86_vendor_id[8],
		(unsigned int *)&c->x86_vendor_id[4]);
	
	c->x86 = 4;
	if (c->cpuid_level >= CPUID_GETFEATURES) {
		uint32_t junk, tfms, cap0, misc;
		
		cpuid(CPUID_GETFEATURES, &tfms, &misc, &junk, &cap0);
		c->x86          = x86_family(tfms);
		c->x86_model    = x86_model(tfms);
		c->x86_stepping = x86_stepping(tfms);
		
		if (cap0 & (1<<19)) {
			c->x86_clflush_size = ((misc >> 8) & 0xff) * 8;
			c->x86_cache_alignment = c->x86_clflush_size;
		}
	}
}

static void get_model_name(struct cpuinfo_x86 *c)
{
	char *p, *q, *s;
	uint32_t eax, ebx, ecx, edx;
	
	if (c->extended_cpuid_level < CPUID_BRANDSTRINGEND)
		return;
	cpuid(CPUID_BRANDSTRING, &eax, &ebx, &ecx, &edx);
	__regs_to_str(eax, ebx, ecx, edx, &c->x86_model_id[0]);
	cpuid(CPUID_BRANDSTRINGMORE, &eax, &ebx, &ecx, &edx);
	__regs_to_str(eax, ebx, ecx, edx, &c->x86_model_id[16]);
	cpuid(CPUID_BRANDSTRINGEND, &eax, &ebx, &ecx, &edx);
	__regs_to_str(eax, ebx, ecx, edx, &c->x86_model_id[32]);
	c->x86_model_id[48] = '\0';
	
	/* Trim whitespace */
	p = q = s = &c->x86_model_id[0];
	
	while (*p == ' ')
		p++;
	
	while (*p) {
		/* Note the last non-whitespace index */
		if (*p != ' ')
			s = q;
		*q++ = *p++;
	}
	
	*(s + 1) = '\0';
}

void cpuinfo(void)
{
	struct cpuinfo_x86 cpu;
	cpu_detect(&cpu);
	cpu_detect_cache_sizes(&cpu);
	get_cpu_cap(&cpu);
	get_model_name(&cpu);
	kprintf("Processor: %s\n", cpu.x86_model_id);
}
