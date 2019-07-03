#include "woody.h"

void	*create_woody(int src, size_t size)
{
	char		*file;
	int			dst;
	char		*woody;
	size_t		i;

	file = mmap(NULL, size, PROT_READ, MAP_PRIVATE, src, 0);
	dst = open("woody", O_RDWR|O_CREAT|O_TRUNC, 0744);
	lseek(dst, size - 1, SEEK_SET);
	write(dst, "", 1);
	woody = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, dst, 0);
	i = 0;
	while (i < size)
	{
		woody[i] = file[i];
		i++;
	}
	munmap(file, size);
	close(dst);
	return (woody);
}

void	*get_section(Elf64_Ehdr *hdr, char *section_name)
{
	Elf64_Shdr		*shdr;
	Elf64_Shdr		*strsec;
	char			*strtab;
	char			*head;
	size_t			i;

	head = (char*)hdr;
	shdr = (Elf64_Shdr*)(head + hdr->e_shoff);
	strsec = &shdr[hdr->e_shstrndx];
	strtab = head + strsec->sh_offset;
	i = 0;
	while (i < hdr->e_shnum)
	{
		if (ft_strequ((strtab + shdr[i].sh_name), section_name))
		{
			/* printf("format", ); */
			/* printf("offset ((%ld))\n", shdr[num_sections].sh_offset); */
			/* printf("((%ld))\n", ((Elf64_Shdr*)(head + shdr->sh_offset))->sh_offset); */
			return (shdr + i);
			/* return (head + shdr->sh_offset); */
		}
		shdr = (Elf64_Shdr*)(((char*)shdr) + shdr->sh_size);
		i++;
	}
	return (NULL);
}

/* typedef struct */
/* { */
/*   Elf32_Word	p_type;			/\* Segment type *\/ */
/*   Elf32_Off	p_offset;		/\* Segment file offset *\/ */
/*   Elf32_Addr	p_vaddr;		/\* Segment virtual address *\/ */
/*   Elf32_Addr	p_paddr;		/\* Segment physical address *\/ */
/*   Elf32_Word	p_filesz;		/\* Segment size in file *\/ */
/*   Elf32_Word	p_memsz;		/\* Segment size in memory *\/ */
/*   Elf32_Word	p_flags;		/\* Segment flags *\/ */
/*   Elf32_Word	p_align;		/\* Segment alignment *\/ */
/* } Elf32_Phdr; */
/* typedef struct */
/* { */
/*   unsigned char	e_ident[EI_NIDENT];	/\* Magic number and other info *\/ */
/*   Elf64_Half	e_type;			/\* Object file type *\/ */
/*   Elf64_Half	e_machine;		/\* Architecture *\/ */
/*   Elf64_Word	e_version;		/\* Object file version *\/ */
/*   Elf64_Addr	e_entry;		/\* Entry point virtual address *\/ */
/*   Elf64_Off	e_phoff;		/\* Program header table file offset *\/ */
/*   Elf64_Off	e_shoff;		/\* Section header table file offset *\/ */
/*   Elf64_Word	e_flags;		/\* Processor-specific flags *\/ */
/*   Elf64_Half	e_ehsize;		/\* ELF header size in bytes *\/ */
/*   Elf64_Half	e_phentsize;		/\* Program header table entry size *\/ */
/*   Elf64_Half	e_phnum;		/\* Program header table entry count *\/ */
/*   Elf64_Half	e_shentsize;		/\* Section header table entry size *\/ */
/*   Elf64_Half	e_shnum;		/\* Section header table entry count *\/ */
/*   Elf64_Half	e_shstrndx;		/\* Section header string table index *\/ */
/* } Elf64_Ehdr; */
void	make_exe(Elf64_Ehdr *hdr)
{
	Elf64_Phdr		*phdr;
	size_t			i;

	phdr = (Elf64_Phdr*)(hdr->e_phoff);
	i = 0;
	while (i < hdr->e_phnum)
	{
		phdr->p_flags = PF_X;
		/* phdr = (Elf64_Phdr*)(((char*)phdr) + phdr->p_memsz); */
		i++;
	}
}

/* int		ft_atoi_hex(char *s) */
/* { */
/* 	int hex; */

/* 	hex = 0; */
/* 	if (ft_isdigit(*s)) */
/* 		hex += (*s - '0') * 16; */
/* 	else */
/* 		hex += (*s - 'a' + 10) * 16; */
/* 	s++; */
/* 	if (ft_isdigit(*s)) */
/* 		hex += *s - '0'; */
/* 	else */
/* 		hex += *s - 'a' + 10; */
/* 	return (hex); */
/* } */

void	haxor(uint8_t *eh_frame, uint64_t jmp_to)
{
	int			fd;
	void		*file;
	Elf64_Shdr	*text;
	struct stat	st;
	uint64_t	i;
	uint8_t		*dump;

	fd = open("asm.o", O_RDONLY);
	fstat(fd, &st);
	file = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	text = get_section(file, ".text");
	dump = file + text->sh_offset;
	(void)jmp_to;
	(void)dump;
	i = 0;
	/* size_t noplen = 20; */
	/* while (i < noplen) */
	/* { */
	/* 	eh_frame[i] = 0x90; */
	/* 	i++; */
	/* } */
	while (i < text->sh_size)
	{
		eh_frame[i] = dump[i];
		i++;
	}

	/* char *code = "48 31 c0 50 48 b8 4f 44 59 2e 2e 2e 2e 2e 50 48 b8 2e 2e 2e 2e 57 4f 00 00 50 48 31 c0 ba 0e 00 00 00 48 89 e6 bf 01 00 00 00 b0 01 0f 05"; */

	/* i = 0; */
	/* while (*code) */
	/* { */
	/* 	eh_frame[i] = ft_atoi_hex(code); */
	/* 	i++; */
	/* 	code += 3; */
	/* } */
}

int main(int argc, char *argv[])
{
	int			src;
	struct stat	st;
	Elf64_Ehdr	*woody;
	Elf64_Shdr	*eh_frame;
	uint64_t	start;

	if (argc != 2)
		return (printf("Usage: ./pecker <64-bit binary>"));
	src = open(argv[1], O_RDONLY);
	fstat(src, &st);
	woody = create_woody(src, st.st_size);
	eh_frame = get_section(woody, ".eh_frame");
	start = woody->e_entry;
	/* 2048 */
	printf("start (%lx) sh_offset (%lx)\n", start, eh_frame->sh_offset);
	woody->e_entry = eh_frame->sh_offset;
	haxor(((uint8_t*)woody) + eh_frame->sh_offset, start);
	make_exe(woody);
	munmap(woody, st.st_size);
	close(src);
	(void)start;
	return (0);
}
