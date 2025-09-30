#ifndef _ENGINEH_
#define _ENGINEH_

//C++ libraries
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <string>
#include <map>
#include <math.h>
#include <unordered_map>

//SDL2
#include <SDL2/SDL.h>

//My library.
#include <tools/input.h>

#ifdef __linux__
	#include <unistd.h>
#endif
#ifdef __MINGW32__
	//for printf
	#include <stdio.h>
	// for sleep
	#include <windows.h>

	void usleep(int microseconds)
	{
		Sleep(microseconds / 1000);
	}
#endif

SDL_Window *Window;
SDL_Renderer *RenderInformation;
int screenWidth = 1000;
int screenHeigth = 500;
int backgroundColour[3] = {125,125,125};

//Function declaration
void Start();
void Update();
bool endApp = false;

const double PI = 3.141592653589793;

struct Lsystem
{
    std::string name;
    float moveDistance;
    float turnAngle; //deg
    float startAngle = 0.0f; //deg
    std::string axiom;
    std::map<char, std::string> rules;
    std::map<char, std::string> actionMap; //What symbols are replaced with what symbols.

    void PrintData()
    {
        std::cout << "# ----- " << name << " ----- #" << std::endl;
        std::cout << "Move amount: " << moveDistance << std::endl;
        std::cout << "Angle amount: " << turnAngle << std::endl;
        std::cout << "Axiom: " << axiom << std::endl;
        std::cout << "Rules: " << std::endl;
        for(auto i: rules)
        {
            std::cout << "\t" << i.first << ": " << i.second << std::endl;
        }
        std::cout << "Action map: " << std::endl;
        for(auto i: actionMap)
        {
            std::cout << "\t" << i.first << ": " << i.second << std::endl;
        }
    }
};

std::vector<Lsystem> lsysVec;

void LoadSettings(std::string path)
{
    std::string rowText;
    std::ifstream MyReadFile(path);
    static std::unordered_map<std::string, int> commandMap = {
        {"width", 1},
        {"heigth", 2},
    };
    while (std::getline (MyReadFile, rowText))
    {
        Lsystem lsys = {};
        char arr[rowText.length() + 1]; //char arr version of the string.
        strcpy(arr, rowText.c_str());

        char *targetptr = strtok(arr, ":");
        if(targetptr == NULL)
        {
            std::cout << "Error LoadSettings at row: " << rowText << ". Missing targetptr!" << std::endl;
            continue;
        }
        char *valueptr = strtok(NULL,":");
        if(valueptr == NULL)
        {
            std::cout << "Error LoadSettings at row: " << rowText << ". Missing valueptr!" << std::endl;
            continue;
        }
        
        switch (commandMap[targetptr])
        {
            case 1: //Width
                screenWidth = std::stoi(valueptr);
                break;
            case 2: //Height
                screenHeigth = std::stoi(valueptr);
                break;
            default:
                break;
        }
    }
}

void LoadData(std::string path, std::vector<Lsystem> *lsysVec)
{
    std::string rowText;
    std::ifstream MyReadFile(path);
    while (std::getline (MyReadFile, rowText))
    {
        //std::cout << "RowTEXT: " << rowText << std::endl;
        Lsystem lsys = {};
        char arr[rowText.length() + 1]; //char arr version of the string.
        strcpy(arr, rowText.c_str());

        char *tokenPtr = strtok(arr, ";");
        if(tokenPtr == NULL)
        {
            std::cout << "Error LoadData at row: " << rowText << std::endl;
            continue;
        }
        lsys.name = tokenPtr;
        tokenPtr = strtok(NULL, ";");
        if(tokenPtr == NULL)
        {
            std::cout << "Error LoadData at row: " << rowText << std::endl;
            continue;
        }
        lsys.moveDistance = std::stof(tokenPtr);
        tokenPtr = strtok(NULL, ";");
        if(tokenPtr == NULL)
        {
            std::cout << "Error LoadData at row: " << rowText << std::endl;
            continue;
        }
        lsys.turnAngle = std::stof(tokenPtr);
        tokenPtr = strtok(NULL, ";");
        if(tokenPtr == NULL)
        {
            std::cout << "Error LoadData at row: " << rowText << std::endl;
            continue;
        }
        lsys.axiom = tokenPtr;

        char *ruleToken = strtok(NULL, ";");
        char *actionToken = strtok(NULL, ";");

        char *rulePtr = strtok(ruleToken,":");
        while(rulePtr != NULL)
        {
            char *rulePtr2 = strtok(NULL,",");
            lsys.rules.insert(std::make_pair(*rulePtr,rulePtr2));
            rulePtr = strtok(NULL,":");
        }

        if(actionToken != NULL)
        {
            char *actionPtr = strtok(actionToken,":");
            while(actionPtr != NULL)
            {
                char *actionPtr2 = strtok(NULL,",");
                lsys.actionMap.insert(std::make_pair(*actionPtr,actionPtr2));
                actionPtr = strtok(NULL,":");
            }
        }

        lsysVec->push_back(lsys);
    }
}

