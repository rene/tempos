/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: cpuid.h
 * Written by: Julio Faracco
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

#ifndef CPU_H

        #define CPU_H

        #include <unistd.h>

	#define NCAPINTS		19	/* N 32-bit words worth of info */
	#define NBUGINTS		1	/* N 32-bit bug flags */

	/* CPU Vendor-strings. */
	#define CPUID_VENDOR_OLDAMD       "AMDisbetter!" /* early engineering samples of AMD K5 processor */
	#define CPUID_VENDOR_AMD          "AuthenticAMD"
	#define CPUID_VENDOR_INTEL        "GenuineIntel"
	#define CPUID_VENDOR_CENTAUR      "CentaurHauls"
	#define CPUID_VENDOR_OLDTRANSMETA "TransmetaCPU"
	#define CPUID_VENDOR_TRANSMETA    "GenuineTMx86"
	#define CPUID_VENDOR_CYRIX        "CyrixInstead"
	#define CPUID_VENDOR_CENTAUR      "CentaurHauls"
	#define CPUID_VENDOR_NEXGEN       "NexGenDriven"
	#define CPUID_VENDOR_UMC          "UMC UMC UMC "
	#define CPUID_VENDOR_SIS          "SiS SiS SiS "
	#define CPUID_VENDOR_NSC          "Geode by NSC"
	#define CPUID_VENDOR_RISE         "RiseRiseRise"
	#define CPUID_VENDOR_VORTEX       "Vortex86 SoC"
	#define CPUID_VENDOR_VIA          "VIA VIA VIA "
 
	/* CPU Vendor-strings from Virtual Machines.*/
	#define CPUID_VENDOR_KVM	  "KVMKVMKVM"
	#define CPUID_VENDOR_VMWARE       "VMwareVMware"
	#define CPUID_VENDOR_XENHVM       "XenVMMXenVMM"
	#define CPUID_VENDOR_MICROSOFT_HV "Microsoft Hv"
	#define CPUID_VENDOR_PARALLELS    " lrpepyh vr"
	#define CPUID_VENDOR_BHYVE	  "bhyve bhyve "

	enum cpuid_leafs {
		CPUID_1_EDX=0,
		CPUID_8000_0001_EDX,
		CPUID_8086_0001_EDX,
		CPUID_LNX_1,
		CPUID_1_ECX,
		CPUID_C000_0001_EDX,
		CPUID_8000_0001_ECX,
		CPUID_LNX_2,
		CPUID_LNX_3,

		CPUID_7_0_EBX,
		CPUID_D_1_EAX,
		CPUID_F_0_EDX,
		CPUID_F_1_EDX,
		CPUID_8000_0008_EBX,

		CPUID_6_EAX,
		CPUID_8000_000A_EDX,
		CPUID_7_ECX,
		CPUID_8000_0007_EBX,
		CPUID_7_EDX,
	};

	enum cpuid_requests {
		CPUID_GETVENDORSTRING=0,
		CPUID_GETFEATURES,
		CPUID_GETTLB,
		CPUID_GETSERIAL,
		CPUID_ADDITIONALCACHE,
		CPUID_MWAIT,
		CPUID_GETPOWER,
		CPUID_INTELADDITIONAL,

		CPUID_EXTENDED=0x80000000,
		CPUID_FEATURES,
		CPUID_BRANDSTRING,
		CPUID_BRANDSTRINGMORE,
		CPUID_BRANDSTRINGEND,
		CPUID_L1TLB,
		CPUID_EXTENDEDL2,
		CPUID_ADVANCEDPOWER,
		CPUID_VIRTPHYADDR
	};

	struct cpuinfo_x86 {
		uchar8_t		x86;		/* CPU family */
		uchar8_t		x86_vendor;	/* CPU vendor */
		uchar8_t		x86_model;	/* CPU model */
		uchar8_t		x86_stepping;
		/* Number of 4K pages in DTLB/ITLB combined(in pages): */
		/* For 64 bits processors. */
		//int			x86_tlbsize;
		uchar8_t		x86_virt_bits;
		uchar8_t		x86_phys_bits;
		uchar8_t		x86_coreid_bits;
		uchar8_t		cu_id;
		uint32_t		extended_cpuid_level;
		int			cpuid_level;
		uint32_t		x86_capability[NCAPINTS + NBUGINTS];
		char                    x86_vendor_id[16];
		char                    x86_model_id[64];
		unsigned int            x86_cache_size;
		int			x86_cache_alignment;
		int			x86_power;
		uint16_t		x86_clflush_size;
	};

	int cpuid (uint32_t leaf, uint32_t *eax, uint32_t *ebx,
		   uint32_t *ecx, uint32_t *edx);

	int cpuid_count (uint32_t leaf, uint32_t subleaf,
 			 uint32_t *eax, uint32_t *ebx,
			 uint32_t *ecx, uint32_t *edx);

	void cpuinfo(void);
 
#endif /* CPU_H */
