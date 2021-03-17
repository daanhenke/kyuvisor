#include <stdint.h>
#include "efi/system-table.hh"
#include "efi/protocol/simple-filesystem.hh"
#include "efi/protocol/graphics-output.hh"

#include "loader.hh"
#include "graphics.hh"
#include "console.hh"

using namespace kyu;

efi::protocol::simple_text_output_protocol_t* con;
efi::boot_services_t* bs;

efi::system_table_t* efi::system_table = nullptr;

efi::status_t efi_main(efi::handle_t module, efi::system_table_t* system_table)
{
    efi::status_t status;
    
    efi::system_table = system_table;
    con = system_table->console_out;
    bs = system_table->boot_services;

    graphics::InitializeGraphics();
    loader::LocateHypervisor();
    graphics::LoadFont();

    con->clear_screen(con);
    graphics::DrawBackground();
    
    console::PrintString("Hello from KyuLoader :D\n");

    loader::MapHypervisor();

    uint64_t result = loader::RunHypervisor();
    console::PrintString("Got result: ");
    console::PrintHex(result);
    console::PrintString("\n");
    
    console::PrintString("Press any key tOwO continue...\n");
    bs->wait_for_event(1, &system_table->console_in->wait_for_key, nullptr);

    return EFI_CUSTOM_ERROR(result);
}