struct Transformation
{
    float posX, posY, angle;
    bool draw = true;
};

std::string Iteration(Lsystem *lsys, int iterationRound = 1, std::string currentRow = "")
{
    if(currentRow == "")
    {
        currentRow = lsys->axiom;
    }
    std::string resultRow = "";
    for(int i = 0; i < currentRow.length(); i++)
    {
        std::string s = lsys->rules[currentRow[i]];
        if (s == "")
        {
            s = currentRow[i];
        }
        resultRow += s;
    }
    if(iterationRound > 1)
    {
        return Iteration(lsys, iterationRound - 1, resultRow);
    }
    return resultRow;
}

bool AllowedSymbols(char c)
{
    char symbolList[] = {'F','f','H','h','+','-','|','[',']'};
    for(int i = 0; i < sizeof(symbolList); i++)
    {
        if(c == symbolList[i])
        {
            return true;
        }
    }
    return false;
}

std::vector<Transformation> CalculateIteration(Lsystem *lsys, int iterations)
{
    std::string axiom = Iteration(lsys, iterations);
    std::string actionRow;

    for (char c : axiom)
    {
        auto it = lsys->actionMap.find(c);
        if (it != lsys->actionMap.end())
        {
            if(it->second != "X") //No point to do nothing!
            {
                for(int i = 0; i < it->second.size(); i++)
                {
                    if(AllowedSymbols(it->second.at(i)))
                    {
                        actionRow += it->second.at(i);
                    }
                }
            }
        }
        else
        {
            if(AllowedSymbols(c)) //Filters out symbols that do nothing.
            {
                actionRow += c;           
            }
        }
    }

    std::vector<Transformation> transformations = {{screenWidth / 2.0f, screenHeigth / 2.0f, (float)(-PI / 2.0 + lsys->startAngle / 180.0 * PI)}};
    std::vector<Transformation> splits;
    for(int i = 0; i < actionRow.length(); i++)
    {
        switch(actionRow[i])
        {
            case 'X': //Do nothing!!!
            {
                std::cout << "ERROR: Doing nothing!" << std::endl;
                break;
            }
            case 'F': //Move forward by line length drawing a line
            {
                Transformation t = {transformations[transformations.size() - 1].posX,transformations[transformations.size() - 1].posY,transformations[transformations.size() - 1].angle};
                t.posX += cos(t.angle) * lsys->moveDistance;
                t.posY += sin(t.angle) * lsys->moveDistance;
                transformations.push_back(t);
                break;
            }
            case 'f': //Move forward without drawing a line.
            {
                Transformation t = {transformations[transformations.size() - 1].posX,transformations[transformations.size() - 1].posY,transformations[transformations.size() - 1].angle};
                t.posX += cos(t.angle) * lsys->moveDistance;
                t.posY += sin(t.angle) * lsys->moveDistance;
                t.draw = false;
                transformations.push_back(t);
                break;
            }
            case 'H': //Move forward by line length drawing a line
            {
                Transformation t = {transformations[transformations.size() - 1].posX,transformations[transformations.size() - 1].posY,transformations[transformations.size() - 1].angle};
                t.posX += cos(t.angle) * lsys->moveDistance / 2;
                t.posY += sin(t.angle) * lsys->moveDistance / 2;
                transformations.push_back(t);
                break;
            }
            case 'h': //Move forward without drawing a line.
            {
                Transformation t = {transformations[transformations.size() - 1].posX,transformations[transformations.size() - 1].posY,transformations[transformations.size() - 1].angle};
                t.posX += cos(t.angle) * lsys->moveDistance / 2;
                t.posY += sin(t.angle) * lsys->moveDistance / 2;
                t.draw = false;
                transformations.push_back(t);
                break;
            }
            case '+': //Turn left by turning angle
            {
                Transformation t = {transformations[transformations.size() - 1].posX,transformations[transformations.size() - 1].posY,transformations[transformations.size() - 1].angle};
                t.angle = std::fmod(t.angle + PI * 2 - PI / 180.0f * lsys->turnAngle, PI * 2);
                transformations.push_back(t);
                break;
            }
            case '-': //Turn right by turning angle
            {
                Transformation t = {transformations[transformations.size() - 1].posX,transformations[transformations.size() - 1].posY,transformations[transformations.size() - 1].angle};
                t.angle = std::fmod(t.angle + PI / 180.0f * lsys->turnAngle,PI * 2);
                transformations.push_back(t);
                break;
            }
            case '|': //Turn 180 deg
            {
                Transformation t = {transformations[transformations.size() - 1].posX,transformations[transformations.size() - 1].posY,transformations[transformations.size() - 1].angle};
                t.angle = std::fmod(t.angle + PI ,PI * 2);
                transformations.push_back(t);
                break;
            }
            case '[': //Push new state to stack -> "Split"
            {
                splits.push_back(transformations[transformations.size() - 1]);
                Transformation t = {transformations[transformations.size() - 1].posX,transformations[transformations.size() - 1].posY,transformations[transformations.size() - 1].angle};
                transformations.push_back(t);
                break;
            }
            case ']': //Pop last state from the stack -> Come back to the splitted position.
            {
                Transformation t = {splits[splits.size() - 1].posX,splits[splits.size() - 1].posY,splits[splits.size() - 1].angle, false};
                transformations.push_back(t);
                splits.pop_back();
                break;
            }
            default:
            {
                std::cout << "ERROR in Draw Iteration with symbol: " << axiom[i] << std::endl;
                break;
            }
        }
    }
    return transformations;
}

