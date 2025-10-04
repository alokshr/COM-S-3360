#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "renderers/raylib/clay_renderer_raylib.h"

const Clay_Color COLOR_LIGHT = (Clay_Color) {224, 215, 210, 255};

void HandleClayErrors(Clay_ErrorData errorData) {
    printf("%s", errorData.errorText.chars);
}

int main(int argc, char const *argv[])
{
    Font fonts[2];

    const int REGULAR_FONT = 0;
    const int OTHER_FONT = 1;

    fonts[REGULAR_FONT] = LoadFontEx("../resources/Roboto-Regular.ttf", 12, 0, 400);
    fonts[OTHER_FONT] = LoadFontEx("../resources/resources/RobotoMono-Medium.ttf", 12, 0, 400);

    Clay_Raylib_Initialize(1024, 768, "Clay UI Demo", FLAG_WINDOW_RESIZABLE);
    Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Window");

    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));
    
    Clay_Initialize(arena, (Clay_Dimensions) {
        .width = GetScreenHeight(),
        .height = GetScreenHeight()
    }, (Clay_ErrorHandler) {
        HandleClayErrors, 0
    });


    while (!WindowShouldClose()) {
        Clay_SetLayoutDimensions((Clay_Dimensions) {
            .width = (float) GetScreenWidth(),
            .height = (float) GetScreenHeight()
        });


        Clay_BeginLayout();
        CLAY({
            .id = CLAY_ID("OuterContainer"),
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_GROW(),
                    .height = CLAY_SIZING_GROW()
                }
            },
            .backgroundColor = COLOR_LIGHT
        }) {}
        Clay_RenderCommandArray renderCommands = Clay_EndLayout();

        BeginDrawing();
        ClearBackground(BLACK);
        Clay_Raylib_Render(renderCommands, fonts);
        EndDrawing();
    }

    return 0;
}
