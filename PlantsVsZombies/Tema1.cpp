#include "lab_m1/Tema1/Tema1.h"

#include "lab_m1/Tema1/hexagon.h"
#include "lab_m1/Tema1/star.h"
#include "lab_m1/Tema1/square.h"
#include "lab_m1/Tema1/rectangle.h"
#include "lab_m1/Tema1/romb.h"

#include "lab_m1/Tema1/transform2D.h"
#include <vector>
#include <iostream>
#include <math.h>
#include <chrono>
#include <cstdlib>


using namespace std;
using namespace m1;
using namespace transform2D;


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


std::chrono::steady_clock::time_point lastSpawnTimeHexa = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point lastSpawnTimeStar = std::chrono::steady_clock::now();


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // creating an instance of every object to create meshes for said objects
    Square squareInstance;
    Rectangle rectangleInstance;
    Hexagon hexagonInstance;
    Romb rombInstance;
    Star starInstance;

    /*********************** positions ***********************/
    resx = resolution.x;
    resy = resolution.y;

    corner = glm::vec3(0, 0, 0);
    gridLeftBottom = glm::vec3(100.0f, 10.0f, 0.0f);
    healthRectangleLeftBottom = glm::vec3(20.0f, 10.0f, 0.0f);
    guiSquareLeftBottom = glm::vec3(20.0f, resy - 10.0f - squareSize, 0.0f);
    healthSquareLeftBottom = glm::vec3(resx - squareSize * 0.6f - 20.0f, resy - squareSize * 0.6f - 20.0f, 0.0f);

    //compute the middle position of each grid square (to help with romb placement, hexa aligning...) 
    for (int row = 0; row < 3; row++) {
        squareCentersX[row] = gridLeftBottom[0] + row * squareInstance.spacing + row * squareInstance.length + squareInstance.length * 0.5f;
        squareCentersY[row] = gridLeftBottom[1] + row * squareInstance.spacing + row * squareInstance.length + squareInstance.length * 0.5f;
    }

    //compute the middle position of each gui square (to help with GUI rombs & stars placement) 
    for (int i = 0; i < 4; i++)
        guiSquareCentersX[i] = guiSquareLeftBottom[0] + i * (squareSpacing * 2) + i * squareSize + squareSize / 2;
    guiSquareCentersY = guiSquareLeftBottom[1] + squareSize / 2;


    /*********************** colors ***********************/
    glm::vec3 orange(1.0f, 0.376f, 0.0f);           // 0
    glm::vec3 blue(0.0f, 0.325f, 1.0f);             // 1
    glm::vec3 yellow(1.0f, 0.675f, 0.0f);           // 2
    glm::vec3 purple(1.0f, 0.0f, 0.8f);             // 3
    glm::vec3 gridColor(0.463f, 1.0f, 0.302f);
    glm::vec3 healthColor(1.0f, 0.0f, 0.0f);
    glm::vec3 wGlow(0.929f, 0.929f, 0.929f);
    glm::vec3 gGlow(0.976f, 0.949f, 0.584f);
    glm::vec3 intHexColor = gGlow;
    glm::vec3 black(0.0f, 0.0f, 0.0f);
    glm::vec3 red(1.0f, 0.0f, 0.0f);


    /*********************** meshes ***********************/
    // mesh for grid square
    Mesh* gridSquare = squareInstance.CreateSquare("gridSquare", corner, squareInstance.length, gridColor);
    AddMeshToList(gridSquare);

    // mesh for health rectangle
    Mesh* healthRectangle = rectangleInstance.CreateRectangle("healthRectangle", corner, rectangleInstance.width, rectangleInstance.length, healthColor);
    AddMeshToList(healthRectangle);

    // mesh for gui star (black, romb prices stars) 
    Mesh* guiStar = starInstance.CreateStar("guiStar", corner, starInstance.radius * 0.5f, black);
    AddMeshToList(guiStar);

    // meshes for projectile stars
    Mesh* orangeStar = starInstance.CreateStar("orangeStar", corner, starInstance.radius, orange);
    AddMeshToList(orangeStar);
    Mesh* blueStar = starInstance.CreateStar("blueStar", corner, starInstance.radius, blue);
    AddMeshToList(blueStar);
    Mesh* yellowStar = starInstance.CreateStar("yellowStar", corner, starInstance.radius, yellow);
    AddMeshToList(yellowStar);
    Mesh* purpleStar = starInstance.CreateStar("purpleStar", corner, hexaRadius, purple);
    AddMeshToList(purpleStar);

    // meshes for enemy hexagons
    Mesh* intHexa = hexagonInstance.CreateInsideCircle("intHexa", corner, hexagonInstance.radius * 0.5f, intHexColor, 6);
    AddMeshToList(intHexa);
    Mesh* orangeExtHexa = hexagonInstance.CreateCircle("orangeExtHexa", corner, hexagonInstance.radius, orange, 6);
    AddMeshToList(orangeExtHexa);
    Mesh* blueExtHexa = hexagonInstance.CreateCircle("blueExtHexa", corner, hexagonInstance.radius, blue, 6);
    AddMeshToList(blueExtHexa);
    Mesh* yellowExtHexa = hexagonInstance.CreateCircle("yellowExtHexa", corner, hexagonInstance.radius, yellow, 6);
    AddMeshToList(yellowExtHexa);
    Mesh* purpleExtHexa = hexagonInstance.CreateCircle("purpleExtHexa", corner, hexagonInstance.radius, purple, 6);
    AddMeshToList(purpleExtHexa);

    // meshes for allied rombs
    Mesh* orangeRomb = rombInstance.CreateRomb("orangeRomb", corner, rombLength, orange);
    AddMeshToList(orangeRomb);
    Mesh* blueRomb = rombInstance.CreateRomb("blueRomb", corner, rombLength, blue);
    AddMeshToList(blueRomb);
    Mesh* purpleRomb = rombInstance.CreateRomb("purpleRomb", corner, rombLength, purple);
    AddMeshToList(purpleRomb);
    Mesh* yellowRomb = rombInstance.CreateRomb("yellowRomb", corner, rombLength, yellow);
    AddMeshToList(yellowRomb);

    // mesh for resource star that is getting randomly spawned
    Mesh* spawnedGuiStar = starInstance.CreateStar("spawnedGuiStar", corner, starInstance.radius * 0.8f, black);
    AddMeshToList(spawnedGuiStar);
    
    // mesh for black contour gui square
    Mesh* blackContourSquare = squareInstance.CreateSquare("blackContourSquare", corner, squareSize, black, false);
    AddMeshToList(blackContourSquare);

    // mesh for red life gui square
    Mesh* lifeSquare = squareInstance.CreateSquare("lifeSquare", corner, squareSize * 0.6f, red);
    AddMeshToList(lifeSquare);


    /*********************** objects that are always rendered ***********************/
    // creating objects for each grid square
    for (int i = 0; i < 9; i++) {
        gridSquares.push_back(Square());
        gridSquares[i].mesh = meshes["gridSquare"];
    }

    for (int i = 0; i < 4; i++) {
        blackContourSquares.push_back(Square());
        blackContourSquares[i].mesh = meshes["blackContourSquare"];
        blackContourSquares[i].colorOfRomb = i;

    }
    blackContourSquares[0].priceOfRomb = 1;
    blackContourSquares[1].priceOfRomb = 2;
    blackContourSquares[2].priceOfRomb = 2;
    blackContourSquares[3].priceOfRomb = 3;
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.133f, 0.161f, 0.133f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    // render grid
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            modelMatrix =  gridSquares[2 * row + row + col].gridPlacement(row, col, gridLeftBottom[0], gridLeftBottom[1]);
            RenderMesh2D(gridSquares[2 * row + row + col].mesh, shaders["VertexColor"], modelMatrix);
        }
    }

    // render health rectangle
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(20, 10);
    RenderMesh2D(meshes["healthRectangle"], shaders["VertexColor"], modelMatrix);


    // render resources (black stars)
    for (int i = 0; i < resources; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(guiSquareLeftBottom[0] + 950 + i * 40, guiSquareCentersY - 50);
        RenderMesh2D(meshes["guiStar"], shaders["VertexColor"], modelMatrix);
    }

    // render remaining lives
    for (int i = 0; i < lives; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(healthSquareLeftBottom[0] - i * (squareSpacing * 2 + squareSize * 0.6f), healthSquareLeftBottom[1]);
        RenderMesh2D(meshes["lifeSquare"], shaders["VertexColor"], modelMatrix);
    }

    if (lives == 0) {
        // interrupt the game
         cout << "*******************  G A M E   O V E R  *******************" << endl;
        
    }

    // render gui black contours
    for (int i = 0; i < 4; i++) {
        modelMatrix = glm::mat3(1);
        blackContourSquares[i].translateX = guiSquareLeftBottom[0] + i * (squareSize + 2 * squareSpacing);
        blackContourSquares[i].translateY = guiSquareLeftBottom[1];
        modelMatrix *= transform2D::Translate(guiSquareLeftBottom[0] + i * (squareSize + 2 * squareSpacing), guiSquareLeftBottom[1]);
        RenderMesh2D(blackContourSquares[i].mesh, shaders["VertexColor"], modelMatrix);

    }

    // render gui rombs + prices
    // orange romb + price
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(guiSquareCentersX[0], guiSquareCentersY);
    RenderMesh2D(meshes["orangeRomb"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(guiSquareCentersX[0] - squareSize * 0.3f, guiSquareCentersY - squareSize * 0.5f - hexaRadius * 0.4f);
    RenderMesh2D(meshes["guiStar"], shaders["VertexColor"], modelMatrix);

    // blue romb + price
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(guiSquareCentersX[1], guiSquareCentersY);
    RenderMesh2D(meshes["blueRomb"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(guiSquareCentersX[1] - squareSize * 0.3f, guiSquareCentersY - squareSize * 0.5f - hexaRadius * 0.4f);
    RenderMesh2D(meshes["guiStar"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2D::Translate(hexaRadius * 0.7f, 0);
    RenderMesh2D(meshes["guiStar"], shaders["VertexColor"], modelMatrix);

    // yellow romb + price
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(guiSquareCentersX[2], guiSquareCentersY);
    RenderMesh2D(meshes["yellowRomb"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(guiSquareCentersX[2] - squareSize * 0.3f, guiSquareCentersY - squareSize * 0.5f - hexaRadius * 0.4f);
    RenderMesh2D(meshes["guiStar"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2D::Translate(hexaRadius * 0.7f, 0);
    RenderMesh2D(meshes["guiStar"], shaders["VertexColor"], modelMatrix);

    // purple romb + price
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(guiSquareCentersX[3], guiSquareCentersY);
    RenderMesh2D(meshes["purpleRomb"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(guiSquareCentersX[3] - squareSize * 0.3f, guiSquareCentersY - squareSize * 0.5f - hexaRadius * 0.4f);
    RenderMesh2D(meshes["guiStar"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2D::Translate(hexaRadius * 0.7f, 0);
    RenderMesh2D(meshes["guiStar"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2D::Translate(hexaRadius * 0.7f, 0);
    RenderMesh2D(meshes["guiStar"], shaders["VertexColor"], modelMatrix);


    /*
        ROMB + STAR VS HEXAGON ANIMATION !!!
    */
    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsedSecondsHexa = currentTime - lastSpawnTimeHexa;
    std::chrono::duration<float> elapsedSecondsStar = currentTime - lastSpawnTimeStar;
    
    // generate random hexagons
    if (elapsedSecondsHexa.count() >= std::rand() % (6 - 4 + 1) + 4) {
        Hexagon newHexa(resx, squareCentersY[std::rand() % (2 - 0 + 1) + 0]);

        // 0 = Orange, 1 = Blue, 2 = Yellow, 3 = Purple
        newHexa.color = std::rand() % (3 - 0 + 1) + 0;

        switch (newHexa.color) {
            case 0: newHexa.mesh = meshes["orangeExtHexa"]; break;
            case 1: newHexa.mesh = meshes["blueExtHexa"]; break;
            case 2: newHexa.mesh = meshes["yellowExtHexa"]; break;
            case 3: newHexa.mesh = meshes["purpleExtHexa"]; break;
        }
        newHexa.intMesh = meshes["intHexa"];

        spawnedHexas.push_back(newHexa);
        lastSpawnTimeHexa = currentTime;
    }

    // render all spawned the hexagons
    for (auto it = spawnedHexas.begin(); it != spawnedHexas.end();) {
        auto& hexa = *it;

        if (hexa.life) {
            if (hexa.currentX <= healthRectangleLeftBottom[0] + rectangleWidth + hexaRadius) {
                // still alive & won => despawn hexa without animation, -1 life
                lives--;
                it = spawnedHexas.erase(it);
            }
            else {
                // still alive & hasnt won yet => keep moving hexa
                hexa.Update(deltaTimeSeconds);
                RenderMesh2D(hexa.mesh, shaders["VertexColor"], hexa.Spawn(hexa.spawnX, hexa.spawnY));
                RenderMesh2D(hexa.intMesh, shaders["VertexColor"], hexa.Spawn(hexa.spawnX, hexa.spawnY));
                ++it;
            }
        }
        else {
            // destroyed => start despawning hexa & remove it from spawned hexas
            despawningHexas.push_back(hexa);
            it = spawnedHexas.erase(it);
        }
    }

    // run despawning animation on all despawning hexagons
    for (auto it = despawningHexas.begin(); it != despawningHexas.end();) {
        auto& hexa = *it;
        hexa.scaleX -= deltaTimeSeconds;
        hexa.scaleY -= deltaTimeSeconds;
        if (hexa.scaleX > 0 && hexa.scaleY > 0) {
            RenderMesh2D(hexa.mesh, shaders["VertexColor"], hexa.Spawn(hexa.spawnX, hexa.spawnY));
            RenderMesh2D(hexa.intMesh, shaders["VertexColor"], hexa.Spawn(hexa.spawnX, hexa.spawnY));
            ++it;
        }
        else {
            // despawn animation finished => remove hexa from vector
            it = despawningHexas.erase(it);
        }
    }

    // render all spawned rombs
    for (auto r : spawnedRombs) {
        RenderMesh(r.mesh, shaders["VertexColor"], r.Spawn(r.currentX, r.currentY));
    }

    // run despawning animation for all despawning rombs
    for (auto rit = despawningRombs.begin(); rit != despawningRombs.end();) {
        auto& romb = *rit;
        romb.scaleX -= deltaTimeSeconds;
        romb.scaleY -= deltaTimeSeconds;
        if (romb.scaleX > 0 && romb.scaleY > 0) {
            // animation still going
            RenderMesh2D(romb.mesh, shaders["VertexColor"], romb.Spawn(romb.currentX, romb.currentY));
            ++ rit;
        }
        else {
            // despawn animation finished => remove romb from vector
            rit = despawningRombs.erase(rit);
        }
    }


    for (auto rit = spawnedRombs.begin(); rit != spawnedRombs.end();) {
        bool skip = false;
        // check if the romb has been touched by any enemy
        for (auto hit = spawnedHexas.begin(); !skip && hit != spawnedHexas.end(); ++hit) {
            if (hit->currentY == rit->currentY && (hit->currentX - hit->radius <= rit->currentX + rit->radius)) {
                // free the grid square that the romb was on
                gridSquares[rit->gridSquareNumber].free = true;

                // add romb to despawning rombs and remove it from spawned vector
                despawningRombs.push_back(*rit);
                rit = spawnedRombs.erase(rit);
                skip = true;
            }
        }

        if (skip) {
            // dont check for current romb targets before checking if it has been touched
            continue;
        }

        // check if the romb has any enemy to target
        for (auto hit = spawnedHexas.begin(); hit != spawnedHexas.end(); ++ hit) {
            if (hit->currentY == rit->currentY && hit->color == rit->color) {
                if (rit->fireing == false) {
                    Star newRombProjectile(rit->currentX, rit->currentY);

                    switch (rit->color) {
                    case 0: newRombProjectile.mesh = meshes["orangeStar"]; break;
                    case 1: newRombProjectile.mesh = meshes["blueStar"]; break;
                    case 2: newRombProjectile.mesh = meshes["yellowStar"]; break;
                    case 3: newRombProjectile.mesh = meshes["purpleStar"]; break;
                    }

                    rit->projectile = newRombProjectile;
                    rit->fireing = true;
                }
                else {
                    if (rit->projectile.currentX + rit->projectile.radius <= hit->currentX - hit->radius) {
                        // projectile hasnt touched the target yet
                        rit->projectile.Update(deltaTimeSeconds);
                        RenderMesh2D(rit->projectile.mesh, shaders["VertexColor"], rit->projectile.Spawn(rit->currentX, rit->currentY));
                    }
                    else {
                        hit->life--;

                        // determining when to switch fireing for the romb/
                        if (rit->projectile.currentX + rit->projectile.radius <= hit->currentX - hit->radius) {
                            // if it launched a projectile that didnt hit anything yet => keep it going
                            rit->fireing = true;
                        }
                        else {
                            // projectile did hit an enemy => switch fireing to off (do that the projectile is not rendered anymore)
                            rit->fireing = false;
                        }
                    }
                }
            }
        }

        if (rit->fireing == true) {
            // if fireing is still on here it means the target died before the projectile could reach it => keep it going
            rit->projectile.Update(deltaTimeSeconds);
            RenderMesh2D(rit->projectile.mesh, shaders["VertexColor"], rit->projectile.Spawn(rit->currentX, rit->currentY));
            if (rit->projectile.currentX >= resx) {
                // projectile reached end of logical space without hitting anything => switch fireing to off (do that the projectile is not rendered anymore)
                rit->fireing = false;
            }
        }

        ++ rit;
    }

    // generate & render the resource stars that get spawned (so you collect them)
    int smin = 6, smax = 8;
    int nmin = 1, nmax = 3;
    int s = std::rand() % (smax - smin + 1) + smin;
    int n = std::rand() % (nmax - nmin + 1) + nmin;

    // generate random gui stars at random times, every (8 - 12) seconds
    if (elapsedSecondsStar.count() >= s) {
        // spawn a random number of stars, (1 - 2 - 3) stars at a time
        for (int i = 0; i < n; i++) {
            Star newStar;
            newStar.mesh = meshes["spawnedGuiStar"];
            newStar.modelMatrix = newStar.GuiSpawn(resx, resy);
            spawnedGuiStars.push_back(newStar);
            lastSpawnTimeStar = currentTime;
        }
    }

    // render all currently not picked up stars
    for (auto& star : spawnedGuiStars) {
        RenderMesh2D(star.mesh, shaders["VertexColor"], star.modelMatrix);
    }

    // render drag&drop romb that you currently hold with lmb
    if (selecting) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(selectingX, selectingY);

        switch (selectedGuiSquare.colorOfRomb) {
        case 0: RenderMesh2D(meshes["orangeRomb"], shaders["VertexColor"], modelMatrix); break;
        case 1: RenderMesh2D(meshes["blueRomb"], shaders["VertexColor"], modelMatrix); break;
        case 2: RenderMesh2D(meshes["yellowRomb"], shaders["VertexColor"], modelMatrix); break;
        case 3: RenderMesh2D(meshes["purpleRomb"], shaders["VertexColor"], modelMatrix); break;
        }
    }
}


void Tema1::OnWindowResize(int width, int height)
{
    cout << "resizing to (" << width << "x" << height << ")\n";
    
    float sx, sy, tx, ty;
    sx = (float) resx / width;
    sy = (float) resy / height;

    visMatrix = transform2D::Scale(sx, sy);
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // translate the mouse coords to the window res
    glm::vec3 mouse_coord = glm::vec3(mouseX, mouseY, 1);
    glm::vec3 mouse_tr_coord = visMatrix * mouse_coord;
    mouseX = mouse_tr_coord[0];
    mouseY = mouse_tr_coord[1];

    if (selecting) {
        // cout << "!!! detecting mouse movement (while holding left button)" << endl;
        selectingX = mouseX;
        selectingY = resy - mouseY;
    }
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // translate the mouse coords to the window res
    glm::vec3 mouse_coord = glm::vec3(mouseX, mouseY, 1);
    glm::vec3 mouse_tr_coord = visMatrix * mouse_coord;
    mouseX = mouse_tr_coord[0];
    mouseY = mouse_tr_coord[1];

    // LMB press actions - select a romb to place or collect a resource star
    if (button == GLFW_MOUSE_BUTTON_2) {
        // cout << "mouse: X = " << mouseX << ", Y = " << resy - mouseY << endl;

        // 1. check if I clicked on a GUI Square with a romb in it
        for (auto sit = blackContourSquares.begin(); sit != blackContourSquares.end(); ++sit) {
            auto& square = *sit;

            if (mouseX >= sit->translateX && mouseX <= (sit->translateX + squareSize) &&
                (resy - mouseY) >= sit->translateY && (resy - mouseY) <= (sit->translateY + squareSize)) {

                // check if i have enough resources to get the romb
                if (square.priceOfRomb <= resources) {
                    // cout << "!!! detecting lmb selection on GUI square X = " << sit->translateX << ", Y = " << sit->translateY << endl;
                    selectedGuiSquare = square;
                    selecting = true;
                    selectingX = mouseX;
                    selectingY = resy - mouseY;
                    break;
                }
                else {
                    // not enough resources => do nothing
                    cout << "*********** n ai bani saracio ***********" << endl;
                }
            }
        }

        // 2. check if I clicked on a GUI Resource Star that I can collect
        for (auto starit = spawnedGuiStars.begin(); starit != spawnedGuiStars.end();) {
            if (mouseX >= (starit->spawnX - starit->radius) && mouseX <= (starit->spawnX + starit->radius) &&
                (resy - mouseY) >= (starit->spawnY - starit->radius) && (resy - mouseY) <= (starit->spawnY + starit->radius)) {

                resources++;
                // remove the star that I collected from rendering vector
                starit = spawnedGuiStars.erase(starit);
                break;
            }
            else {
                starit++;
            }
        }
    }

    // RMB press action - despawn one of the rombs
    if (button == GLFW_MOUSE_BUTTON_3) {
        // cout << "mouse: X = " << mouseX << ", Y = " << resy - mouseY << endl;

        int i = 0;
        bool found = false;
        // check if i clicked on a grid square with a romb in it
        for (auto sit = gridSquares.begin(); sit != gridSquares.end() && !found; sit++) {
            if (mouseX >= (sit->translateX - sit->length) && mouseX <= (sit->translateX + sit->length) &&
                (resy - mouseY) >= (sit->translateY - sit->length) && (resy - mouseY) <= (sit->translateY + sit->length) &&
                !sit->free) {
                
                // cout << "!!! detecting rmb selection on grid square X = " << sit->translateX << ", Y = " << sit->translateY << endl;
                // find the romb and kill it (with despawn animation)
                for (auto rit = spawnedRombs.begin(); rit != spawnedRombs.end(); rit++) {
                    auto& romb = *rit;
                    if (rit->currentX == squareCentersX[i / 3] && rit->currentY == squareCentersY[i % 3]) {
                        despawningRombs.push_back(romb);
                        spawnedRombs.erase(rit);
                        found = true;
                        // also free the grid square that it was on
                        sit->free = true;
                        break;
                    }
                }
            }
            i++;
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // translate the mouse coords to the window res
    glm::vec3 mouse_coord = glm::vec3(mouseX, mouseY, 1);
    glm::vec3 mouse_tr_coord = visMatrix * mouse_coord;
    mouseX = mouse_tr_coord[0];
    mouseY = mouse_tr_coord[1];

    // LMB release action - spawn a romb if it is placed right on the grid
    if (button == GLFW_MOUSE_BUTTON_RIGHT && selecting == true) {
        // cout << "mouse: X = " << mouseX << ", Y = " << resy - mouseY << endl;

        int i = 0;
        // check if I let go of the LMB over a grid square that doesnt have a romb in it yet
        for (auto sit = gridSquares.begin(); sit != gridSquares.end(); sit++) {
            if (mouseX >= (sit->translateX) && mouseX <= (sit->translateX + sit->length) &&
                (resy - mouseY) >= (sit->translateY) && (resy - mouseY) <= (sit->translateY + sit->length)) {
                
                // cout << "!!! detecting you were on grid square X = " << sit->translateX << ", Y = " << sit->translateY << " when letting go of lmb" << endl;
                if (sit->free) {
                    Romb newRomb;

                    newRomb.gridSquareNumber = i;
                    newRomb.currentX = squareCentersX[i / 3];
                    newRomb.currentY = squareCentersY[i % 3];

                    newRomb.color = selectedGuiSquare.colorOfRomb;

                    switch (newRomb.color) {
                        case 0: newRomb.mesh = meshes["orangeRomb"]; break;
                        case 1: newRomb.mesh = meshes["blueRomb"]; break;
                        case 2: newRomb.mesh = meshes["yellowRomb"]; break;
                        case 3: newRomb.mesh = meshes["purpleRomb"];  break;
                    }

                    spawnedRombs.push_back(newRomb);

                    resources -= selectedGuiSquare.priceOfRomb;

                    // occupy the grid square
                    sit->free = false;
                }
            }
            i++;
        }

        selecting = false;
    }
}




void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void Tema1::FrameEnd() {}

void Tema1::OnInputUpdate(float deltaTime, int mods) {}

void Tema1::OnKeyPress(int key, int mods) {}

void Tema1::OnKeyRelease(int key, int mods) {}