bool createWindow()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    Window = SDL_CreateWindow("SDL Practice",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,screenWidth,screenHeigth,SDL_WINDOW_ALLOW_HIGHDPI);
    if (Window != NULL)
    {
        RenderInformation = SDL_CreateRenderer(Window, -1, 0);
        std::cout << "Did create a window!" << std::endl; 
    }
    else
    {
        std::cout << "Could not create window: " << SDL_GetError() << std::endl;
    }
    return Window != NULL;
}

void closeWindow()
{
    SDL_DestroyWindow(Window);
    SDL_Quit();
}


int iterationCount = 4;
int lsysIndex = 0;
float offsetX = 0;
float offsetY = 0;
float zoom = 1;
float offsetSpeed = 50.0f;

void RenderFrame(std::vector<Transformation> lines)
{
    // ----- Draw background ----- //
    SDL_SetRenderDrawColor(RenderInformation, backgroundColour[0], backgroundColour[1], backgroundColour[2], 255);
    SDL_RenderClear(RenderInformation); //Fills the screen with the background colour
    SDL_SetRenderDrawColor(RenderInformation,125,0,0,255);
    float zoomOffsetX = screenWidth - screenWidth / 2 * zoom;
    float zoomOffsetY = screenHeigth - screenHeigth / 2 * zoom;
    for(int i = 1; i < lines.size(); i++)
    {
        if(lines[i].draw)
        {
            int x1 = lines[i-1].posX * zoom + zoomOffsetX  + offsetX * zoom - screenWidth / 2; //lines[i-1].posX * zoom + offsetX - screenWidth + screenWidth * zoom;
            int y1 = lines[i-1].posY * zoom + zoomOffsetY  + offsetY * zoom - screenHeigth / 2;// * zoom + offsetY - screenHeigth + screenHeigth * zoom;
            int x2 = lines[i].posX * zoom + zoomOffsetX + offsetX * zoom - screenWidth / 2;// * zoom + offsetX - screenWidth + screenWidth * zoom;
            int y2 = lines[i].posY * zoom + zoomOffsetY + offsetY * zoom - screenHeigth / 2;// * zoom + offsetY - screenHeigth + screenHeigth * zoom;
            SDL_RenderDrawLine(RenderInformation,x1,y1,x2,y2);
        }
    }

    SDL_RenderPresent(RenderInformation);
}

