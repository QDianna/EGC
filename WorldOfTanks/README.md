# World of Tanks (EGC – Elemente de Grafică pe Calculator)

Proiect realizat pentru cursul **EGC – Elemente de Grafică pe Calculator**, implementat în **C++ și OpenGL**.  
Este o versiune simplificată, single-player, a jocului World of Tanks.  

## Descriere
- Scena 3D cu camera poziționată deasupra și în spatele tancului, orientată spre acesta  
- Control tanc: deplasare în toate direcțiile (WASD)  
- Control cameră: rotire liberă în jurul tancului (Right Mouse Button Hold)  
- Tancul este compus din mai multe componente randate separat (șenile, corp, turelă, tun), animate sincron  
- Turelă și tun independente: jucătorul poate roti turela și poate trage proiectile  

## Elemente ale scenei
- Clădiri generate aleator (poziție, dimensiuni, culori)  
- Inamici care se deplasează pe traiectorii predefinite  
- Posibilitatea de a trage în inamici și în clădiri  

## Detecția coliziunilor
- Coordonatele tuturor tancurilor și clădirilor sunt actualizate în timp real  
- Coliziunea este verificată prin intersectarea suprafeței proiectilului cu suprafața corpului unui tanc sau a unei clădiri  

## Tehnologii
- OpenGL pentru randare 3D  
- C++ pentru logica jocului și detecția coliziunilor  

## Cum rulezi proiectul
1. Compilează codul sursă C++ cu suport OpenGL (GLFW/GLUT, GLEW)  
2. Rulează executabilul generat  
3. Controlează tancul cu **WASD**, rotește camera cu **Right Mouse Button Hold**, trage cu tasta implementată pentru proiectile  

## Concluzie
Proiectul demonstrează utilizarea conceptelor de randare 3D, modelare pe componente, animații sincronizate și detecție a coliziunilor într-un joc simplificat realizat în OpenGL.  
