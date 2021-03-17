#pragma once

#include <stdint.h>

#define EI_NIDENT 16

#define	PT_NULL		0		/* Program header table entry unused */
#define PT_LOAD		1		/* Loadable program segment */
#define PT_DYNAMIC	2		/* Dynamic linking information */
#define PT_INTERP	3		/* Program interpreter */
#define PT_NOTE		4		/* Auxiliary information */
#define PT_SHLIB	5		/* Reserved */
#define PT_PHDR		6		/* Entry for header table itself */
#define PT_TLS		7		/* Thread-local storage segment */
#define	PT_NUM		8		/* Number of defined types */
#define PT_LOOS		0x60000000	/* Start of OS-specific */
#define PT_GNU_EH_FRAME	0x6474e550	/* GCC .eh_frame_hdr segment */
#define PT_GNU_STACK	0x6474e551	/* Indicates stack executability */
#define PT_GNU_RELRO	0x6474e552	/* Read-only after relocation */
#define PT_LOSUNW	0x6ffffffa
#define PT_SUNWBSS	0x6ffffffa	/* Sun Specific segment */
#define PT_SUNWSTACK	0x6ffffffb	/* Stack segment */
#define PT_HISUNW	0x6fffffff
#define PT_HIOS		0x6fffffff	/* End of OS-specific */
#define PT_LOPROC	0x70000000	/* Start of processor-specific */
#define PT_HIPROC	0x7fffffff	/* End of processor-specific */

namespace barelib::elf
{
    typedef uint16_t elf_half_t;
    typedef uint32_t elf_word_t;
    typedef int32_t elf_sword_t;
    typedef uint64_t elf_xword_t;
    typedef int64_t elf_sxword_t;

    typedef uint64_t elf64_address_t;
    typedef uint64_t elf64_offset_t;

    typedef struct _elf64_ehdr
    {
        unsigned char e_ident[EI_NIDENT];
        elf_half_t e_type;
        elf_half_t e_machine;
        elf_word_t e_version;
        elf64_address_t e_entry;
        elf64_offset_t e_phoff;
        elf64_offset_t e_shoff;
        elf_word_t e_flags;
        elf_half_t e_ehsize;
        elf_half_t e_phentsize;
        elf_half_t e_phnum;
        elf_half_t e_shentsize;
        elf_half_t e_shnum;
        elf_half_t e_shstrndx;
    } elf64_ehdr;

    typedef struct _elf64_phdr
    {
        elf_word_t p_type;
        elf_word_t p_flags;
        elf64_offset_t p_offset;
        elf64_address_t p_vaddr;
        elf64_address_t p_paddr;
        elf_xword_t p_filesz;
        elf_xword_t p_memsz;
        elf_xword_t p_align;
    } elf64_phdr;
}