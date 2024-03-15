În cadrul temei 1, am implementat un joc similar cu "Plants vs Zombies", dar adaptat pentru a conține o geometrie simplificata.
Pe scurt, am creat in functia de initializare elementele corespunzatoare scenei 'stationare', cum ar fi grid-ul, hud-ul, health bar-ul, vietile etc.
In functia de randare am in permanenta afisate elementele stationare, ale caror coordonate/dimensiuni nu se schimba in timp. Elementele dinamice sunt randate
dupa logica jocului, fie aleator: hexagoanele inamice, fie dupa o comanda a jucatorului (apasare de buton/tasta): romburile de defence. Elemente precum
proiectilele sunt randate dinamic, daca anumite criterii sunt indeplinite: jucatorul a plasat un romb de aceeasi culoare cu hexagonul inamic, pe una dintre linii.
Coliziunile sunt detectate prin prisma faptului ca retin, la orice moment de timp, coordonatele elementelor dinamice din scena, astfel imi pot da seama cu usurinta cand
un proiectil loveste un hexagon/cand un hexagon a ajuns sa 'loveasca' jucatorul.
