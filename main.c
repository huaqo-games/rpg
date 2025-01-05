#include <raylib.h>
#include <stdio.h>

const int TILEMAP_COLUMNS = 16;
const int TILEMAP_ROWS = 10;
const int TILE_WIDTH = 8;
const int TILE_HEIGHT = 8;
const int WORLD_WIDTH = 100;
const int WORLD_HEIGHT = 100;
const int screenWidth = 800;
const int screenHeight = 600;
const char *gameTitle = "RPG";
const int fps = 60;

// Camera
Camera2D camera = {0};
const Vector2 initialTarget = {0, 0};
const Vector2 initialOffset = {(float)(screenWidth / 2) - TILE_WIDTH, (float)(screenHeight / 2) - TILE_HEIGHT};
const float initialRotation = 0.0f;
const float initialZoom = 5.0f;

// Texture
const int MAX_TEXTURES = 1
typedef enum
{
    TEXTURE_TILEMAP = 0
} texture_asset;

Texture2D textures[MAX_TEXTURES];

typedef struct
{
    int x;
    int y;
} sTile;

sTile world[WORLD_WIDTH][WORLD_HEIGHT];

// player
typedef struct
{
    int x;
    int y;
} sEntity;

sEntity player;

// Audios
const int MAX_SOUNDS = 1
typedef enum
{
    SOUNDTRACK_AUDIO = 0
} audio_asset;

Sound sounds[MAX_SOUNDS];

// functions
void DrawTile(int pos_x, int pos_y, int tile_x, int tile_y)
{

    Rectangle source = {
        (float)(TILE_WIDTH * tile_x),
        (float)(TILE_HEIGHT * tile_y),
        (float)(TILE_WIDTH),
        (float)(TILE_HEIGHT)};

    Rectangle dest = {
        (float)(pos_x),
        (float)(pos_y),
        (float)(TILE_WIDTH),
        (float)(TILE_HEIGHT)};

    Vector2 origin = {0, 0};

    DrawTexturePro(textures[TEXTURE_TILEMAP], source, dest, origin, 0.0f, WHITE);
}

void GameStartup(Vector2 initialTarget, Vector2 initialOffset, float initialRotation, float initialZoom)
{
    // Player setup
    player = (sEntity){
        .x = 0,
        .y = 0};

    // Camera setup
    camera.target = (Vector2){player.x, player.y};
    camera.offset = initialOffset;
    camera.rotation = initialRotation;
    camera.zoom = initialZoom;

    // Audio setup
    InitAudioDevice();
    Wave wave = LoadWave("assets/wav/Dark Ambient 1.wav");
    sounds[SOUNDTRACK_AUDIO] = LoadSoundFromWave(wave);
    UnloadWave(wave);
    PlaySound(sounds[SOUNDTRACK_AUDIO]);

    // Texture setup
    Image image = LoadImage("assets/Tilemap/colored_tilemap_packed.png");
    textures[TEXTURE_TILEMAP] = LoadTextureFromImage(image);
    UnloadImage(image);

    // World setup

    for (int i = 0; i < WORLD_WIDTH; i++)
    {
        for (int j = 0; j < WORLD_HEIGHT; j++)
        {
            world[i][j] = (sTile){
                .x = i,
                .y = j,
            };
        }
    }
}

void GameUpdate()
{
    // WASD Movement
    if (IsKeyReleased(KEY_A))
        player.x -= 8.0f;
    if (IsKeyReleased(KEY_D))
        player.x += 8.0f;
    if (IsKeyReleased(KEY_W))
        player.y -= 8.0f;
    if (IsKeyReleased(KEY_S))
        player.y += 8.0f;

    // Arrow movement
    if (IsKeyReleased(KEY_LEFT))
        player.x -= 8.0f;
    if (IsKeyReleased(KEY_RIGHT))
        player.x += 8.0f;
    if (IsKeyReleased(KEY_UP))
        player.y -= 8.0f;
    if (IsKeyReleased(KEY_DOWN))
        player.y += 8.0f;

    camera.target.x = player.x + TILE_WIDTH / 2;
    camera.target.y = player.y + TILE_HEIGHT / 2;

    printf("Player X%d", player.x);

    // MouseWheelZoom
    float wheelMove = GetMouseWheelMove();
    if (wheelMove != 0)
    {
        const float zoomIncrement = 0.125f;
        camera.zoom += wheelMove * zoomIncrement;
        if (camera.zoom < 2.5f)
            camera.zoom = 2.5f;
        if (camera.zoom > 10.0f)
            camera.zoom = 10.0f;
    }
}

void GameRender(Vector2 initialTarget)
{
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(camera);

    // Floor
    for (int i = 0; i < WORLD_WIDTH; i++)
    {
        for (int j = 0; j < WORLD_HEIGHT; j++)
        {
            sTile tile = world[i][j];
            DrawTile(tile.x * TILE_WIDTH, tile.y * TILE_HEIGHT, 4, 4);
        }
    }

    // Grid
    for (int x = -WORLD_WIDTH * TILE_WIDTH * 10; x <= WORLD_WIDTH * TILE_WIDTH * 10; x += TILE_WIDTH)
    {
        DrawLine(x, -1000, x, 1000, GRAY);
    }
    for (int y = -WORLD_WIDTH * TILE_WIDTH * 10; y <= WORLD_WIDTH * TILE_WIDTH * 10; y += TILE_WIDTH)
    {
        DrawLine(-1000, y, 1000, y, GRAY);
    }

    // Manual
    DrawText("Use WASD to move the camera", 10, 10, 10, LIGHTGRAY);
    DrawText("Scroll to zoom in/out", 10, 40, 10, LIGHTGRAY);

    // Player
    DrawTile(player.x, player.y, 15, 0);

    EndMode2D();

    // Debug
    DrawText(TextFormat("Player X: %.2f", player.x), 10, 0, 20, LIGHTGRAY);
    DrawText(TextFormat("Player Y: %.2f", player.y), 10, 20, 20, LIGHTGRAY);
    DrawText(TextFormat("Zoom Level: %.2f", camera.zoom), 10, 80, 20, LIGHTGRAY);

    EndDrawing();
}

void GameShutdown()
{

    for (int i = 0; i < MAX_TEXTURES; i++)
    {
        UnloadTexture(textures[i]);
    }

    for (int i = 0; i < MAX_SOUNDS; i++)
    {
        UnloadSound(sounds[i]);
    }

    CloseAudioDevice();
}

int main()
{
    InitWindow(screenWidth, screenHeight, gameTitle);
    SetTargetFPS(fps);
    GameStartup(initialTarget, initialOffset, initialRotation, initialZoom);
    while (!WindowShouldClose())
    {
        GameUpdate();
        GameRender(initialTarget);
    }
    GameShutdown();
    CloseWindow();
    return 0;
}