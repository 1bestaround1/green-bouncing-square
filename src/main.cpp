#include <iostream> 
#include <cstdio> 
#include <cstdlib>
#include <SDL2/SDL.h> 
using namespace std; 


// Colors 

namespace Colors { 
    const SDL_Color BLACK = {0, 0, 0, SDL_ALPHA_OPAQUE};
    const SDL_Color GREEN = {0, 255, 0, SDL_ALPHA_OPAQUE}; 
}


// Graphics 

namespace Graphics 
{
    struct Screen 
    {
        const int WIDTH = 512; 
        const int HEIGHT = 284;

        const int center_x = WIDTH /2; 
        const int center_y = HEIGHT /2; 
    };

};

// Game objects

enum SquareState { IDLE, MOVING_UP, MOVING_DOWN, MOVING_LEFT, MOVING_RIGHT};


struct Square 
{
    int HEIGHT;
    int WIDTH; 
    int x; 
    int y; 
    SquareState state;
    int step;
};

struct App
{
    const int DEFAULT_STEP = 2; 

    SDL_Window* window; 
    SDL_Renderer* renderer;

    Graphics::Screen screen;

    Square square = {
        32,
        32,
        screen.center_x,
        screen.center_y,
        IDLE,
        DEFAULT_STEP

    };

} app; 



//const int WINDOW_WIDTH = 584;
//const int WINDOW_HEIGHT = 284; 
//SDL_Window* g_main_window;
//SDL_Renderer* g_main_renderer;

bool InitSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "SDL_Init failed with error: " << SDL_GetError() << endl;


        return false;
            }

    return true; 
}

void ShutdownApplication() {
    if (app.window != nullptr) {
        SDL_DestroyWindow(app.window); 
        app.window = nullptr;
    }

    if (app.renderer != nullptr) {
        SDL_DestroyRenderer(app.renderer);
        app.renderer = nullptr;
    }

    SDL_Quit();

}


static bool InitApplication()
{
    if (InitSDL() == false)
    {
        ShutdownApplication();
        return false;
    }

    app.window = SDL_CreateWindow(
        "SDL Create Window (512x284)",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        app.screen.WIDTH,
        app.screen.HEIGHT, 
        SDL_WINDOW_OPENGL
    );

    if (app.window == nullptr) {
        cout << "Unable to create the main window. Error: " << SDL_GetError() << endl; 
        SDL_Quit();
        return false;
    }

    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_PRESENTVSYNC); 

    return true;
    
    
}

static void ClearScreen(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, Colors::BLACK.r,Colors::BLACK.g,Colors::BLACK.b,Colors::BLACK.a);
    SDL_RenderClear(renderer); 
    
}

void IncreaseStep() {
    app.square.step += app.DEFAULT_STEP;
}

void DecreaseStep() {
    int new_step = app.square.step / app.DEFAULT_STEP;
    if (new_step > app.DEFAULT_STEP) {
        app.square.step = new_step;
    }
    else 
    {
        app.square.step = app.DEFAULT_STEP;
    }
}

void DrawSquare(int x, int y) 
{
    SDL_Rect rect;
    rect.x = x - (app.square.WIDTH /2); 
    rect.y = y - (app.square.HEIGHT /2);
    rect.w = app.square.WIDTH;
    rect.h = app.square.HEIGHT; 

    // Define the green color on the renderer
    SDL_SetRenderDrawColor(app.renderer, Colors::GREEN.r,  Colors::GREEN.g,  Colors::GREEN.b,  Colors::GREEN.a );

    // ask the renderer to fill our rect with the green color
    SDL_RenderFillRect(app.renderer, &rect);
}

void SetState(SquareState newState)
{
    app.square.state = newState;

}



int main(int argc, char* argv[]) 
{
   if (InitApplication() == false) { ShutdownApplication(); } 

   SDL_Event event; 
   bool running = true; 

   while(running) { 
    ClearScreen(app.renderer);

    // Draw the new content on the renderer

    DrawSquare(app.square.x, app.square.y); 

    // check the square's state

    switch (app.square.state) 
    {
        case MOVING_UP:
        {
            if (app.square.y > (app.square.HEIGHT/2)) { 
                app.square.y = app.square.y - app.square.step; 
            }

            else {
                app.square.y = (app.square.HEIGHT/2);
                SetState(MOVING_DOWN);
            }
            
            break;
            
        }

        case MOVING_DOWN:
        {
            
            if ((app.square.y + (app.square.HEIGHT/2)) < app.screen.HEIGHT)
            {           
                app.square.y = app.square.y + app.square.step; 
            }

            else 
            {
                app.square.y = app.screen.HEIGHT - (app.square.HEIGHT / 2);
                SetState(MOVING_UP);
            }

            break;
           
        }

        case MOVING_LEFT:
        {
            if (app.square.x > app.square.WIDTH /2 ) {
                
                app.square.x = app.square.x - app.square.step; 
            }

            else 
            {
                app.square.x = app.square.WIDTH /2; 
                SetState(MOVING_RIGHT);
            }
            
            break;
        }

        case MOVING_RIGHT:
        {
            if (app.square.x + (app.square.WIDTH /2 )< app.screen.WIDTH) 
            {
                app.square.x = app.square.x + app.square.step; 
            }
            else 
            {
                SetState(MOVING_LEFT);
                // Do nothing
            }
           
            break;
        }

        default:
        break; 

        
    }

    if (SDL_PollEvent(&event)) {

        switch (event.type) {
            case SDL_KEYDOWN: {
                running = event.key.keysym.scancode != SDL_SCANCODE_ESCAPE;
                
                if (event.key.keysym.scancode == SDL_SCANCODE_UP)
                {
                    SetState(MOVING_UP);
                }

                 if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                {
                    SetState(MOVING_DOWN);
                }

                 if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)
                {
                    SetState(MOVING_LEFT);
                }

                  if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
                {
                    SetState(MOVING_RIGHT);
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_S)
                {
                    SetState(IDLE);
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_I)
                {
                    IncreaseStep();
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_D)
                {
                    DecreaseStep();
                }

                break;
            }

            case SDL_QUIT: { 
                running = false; 
                break;
            }

            default: 
            break; 


        }
    }

   SDL_RenderPresent(app.renderer);
   }

   ShutdownApplication();
   return EXIT_SUCCESS;
}