#pragma once

#include <stdint.h>

#define EI_NIDENT 16

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