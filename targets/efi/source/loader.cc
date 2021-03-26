#include "loader.hh"
#include "log.hh"
#include "console.hh"

#include "efi/protocol/simple-filesystem.hh"

#include "barelib/elf.hh"
#include "barelib/farbfeld.hh"

#include "public/HypervisorStartConfig.hh"

extern "C" typedef __attribute__((sysv_abi)) uint64_t (*start_func_t)(kyu::pub::HypervisorStartConfig*);
#include "pub.hh"

namespace kyu::loader
{
    efi::protocol::file_protocol_t* hypervisor_root;
    efi::protocol::file_protocol_t* hypervisor_file;

    start_func_t hypervisor_start;

    int RunHypervisor()
    {
        uint64_t hypervisor_mem_size = EFI_SIZE_TO_PAGES(1024 * 1024 * 100);
        efi::physical_address_t hypervisor_mem;
        efi::status_t status = efi::system_table->boot_services->allocate_pages(efi::allocate_any_pages, EfiRuntimeServicesData, hypervisor_mem_size, &hypervisor_mem);
        if (status != 0)
        {
            console::PrintString("Failed to allocate memory for hypervisor!\n");
            return status;
        }

        pub::HypervisorStartConfig config;
        config.LoaderVersion = 0;
        config.MemeResult = 1337;
        config.PreparedMemoryRegion = (void*) hypervisor_mem;
        config.PreparedMemorySize = hypervisor_mem_size;
        
        config.LoaderFunctions.PrintString = LoaderPrintString;
        config.LoaderFunctions.PrintHex = LoaderPrintHex;
        config.LoaderFunctions.CPUCount = LoaderCPUCount;
        config.LoaderFunctions.CPUCurrent = LoaderCPUCurrent;
        config.LoaderFunctions.CPUExecuteOnAllCores = LoaderCPUExecuteOnAllCores;

        return hypervisor_start(&config);
    }

    void LocateHypervisor()
    {
        efi::status_t status = 0;

        auto con = efi::system_table->console_out;
        auto bs = efi::system_table->boot_services;

        efi::guid_t sfsp_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
        efi::handle_t* sfsp_handles;
        uint64_t sfsp_handlecount;

        // Aquire all file system protocol handles
        status = bs->locate_handle_buffer(efi::by_protocol, &sfsp_guid, nullptr, &sfsp_handlecount, &sfsp_handles);

        if (status != 0)
        {
            con->output_string(con, L"Failed to get fs handle\r\n");
            return;
        }

        // Check each protocol until we have found the hypervisor
        for (int i = 0; i < sfsp_handlecount; i++)
        {
            // Get the protocol from the handle
            efi::protocol::simple_filesystem_protocol_t* fs;
            status = bs->handle_protocol(sfsp_handles[i], &sfsp_guid, (void**) &fs);

            if (status != 0)
            {
                con->output_string(con, L"Failed to open fs protocol\r\n");
                continue;
            }

            // Open the root folder
            efi::protocol::file_protocol_t* root;
            status = fs->open_volume(fs, &root);

            if (status != 0)
            {
                con->output_string(con, L"Failed to open fs root\r\n");
                continue;
            }

            hypervisor_root = root;

            // Open the hypervisor file
            efi::protocol::file_protocol_t *hv_file;
            // TODO: REPLACE THIS FILE LOCATION WITH SOMETHING MORE APROPRIATE
            status = root->open(root, &hv_file, L"\\visor\\kyuvisor.elf", EFI_FILE_MODE_READ, 0);

            // TODO: Actually check if it's a file not found error
            if (status != 0)
            {
                continue;
            }

            hypervisor_file = hv_file;
            break;
        }

        if (hypervisor_file == nullptr)
        {
            con->output_string(con, L"Failed to find hypervisor!\r\n");
            // TODO: PANIC
            return;
        }
    }

