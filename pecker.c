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
	char			*strtab;
	char			*head;
	size_t			i;

	head = (char*)hdr;
	shdr = (Elf64_Shdr*)(head + hdr->e_shoff);
	strtab = head + shdr[hdr->e_shstrndx].sh_offset;
	i = 0;
	while (i < hdr->e_shnum)
	{
		if (ft_strequ(strtab + shdr[i].sh_name, section_name))
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
	// eh_frame
	/* phdr[4].p_flags = PF_R | PF_W | PF_X; */
	while (i < hdr->e_phnum)
	{
		phdr->p_flags = PF_R | PF_W | PF_X;
		phdr = (Elf64_Phdr*)(((char*)phdr) + sizeof(*phdr));
		i++;
	}
}

uint8_t	*write_uint32(uint8_t *file, uint32_t n)
{
	*file++ = (n >>  0) & 0x00000000000000ff;
	*file++ = (n >>  8) & 0x00000000000000ff;
	*file++ = (n >> 16) & 0x00000000000000ff;
	*file++ = (n >> 24) & 0x00000000000000ff;
	return (file);
}

uint8_t	*write_mov(uint8_t *file, uint8_t reg, uint32_t n)
{
	*file++ = 0x41;
	*file++ = reg;
	return (write_uint32(file, n));
}

void	haxor(Elf64_Ehdr *woody, uint8_t *eh_frame, uint32_t jmp_to)
{
	int			fd;
	void		*asm_file;
	Elf64_Shdr	*text;
	struct stat	st;
	uint64_t	i;
	uint8_t		*dump;
	uint32_t	rel_text;
	Elf64_Shdr	*woody_eh_frame;
	Elf64_Shdr	*woody_text;

	fd = open("asm.o", O_RDONLY);
	fstat(fd, &st);
	asm_file = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	text = get_section(asm_file, ".text");
	dump = asm_file + text->sh_offset;
	woody_eh_frame = get_section(woody, ".eh_frame");
	woody_text = get_section(woody, ".text");
	// mov r9,size
	eh_frame = write_mov(eh_frame, R9, woody_text->sh_size);
	// mov r8,rel_text
	rel_text = woody_eh_frame->sh_offset - woody_text->sh_offset;
	eh_frame = write_mov(eh_frame, R8, rel_text);
	i = 0;
	while (i < text->sh_size)
		*eh_frame++ = dump[i++];
	// relative jmp to _start
	*eh_frame++ = 0xe9;
	jmp_to -= (text->sh_size + 17); // 17 == sizeof(2 movs and a jmp)
	eh_frame = write_uint32(eh_frame, jmp_to);
	munmap(asm_file, st.st_size);
	close(fd);
}

void	encrypt_text(Elf64_Ehdr *woody)
{
	Elf64_Shdr	*texthdr;
	uint8_t		*text;
	size_t		i;

	texthdr = get_section(woody, ".text");
	text = (uint8_t*)woody + texthdr->sh_offset;
	i = 0;
	while (i < texthdr->sh_size)
	{
		text[i] ^= 0x13;
		i++;
	}
}

int main(int argc, char *argv[])
{
	int			src;
	struct stat	st;
	Elf64_Ehdr	*woody;
	Elf64_Shdr	*eh_frame;
	size_t		start;

	if (argc != 2)
		return (printf("Usage: ./pecker <64-bit binary>"));
	src = open(argv[1], O_RDONLY);
	fstat(src, &st);
	woody = create_woody(src, st.st_size);
	make_exe(woody);
	eh_frame = get_section(woody, ".eh_frame");
	start = woody->e_entry;
	woody->e_entry = eh_frame->sh_offset;
	haxor(woody, ((uint8_t*)woody) + eh_frame->sh_offset, start - eh_frame->sh_offset);
	encrypt_text(woody);
	munmap(woody, st.st_size);
	close(src);
	return (0);
}
