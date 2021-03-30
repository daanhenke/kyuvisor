#include "graphics.hh"
#include "loader.hh"
#include "log.hh"
#include "console.hh"
#include "barelib/string.hh"

namespace kyu::graphics
{
    efi::protocol::graphics_output_protocol_t* protocol;
    image_t* font_image;

    void DrawImage(graphics::image_t* image, uint64_t x, uint64_t y, uint64_t source_x = 0, uint64_t source_y = 0, uint64_t width = 0, uint64_t height = 0)
    {
        if (width == 0) width = image->width;
        if (height == 0) height = image->height;
        auto status =protocol->blt(protocol, image->data, efi::protocol::blt_buffer_to_video, source_x, source_y, x, y, width, height, image->width * 4);
    }

    void DrawChar(char character, uint64_t x, uint64_t y)
    {
        static uint64_t char_width = font_image->width / 16;
        static uint64_t char_height = font_image->height / 16;

        auto index_y = character / 16;
        auto index_x = character % 16;
        DrawImage(font_image, x, y, index_x * char_width, index_y * char_height, char_width, char_height);
    }

    void DrawString(const char* string, uint64_t* x, uint64_t *y)
    {
        static uint64_t char_width = font_image->width / 16;
        static uint64_t char_height = font_image->height / 16;

        int offset_x = 0;
        int offset_y = 0;
        for (int i = 0; i < string_length(string); i++)
        {
            auto chr = string[i];
            auto target_x = *x + offset_x * char_width;
            auto target_y = *y + offset_y * char_height;

            if (chr == '\n')
            {
                offset_x = 0;
                offset_y++;
            }
            else if (chr == '\t')
            {
                offset_x += 4 - (offset_x % 4);
            }
            else
            {
                DrawChar(chr, target_x, target_y);
                offset_x++;
            }

            if (target_x + char_width > protocol->mode->info->horizontal_resolution)
            {
                offset_x = 0;
                offset_y++;
            }
        }
    }

    void DrawConsole()
    {
        static uint64_t char_width = font_image->width / 16;
        static uint64_t char_height = font_image->height / 16;

        for (int j = 0; j < CONSOLE_HEIGHT; j++)
        {
            for (int i = 0; i < CONSOLE_WIDTH; i++)
            {
                auto chr = console::GetCell(i, j);
                if (chr == '\0') continue;
                DrawChar(chr, i * char_width, j * char_height);
            }
        }
    }

    void DrawBackground()
    {
        static auto* weeb_image = loader::LoadFFImageFromFS(L"\\visor\\images\\crusty-smiley.ff");

        if (weeb_image == nullptr)
        {
            log::String(L"Failed!\r\n");
        }

        DrawImage(weeb_image, protocol->mode->info->horizontal_resolution / 2 - weeb_image->width / 2, protocol->mode->info->vertical_resolution - weeb_image->height);
    }

    void Panic(int code)
    {
        auto con = efi::system_table->console_out;

        con->clear_screen(con);
        DrawBackground();

        con->output_string(con, L"PANIC!\r\n\r\n");
        con->output_string(con, L"Error Code: ");
        
    }

    void InitializeGraphics()
    {
        efi::status_t status = 0;
        efi::guid_t gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
        efi::handle_t* gop_handles;
        uint64_t gop_handlecount;

        auto con = efi::system_table->console_out;
        auto bs = efi::system_table->boot_services;

        status = bs->locate_handle_buffer(efi::by_protocol, &gop_guid, nullptr, &gop_handlecount, &gop_handles);

        if (status != 0)
        {
            con->output_string(con, L"Failed to get graphics handle\r\n");
            return;
        }

        for (int i = 0; i < gop_handlecount; i++)
        {
            efi::protocol::graphics_output_protocol_t* gfx;
            status = bs->handle_protocol(gop_handles[i], &gop_guid, (void**) &gfx);

            if (status != 0)
            {
                con->output_string(con, L"Failed to get graphics protocol\r\n");
                continue;
            }

            protocol = gfx;
            break;
        }

        uint64_t info_size;
        uint64_t current_horizontal_resolution = protocol->mode->info->horizontal_resolution;
        int chosen_mode = protocol->mode->mode;
        efi::protocol::graphics_output_mode_information_t* info;
        
        for (int i = 0; i < protocol->mode->max_mode; i++)
        {
            status = protocol->query_mode(protocol, 0, &info_size, &info);

            if (status != 0)
            {
                con->output_string(con, L"Failed to get graphics mode info\r\n");
                break;
            }

            if (info->horizontal_resolution > current_horizontal_resolution)
            {
                current_horizontal_resolution = info->horizontal_resolution;
                chosen_mode = i;
            }
        }

        if (status != 0) return;

        if (protocol->mode->mode != chosen_mode) protocol->set_mode(protocol, chosen_mode);
    }

    void LoadFont()
    {
        font_image = loader::LoadFFImageFromFS(L"\\visor\\images\\font12.ff");
    }
}
