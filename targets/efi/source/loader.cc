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
    efi::physical_address_t hypervisor_elf;

    efi::protocol::file_protocol_t* hypervisor_root;

    int RunHypervisor()
    {
        using namespace barelib;
        auto con = efi::system_table->console_out;

        elf::elf64_ehdr* elf_header = (elf::elf64_ehdr*) hypervisor_elf;

        uint64_t entrypoint_vaddr = elf_header->e_entry;

        elf::elf64_phdr* current_phdr = (elf::elf64_phdr*) (hypervisor_elf + 64);
        bool has_found = false;
        for (int i = 0; i < elf_header->e_phnum; i++)
        {
            if (current_phdr->p_type == 1)
            {
                if (current_phdr->p_vaddr <= entrypoint_vaddr && current_phdr->p_vaddr + current_phdr->p_filesz >= entrypoint_vaddr)
                {
                    has_found = true;
                    break;
                }
            }

            current_phdr++;
        }

        if (! has_found)
        {
            console::PrintString("Couldn't find entrypoint, is the hypervisor corrupted?!\n");
            return 1;
        }

        uint64_t entrypoint_addr = hypervisor_elf + current_phdr->p_offset + (entrypoint_vaddr - current_phdr->p_vaddr);

        console::PrintString("Found entrypoint @ ");
        console::PrintHex(entrypoint_addr);
        console::PrintString("!\n");
        console::PrintString("Starting the hypervisor!\n");

        start_func_t test = (start_func_t) entrypoint_addr;

        pub::HypervisorStartConfig config;
        config.LoaderVersion = 0;
        config.MemeResult = 1337;
        
        console::PrintString("ps @ ");
        console::PrintHex((uint64_t) console::PrintString);
        console::PrintString("\n");

        
        config.LoaderFunctions.PrintString = LoaderPrintString;

        return test(&config);
    }

    void LoadHypervisorFromFS()
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
        efi::protocol::file_protocol_t* hypervisor_file = nullptr;
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
            return;
        }

        // Get the file size
        efi::protocol::file_info_t info;
        uint64_t info_size = sizeof(info);
        efi::guid_t info_guid = EFI_FILE_INFO_ID;
        status = hypervisor_file->get_info(hypervisor_file, &info_guid, &info_size, &info);

        if (status != 0)
        {
            con->output_string(con, L"Failed to get hypervisor file info\r\n");
            return;
        }

        con->output_string(con, L"Loading: ");
        con->output_string(con, info.file_name);
        con->output_string(con, L"\r\n");

        // Allocate enough memory for the hypervisor
        status = bs->allocate_pages(efi::allocate_any_pages, EfiRuntimeServicesData, EFI_SIZE_TO_PAGES(info.file_size), &hypervisor_elf);
        if (status != 0)
        {
            con->output_string(con, L"Failed to allocate memory for elf\r\n");
            return;
        }

        // Copy file to memory buffer
        uint64_t total_copied = 0;

        while (total_copied < info.file_size)
        {
            uint64_t buffer_size = info.file_size;
            status = hypervisor_file->read(hypervisor_file, &buffer_size, (void*)((uint64_t)(hypervisor_elf) + total_copied));
            if (status != 0)
            {
                con->output_string(con, L"Failed to read elf\r\n");
            }

            total_copied += buffer_size;
            log::HexNumber(total_copied);
        }
    }
    
    graphics::image_t* LoadFFImageFromFS(wchar_t* image_path)
    {
        efi::status_t status = 0;
        graphics::image_t* result;
        
        status = efi::system_table->boot_services->allocate_pages(
            efi::allocate_any_pages,
            EfiRuntimeServicesData,
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