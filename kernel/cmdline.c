/*
 * Copyright (C) 2009-2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: cmdline.c
 * Desc: Parser for command line passed at boot
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
#include <string.h>

/** Maximum command line arguments */
#define CMDLINE_MAX_ARGS 50

/** command line arguments */
cmdline_arg_t cmdline_args[CMDLINE_MAX_ARGS];

/** copy of the whole command line string */
char cmdline_str[CMDLINE_MAX];

/** Number of valid arguments parsed */
int cmdline_argc = 0;

/**
 * Parse command line
 * \param cmdline Command line (string)
 * \return Number of arguments found.
 */
int parse_cmdline(char *cmdline)
{
	size_t i, len;
	int p;
	char *key, *value;

	strcpy(cmdline_str, cmdline);
	len = strlen(cmdline_str);

	p     = 0;
	i     = 0;
	key   = &cmdline_str[0];
	value = key;
	
	/* Remove initial spaces */
	while(cmdline_str[i] == ' ') {
		i++;
		key++;
	}

	while(cmdline_str[i++] != '\0') {
		/* check for key */
		if (cmdline_str[i] == '=') {
			cmdline_str[i] = '\0';
			cmdline_args[p].key = key;
			i++;
			value = &cmdline_str[i];
			cmdline_args[p].value = value;
		}

		if (cmdline_str[i] == ' ') {
			cmdline_str[i] = '\0';
			cmdline_args[p].value = value;
			p++;
			i++;
			key = &cmdline_str[i];
			while(cmdline_str[i] == ' ') {
				i++;
				key++;
			}
		}
	}

	cmdline_argc = p;
	return p;
}

/**
 * Return key value of command line argument
 * \param key to get its value
 * \return Key value
 */
char *cmdline_get_value(char *key)
{
	int i;

	for (i = 0; i <= cmdline_argc; i++) {
		if (strcmp(cmdline_args[i].key, key) == 0) {
			return cmdline_args[i].value;
		}
	}

	return NULL;
}

