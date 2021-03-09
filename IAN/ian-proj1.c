#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <libelf.h>
#include <gelf.h>
#include <elf.h>
#include <string.h>
#include <stdlib.h>

int getSegType(size_t type, char* string){
	if(type == 0) strcpy(string,"NULL");
	else if(type == 1) strcpy(string,"LOAD");
	else if(type == 2) strcpy(string,"DYNAMIC");
	else if(type == 3) strcpy(string,"INTERP");
	else if(type == 4) strcpy(string,"NOTE");
	else if(type == 5) strcpy(string,"SHLIB");
	else if(type == 6) strcpy(string,"PHDR");
	else if(type == 7) strcpy(string,"TLS");
	else if(type == 1685382483){
		strcpy(string,"GNU_PROPERTY");
		return 2;
	}
	else if(type == 1685382480){
		strcpy(string,"GNU_EH_FRAME");
		return 2;
	}
	else if(type == 1685382481){
		strcpy(string,"GNU_STACK");
		return 2;
	}
	else if(type == 1685382482){
		strcpy(string,"GNU_RELRO");
		return 2;
	}
	//IGNORE
	else{
		sprintf(string,"%d",type);
		return 0;
	}
	return 0;
}

void getSegPer(size_t type, char* string){
	
	memset(string,0,strlen(string));
	
	if(type & PF_R) strcpy(&(string[0]),"R");
	else strcpy(&(string[0]),"-");
	
	if(type & PF_W) strcpy(&(string[1]),"W");
	else strcpy(&(string[1]),"-");
	
	if(type & PF_X) strcpy(&string[2],"X");
	else strcpy(&string[2],"-");
	
	return;
}

int main(int argc, char **argv){

    if(argc != 2)
    {
        fprintf(stderr,"Program needs 1 argument\n");
        return 1;
    }
	
	int file, phdrNum;
	Elf *elf;
	GElf_Phdr *phdr, dst;
	GElf_Shdr shdr;
	Elf_Scn *scn = NULL;
	size_t numb, n;
	long long int mem_start, mem_end;
	char type[20], perm[10], sections[400];
	
	file = open(argv[1], O_RDONLY);
	if(file == -1)
	{
		fprintf(stderr,"Program cant open %s", argv[1]);
		return 2;
	}
	
	//Check lib version
	if (elf_version(EV_CURRENT) == EV_NONE) return 4;
	
	elf = elf_begin(file, ELF_C_READ, (Elf *) 0);
	if(elf == 0){
		printf("%s\n",elf_errmsg(elf_errno()));
		return 2;
	}
	
	//Get count of segments
	if(elf_getphdrnum(elf, &numb) != 0) return 5;
	
	if(elf_getshdrstrndx(elf, &n)) return 6;
	
	//HEADER
	printf("Segment Type		Perm	 Sections\n");
	
	for(unsigned int i = 0; i < numb; ++i)
	{
		memset(sections, 0, strlen(sections));
		mem_start = 0;
		mem_end = 0;
		
		// Get segments
		phdr = gelf_getphdr(elf, i, &dst);
		
		if(phdr->p_vaddr){
			mem_end = (phdr->p_vaddr + phdr->p_memsz);
			mem_start = phdr->p_vaddr;
		}
		
		//Sections
		while((scn = elf_nextscn(elf, scn)) != NULL){
			gelf_getshdr(scn, &shdr);
			if(shdr.sh_addr >= mem_start && shdr.sh_addr < mem_end)
			{
				strcat(sections," ");
				strcat(sections,elf_strptr(elf, n, shdr.sh_name));
			}
		}
		
		getSegPer(phdr->p_flags, perm);
		int t = getSegType(phdr->p_type, type);
		
		if(t == 0) printf("%2d	%s		%s	%s\n", i, type, perm, sections);
		else if(t == 2) printf("%2d	%s	%s	%s\n", i, type, perm, sections);
	}
	elf_end(elf);
	close(file);
    return 0;
}