    void MapHypervisor()
    {
        using namespace barelib;

        auto bs = efi::system_table->boot_services;
        efi::status_t status = 0;
        uint64_t buffer_size = 0;
        
        console::PrintString("Mapping hypervisor...\n");

        efi::protocol::file_info_t file_info;
        efi::guid_t file_info_guid = EFI_FILE_INFO_ID;
        buffer_size = sizeof(file_info);
        status = hypervisor_file->get_info(hypervisor_file, &file_info_guid, &buffer_size, &file_info);
        if (status != 0)
        {
            console::PrintString("Couldn't get file info!\n");
            return;
        }

        efi::physical_address_t elf_base;
        status = bs->allocate_pages(efi::allocate_any_pages, EfiBootServicesData, EFI_SIZE_TO_PAGES(file_info.file_size), &elf_base);
        if (status != 0)
        {
            console::PrintString("Failed to allocate memory for ELF file!\n");
            return;
        }

        buffer_size = file_info.file_size;
        status = hypervisor_file->read(hypervisor_file, &buffer_size, (void*) elf_base);
        if (status != 0)
        {
            console::PrintString("Failed to read ELF file!\n");
        }

        auto ehdr = reinterpret_cast<elf::elf64_ehdr*>(elf_base);
        auto phdrs = reinterpret_cast<elf::elf64_phdr*>(elf_base + ehdr->e_phoff);

        uint64_t sections_combined_size = 0;
        for (int i = 0; i < ehdr->e_phnum; i++)
        {
            auto last_offset = phdrs[i].p_vaddr + phdrs[i].p_memsz;
            if (last_offset > sections_combined_size)
            {
                sections_combined_size = last_offset;
            }
        }

        console::PrintString("ELF size in memory: ");
        console::PrintHex(sections_combined_size);
        console::PrintString("\n");

        efi::physical_address_t target_base;
        status = bs->allocate_pages(efi::allocate_any_pages, EfiRuntimeServicesCode, EFI_SIZE_TO_PAGES(sections_combined_size), &target_base);
        if (status != 0)
        {
            console::PrintString("Failed to allocate memory for hypervisor!\n");
            return;
        }

        console::PrintString("Mapping ELF to: ");
        console::PrintHex(target_base);
        console::PrintString("\n");

        for (int i = 0; i < ehdr->e_phnum; i++)
        {
            if (phdrs[i].p_type != PT_LOAD) continue;

            console::PrintString("Found loadable section!\n");

            auto target_offset = target_base + phdrs[i].p_vaddr;
            buffer_size = phdrs[i].p_filesz;
            bs->copy_mem((void*) target_offset, (void*) (elf_base + phdrs[i].p_offset), phdrs[i].p_memsz);
        }

        efi::physical_address_t entrypoint_addr = target_base + ehdr->e_entry;
        hypervisor_start = (start_func_t) entrypoint_addr;
    }
    
    graphics::image_t* LoadFFImageFromFS(wchar_t* image_path)
    {
        efi::status_t status = 0;
        graphics::image_t* result;
        
        status = efi::system_table->boot_services->allocate_pages(
            efi::allocate_any_pages,
            EfiBootServicesData,
            EFI_SIZE_TO_PAGES(sizeof(graphics::image_t)),
            (efi::physical_address_t*) &result
        );

        efi::protocol::file_protocol_t* image_file;
        status = hypervisor_root->open(hypervisor_root, &image_file, image_path, EFI_FILE_MODE_READ, 0);

        if (status != 0)
        {
            log::String(L"Failed to load image: ");
            log::String(image_path);
            log::String(L"\r\n");
            return nullptr;
        }

        barelib::farbfeld::header_t farb_header;
        uint64_t farb_header_size = sizeof(farb_header);
        status = image_file->read(image_file, &farb_header_size, &farb_header);

        if (status != 0)
        {
            log::String(L"no read\r\n");
            return nullptr;
        }

        result->width = (farb_header.width[3]) | (farb_header.width[2] << 8) | (farb_header.width[1] << 16) | (farb_header.width[0] << 24);
        result->height = (farb_header.height[3]) | (farb_header.height[2] << 8) | (farb_header.height[1] << 16) | (farb_header.height[0] << 24);

        status = efi::system_table->boot_services->allocate_pages(
            efi::allocate_any_pages,
            EfiRuntimeServicesData,
            EFI_SIZE_TO_PAGES(result->width * result->height * 4),
            (efi::physical_address_t*) &(result->data)
        );

        if (status != 0)
        {
            log::String(L"no ram\r\n");
            log::HexNumber(status);
            return nullptr;
        }

        uint8_t* buffer;
        uint64_t buffer_size = result->width * result->height * 8;
        status = efi::system_table->boot_services->allocate_pages(efi::allocate_any_pages, EfiRuntimeServicesData, EFI_SIZE_TO_PAGES(buffer_size), (efi::physical_address_t*) &buffer);
        if (status != 0)
        {
            log::String(L"no ram 2\r\n");
            return nullptr;
        }

        status = image_file->read(image_file, &buffer_size, buffer);
        if (status != 0)
        {
            log::String(L"no copy\r\n");
            return nullptr;
        }

        for (int i = 0; i < result->height; i++)
        {
            for (int j = 0; j < result->width; j++)
            {
                if (status != 0)
                {
                    log::String(L"ERERERERRR\r\n");
                    return nullptr;
                }

                auto index = i * result->width + j;
                result->data[index].red = (buffer[index * 8 + 0]);
                result->data[index].green = (buffer[index * 8 + 2]);
                result->data[index].blue = (buffer[index * 8 + 4]);
                result->data[index].reserved = 0;
            }
        }

        return result;
    }
}