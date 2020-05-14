#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
    // Params for executable ELF header on Linux x86_64
    Elf64_Addr entry_point = 0x400080;
    Elf64_Off phoff = 0x40;
    Elf64_Half ehsize = 0x40;
    Elf64_Half phsize = 56;
    Elf64_Half	phnum = 1;

    Elf64_Ehdr elf_h = 
    {
        {                       // e_ident
            ELFMAG0,
            ELFMAG1,
            ELFMAG2, 
            ELFMAG3, 
            ELFCLASS64, 
            ELFDATA2LSB, 
            EV_CURRENT, 
            ELFOSABI_NONE,
            EI_PAD
        },
        ET_EXEC,                // e_type
        EM_X86_64,              // e_machine
        EV_CURRENT,             // e_version
        entry_point,            // e_entry
        phoff,                  // e_phoff
        0,                      // e_shoff
        0,                      // e_flags
        ehsize,                 // e_ehsize
        phsize,                 // e_phentsize
        phnum,                  // e_phnum
        0,                      // e_shentsize
        0,                      // e_shum
        0                       // e_shstrndx
    };

    // Params for Programm header
    Elf64_Addr address = 0x400000;
    Elf64_Xword size   = 0xC7;          // different for every programm!
    Elf64_Xword align  = 0x200000;

    Elf64_Phdr prog_h = 
    {
        PT_LOAD,                // p_type
        PF_X | PF_R,            // p_flags
        0,  	                // p_offset
        address,	            // p_vaddr
        address,	            // p_paddr
        size,	                // p_filesz
        size,                   // p_memsz
        align	                // p_align
    };

    uint8_t program_body[] = 
    {   
        0x90, 0x90, 0x90, 0x48, 0x83, 0xec, 0x06, 0xc6, 0x04, 0x24, 0x48, 0xc6, 0x44, 0x24, 0x01, 0x65,
        0xc6, 0x44, 0x24, 0x02, 0x6c, 0xc6, 0x44, 0x24, 0x03, 0x6c, 0xc6, 0x44, 0x24, 0x04, 0x6f, 0xc6,
        0x44, 0x24, 0x05, 0x0a, 0xb8, 0x01, 0x00, 0x00, 0x00, 0xbf, 0x01, 0x00, 0x00, 0x00, 0x48, 0x89,
        0xe6, 0xba, 0x06, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xb8, 0x3c, 0x00, 0x00, 0x00, 0xbf, 0x00, 0x00,
        0x00, 0x00, 0x0f, 0x05, 0x90, 0x90, 0x90
    };

    size_t offset = 0x80 - sizeof(elf_h) - sizeof(prog_h);
    char placeholder[offset] = {0};
    FILE* file = fopen("my.elf", "wb");
    fwrite((char*)&elf_h, sizeof(char), sizeof(elf_h), file);
    fwrite((char*)&prog_h, sizeof(char), sizeof(prog_h), file);
    fwrite(placeholder, sizeof(char), offset, file);
    fwrite((char*)program_body, sizeof(char), sizeof(program_body), file);
    fclose(file);

    return 0;
}