std::vector<Transformation> lines;

// ----- Time ----- //
double deltaTime;
clock_t t1, t2; //Time at start and end of the frame
double elapsedTime;
double sessionTime = 0; //Total time the session has been on.
int fpsLimiter = 60;

void Restart()
{
    offsetX = 0;
    offsetY = 0;
    zoom = 1;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    LoadSettings("settings.config");
    std::cout << "Starting" << std::endl;
    if(createWindow() == false)
    {
        return 1;
    }
    Start();

    
    std::cout << "Start completed succesfully" << std::endl;

    while (true)
    {
        //Begining of calculating time.
        t1 = clock();
        //mingw_gettimeofday(&t1, NULL); 

        SDL_Event Event;
        if (SDL_PollEvent(&Event))
        {
            if (Event.type == SDL_QUIT) //Ending application.
            {
                endApp = true;
            }
            if(Event.type == SDL_MOUSEMOTION)
            {
                if(Event.button.button == 1)
                {
                    if(Event.motion.xrel != 0 || Event.motion.yrel != 0)
                    {
                        offsetX += Event.motion.xrel / zoom;
                        offsetY += Event.motion.yrel / zoom;
                        RenderFrame(lines);
                    }
                }                
            }
            if(Event.type == SDL_MOUSEWHEEL)
            {
                zoom += Event.wheel.y * 0.1;
                if(zoom <= 0)
                {
                    zoom = 0.1f;
                }
                RenderFrame(lines);
            }
        }
        if(endApp)
        {
            break;
        }

        UpdateInputs(Event);

        Update();

        UpdatePreviousInputs(Event); //Updates previousinputs, used for keyUp and keyDown functions

        //Calculating passing time.
        //mingw_gettimeofday(&t2, NULL);
        t2 = clock();
        deltaTime = (double)(t2 - t1) / CLOCKS_PER_SEC;
        sessionTime += deltaTime;

        //fps limiter
        usleep(std::max(0.0,(1000000 / fpsLimiter) - (deltaTime * 1000000)));
    }

    closeWindow();
    return 1;
}

void QuitApplication(){
    endApp = true;
}

void Start()
{
    LoadData("Presets.txt", &lsysVec);
    lines = CalculateIteration(&lsysVec[lsysIndex],iterationCount);
    RenderFrame(lines);
}

void Update()
{
    if(GetActionDownByName("Left"))
    {
        lsysIndex = (lsysIndex + lsysVec.size() - 1) % lsysVec.size();
        iterationCount = 3;
        lines = CalculateIteration(&lsysVec[lsysIndex],iterationCount);
        Restart();
        RenderFrame(lines);
    }
    else if(GetActionDownByName("Right"))
    {
        lsysIndex = (lsysIndex + 1) % lsysVec.size();
        iterationCount = 3;
        lines = CalculateIteration(&lsysVec[lsysIndex],iterationCount);
        Restart();
        RenderFrame(lines);
    }
    else if(GetActionDownByName("Up"))
    {
        iterationCount++;
        lines = CalculateIteration(&lsysVec[lsysIndex],iterationCount);
        RenderFrame(lines);
    }
    else if(GetActionDownByName("Down"))
    {
        if(iterationCount > 1)
        {
            iterationCount--;
            lines = CalculateIteration(&lsysVec[lsysIndex],iterationCount);
            RenderFrame(lines);
        }
    }

    if(GetActionByName("Up2"))
    {
        offsetY += offsetSpeed * deltaTime;
        RenderFrame(lines);
    }
    else if(GetActionByName("Down2"))
    {
        offsetY -= offsetSpeed * deltaTime;
        RenderFrame(lines);
    }

    if(GetActionByName("Left2"))
    {
        offsetX += offsetSpeed * deltaTime;
        RenderFrame(lines);
    }
    else if(GetActionByName("Right2"))
    {
        offsetX -= offsetSpeed * deltaTime;
        RenderFrame(lines);
    }
}

#endif