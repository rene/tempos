/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: makedeps.c
 * Desc: Part of TempOS build system.
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define REGEX_1 "^ {0,}obj-[yn] {0,}[+:]= {0,}(\\w+.o) {0,}"
#define REGEX_2 "^ {0,}obj-\\$\\(\\w+\\) {0,}[+:]= {0,}(\\w+.o) {0,}"
#define REGEX_C REGEX_1"|"REGEX_2


char *dirname(char *str);
char *new_str(size_t len);
char *get_deps(char *dest, char *path, char *rules);
int check_file(const char *fname);


int main(int argc, char **argv)
{
	FILE *fp, *mk, *rp;
	char *line  = NULL;
	char *line2 = NULL;
	size_t len  = 0;
	size_t len2 = 0;
	ssize_t read, read2;
	regex_t re;
	char *files;
	char *dname, *lexp, *tok;
	char *path, *deps;
	char *rules, *tmp, *cptr;
	char lbrk, *lbrk_pos;
	long fsize, pos;

	if(argc != 3) {
		fprintf(stderr, "Usage: %s <file_rules> <file_list>\n"   \
				"    <file_rules> - File with the build rules\n" \
				"    <file_list>  - List of build files (.mk)\n", argv[0]);
		return(EXIT_FAILURE);
	} else {
		if( (fp = fopen(argv[2], "r")) == NULL ) {
			fprintf(stderr, "Error on open %s. Check file permissions.\n", argv[2]);
			return(EXIT_FAILURE);
		}
		if( (rp = fopen(argv[1], "r")) == NULL ) {
			fclose(fp);
			fprintf(stderr, "Error on open %s. Check file permissions.\n", argv[1]);
			return(EXIT_FAILURE);
		} else {
			fseek(rp, 0, SEEK_END);
			fsize = ftell(rp);
			fseek(rp, 0, SEEK_SET);

			if((rules = new_str(fsize)) == NULL) {
				fclose(rp);
				fclose(fp);
				fprintf(stderr, "There is not enough free memory avaliable.\n");
				return(EXIT_FAILURE);
			}

			fread(rules, sizeof(char), fsize-1, rp);

			fclose(rp);
		}
	}

	/* Makefile mandatory rule */
	printf(".PHONY: all end clean\n\nall: end\n\n");

	/* Compile RE to parse configuration file */
	if(regcomp(&re, REGEX_C, REG_EXTENDED | REG_NOSUB | REG_ICASE) != 0) {
		fprintf(stderr, "Error with regular exressions.\n");
		fclose(fp);
		return(EXIT_FAILURE);
	}

	/* Parse file list */
	while( (read = getline(&line, &len, fp)) != - 1 ) {

		/* Parse file */
		if(strstr(line, "\n") != NULL && read >= 1) {
			line[read-1] = '\0';
			if((mk = fopen(line, "r")) == NULL) {
				fprintf(stderr, "Error on open %s.\n", line);
				continue;
			}
		}

		dname = dirname(line);
		line2 = NULL;
		read2 = 0;

		while( (read2 = getline(&line2, &len2, mk)) != -1 ) {

			/* Parse file */
			if(strstr(line2, "\n") != 0 && read2 >= 1) {
				line2[read2-1] = '\0';
				if(read2 == 1)
					continue;
			}

			/* TODO: Check for line break \ */


			if(regexec(&re, line2, 0, NULL, 0) == 0) {

				if((files = new_str(strlen(line2) + 1)) == NULL)
					continue;

				cptr = files;
				strcpy(files, line2);

				files    = strchr(files, '=');
				files[0] = '\0';
				files++;

				lexp = cptr;

				printf("%s= ", lexp);

				deps = NULL;
				tok  = strtok(files, " ");
				do {
					/* Print rule */
					if(strcmp(tok, "") != 0) {

						printf("%s/%s ", dname, tok);

						/* Check dependencies */
						if((path = new_str(strlen(dname) + strlen(tok) + 2)) == NULL)
							continue;

						sprintf(path, "%s/%s", dname, tok);
						deps = get_deps(deps, path, rules);
						if(path)
							free(path);
					}
				} while((tok = strtok(NULL, "  \n")) != NULL);

				printf("\n\n%s\n", deps);
			}
		}
		if(line2)
			free(line2);
		fclose(mk);
	}

	/* Makefile mandatory rule */
	printf("\nend: $(obj-y)\n\t@echo $(obj-y) >> objs.list\n");
	printf("\nclean:\n\t@for obj in $(obj-y); do \\\n");
	printf("\t[ -f $$obj ] && (rm -f $$obj && echo \" - Remove $$obj\") \\\n");
	printf("\t|| echo \" ! $$obj not found.\"; done\n");

	/* Clean-up */
	if(line)
		free(line);

	if(rules)
		free(rules);

	fclose(fp);
	regfree(&re);

	return(EXIT_SUCCESS);
}


