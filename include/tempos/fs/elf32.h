/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: elf32.h
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

#ifndef ELF32_H

	#define ELF32_H

	#include <unistd.h>


	/* File Identification */
	#define EI_MAG0		0
	#define EI_MAG1		1
	#define EI_MAG2		2
	#define EI_MAG3		3
	#define EI_CLASS	4
	#define EI_DATA		5
	#define EI_VERSION	6
	#define EI_PAD		7
	#define EI_NIDENT	16

	#define ELFMAG0		0x7f
	#define ELFMAG1		'E'
	#define ELFMAG2		'L'
	#define ELFMAG3		'F'

	#define ELFCLASSNONE	0
	#define ELFCLASS32	 	1
	#define ELFCLASS64	 	2

	#define ELFDATANONE	0
	#define ELFDATA2LSB	1
	#define ELFDATA2MSB	2

	/* File types */
	#define ET_NONE		0
	#define ET_REL		1
	#define ET_EXEC		2
	#define ET_DYN		3
	#define ET_CORE		4
	#define ET_LOPROC	0xff00
	#define ET_HIPROC	0xffff

	/* Machine types */
	#define EM_NONE			0
	#define EM_M32			1
	#define EM_SPARC		2
	#define EM_386			3
	#define EM_68K			4
	#define EM_88K			5
	#define EM_860			7
	#define EM_MIPS			8
	#define EM_MIPS_RS4_BE	10

	/* File versions */
	#define EV_NONE		0
	#define EV_CURRENT	1

	/* Segment types */
	#define PT_NULL		0
	#define PT_LOAD		1
	#define PT_DYNAMIC	2
	#define PT_INTERP	3
	#define PT_NOTE		4
	#define PT_SHLIB	5
	#define PT_PHDR		6
	#define PT_LOPROC	0x70000000
	#define PT_HIPROC	0x7fffffff

	/* Segment permissions */
	#define PF_X		0x1
	#define PF_W		0x2
	#define PF_R		0x4
	#define PF_MASKPROC 0xf0000000


	/* 32-Bit data types */
	typedef uint32_t Elf32_Addr;
	typedef uint16_t Elf32_Half;
	typedef uint32_t Elf32_Off;
	typedef int32_t  Elf32_Sword;
	typedef uint32_t Elf32_Word;


	/* ELF Header */
	struct _Elf32_Ehdr {
		uchar8_t	e_ident[EI_NIDENT];
		Elf32_Half	e_type;
		Elf32_Half	e_machine;
		Elf32_Word	e_version;
		Elf32_Addr	e_entry;
		Elf32_Off	e_phoff;
		Elf32_Off	e_shoff;
		Elf32_Word	e_flags;
		Elf32_Half	e_ehsize;
		Elf32_Half	e_phentsize;
		Elf32_Half	e_phnum;
		Elf32_Half	e_shentsize;
		Elf32_Half	e_shnum;
		Elf32_Half	e_shstrndx;
	};

	/* Program Header */
	struct _Elf32_Phdr {
		Elf32_Word	p_type;
		Elf32_Off	p_offset;
		Elf32_Addr	p_vaddr;
		Elf32_Addr	p_paddr;
		Elf32_Word	p_filesz;
		Elf32_Word	p_memsz;
		Elf32_Word	p_flags;
		Elf32_Word	p_align;
	};


	typedef struct _Elf32_Ehdr Elf32_Ehdr;
	typedef struct _Elf32_Phdr Elf32_Phdr;


#endif /* ELF32_H */

