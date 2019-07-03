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
			return (shdr + i);
		shdr = (Elf64_Shdr*)(((char*)shdr) + shdr->sh_size);
		i++;
	}
	return (NULL);
}

void	make_exe(Elf64_Ehdr *hdr)
{
	Elf64_Phdr		*phdr;
	size_t			i;

	phdr = (Elf64_Phdr*)(((char*)hdr) + hdr->e_phoff);
	phdr->p_flags = PF_X;
	i = 0;
	while (i < hdr->e_phnum)
	{
		phdr->p_flags = PF_R | PF_W | PF_X;
		phdr = (Elf64_Phdr*)(((char*)phdr) + sizeof(*phdr));
		i++;
	}
}

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
	i = 0;
	while (i < text->sh_size)
	{
		eh_frame[i] = dump[i];
		i++;
	}
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
	make_exe(woody);
	eh_frame = get_section(woody, ".eh_frame");
	start = woody->e_entry;
	woody->e_entry = eh_frame->sh_offset;
	haxor(((uint8_t*)woody) + eh_frame->sh_offset, start);
	munmap(woody, st.st_size);
	close(src);
	(void)start;
	return (0);
}