char *dirname(char *str)
{
	size_t len, i;

	if(str == NULL)
		return(NULL);

	len = strlen(str);
	for(i=len-1; i>0 && str[i] != '/'; i--);

	if(i > 0) {
		str[i] = '\0';
		return(str);
	} else {
		return(NULL);
	}
}


char *new_str(size_t len)
{
	if(len <= 0)
		return(NULL);

	char *nstr = (char*)malloc(sizeof(char) * len);
	return(nstr);
}


int check_file(const char *fname)
{
	FILE *fp;

	if((fp = fopen(fname, "r")) == NULL) {
		return(0);
	} else {
		fclose(fp);
		return(1);
	}
}


char *get_deps(char *dest, char *path, char *rules)
{
	char tmpname[] = "/tmp/tempos.bs.XXXXXX";
	char *obj, *cmdline;
	char *deps = NULL;
	int fd;
	off_t fsize;
	ssize_t strsz, pos, nb;
	int status;

	if((obj = new_str(strlen(path) + 2)) == NULL)
		return(NULL);

	strcpy(obj, path);
	pos = strlen(obj) - 2;
	if(obj[pos] == '.' && obj[pos+1] == 'o') {
		obj[pos+1] = 'c';
	} else {
		return(NULL);
	}

	/* Check file */
	if( !check_file(obj) ) {
		obj[pos+1] = 'S';
		if( !check_file(obj) ) {
			free(obj);
			return(NULL);
		}
	}

	/* Call compiler */
	if((fd = mkostemp(tmpname, O_APPEND)) < 0) {
		return(NULL);
	}

	if( !fork() ) {
		/* Child proccess */
		char *fmt    = "$CC $CFLAGS -MM %s -MT %s";
		size_t fmtsz = strlen(fmt) + strlen(obj) + strlen(path) + 2;

		if((cmdline = new_str(fmtsz)) == NULL)
			exit(1);

		sprintf(cmdline, fmt, obj, path);

		/* Redirect I/O */
		close(STDERR_FILENO);
		dup(fd);
		close(STDOUT_FILENO);
		dup(fd);
		execlp("sh", "sh", "-c", cmdline, NULL);
		exit(1);
	} else {
		/* Father */
		wait(&status);
		sync();
	}

	fsize = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	if(fsize <= 0)
		return(NULL);

	strsz = fsize + strlen(rules) + 3;

	if(dest == NULL) {
		if((deps = (char*)malloc(sizeof(char) * strsz)) == NULL) {
			free(obj);
			close(fd);
			unlink(tmpname);
			return(NULL);
		}
		pos = 0;
	} else {
		pos    = strlen(dest);
		strsz += pos;

		if((deps = realloc(dest, sizeof(char) * strsz)) == NULL) {
			free(obj);
			close(fd);
			unlink(tmpname);
			return(NULL);
		}
	}

	if((nb = read(fd, &deps[pos], fsize)) > 0)
		deps[pos + nb] = '\0';

	strcat(deps, rules);
	strcat(deps, "\n\n");

	free(obj);

	close(fd);
	unlink(tmpname);
	return(deps);
}

