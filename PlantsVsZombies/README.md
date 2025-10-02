# Plants vs Zombies (EGC – Elemente de Grafică pe Calculator)

Proiect realizat pentru cursul **EGC – Elemente de Grafică pe Calculator**, implementat în **C++ și OpenGL**.  
Scopul proiectului a fost înțelegerea și aplicarea conceptelor de randare 2D, detecție a coliziunilor și interacțiune utilizator într-un joc simplificat.  

## Descriere
- **Scena staționară**: grid, HUD, health bar, vieți.  
- **Elemente dinamice**:
  - Inamici generați aleator (hexagoane).  
  - Elemente de apărare plasate de jucător prin comenzi (romburi).  
  - Proiectile generate dacă apărarea se potrivește cu inamicul de pe linie (culoare + poziție).  
- **Randare**:
  - Elemente staționare – afișate permanent.  
  - Elemente dinamice – randate conform logicii jocului.  
- **Coliziuni**:
  - Detectate prin păstrarea coordonatelor elementelor dinamice.  
  - Se verifică impactul dintre proiectile și inamici, sau dintre inamici și jucător.  

## Funcționalități cheie  
- Plasarea manuală a elementelor defensive.  
- Generare aleatorie a inamicilor.  
- Proiectile dinamice condiționate de potrivirea culorilor.  
- Sistem de coliziuni între obiectele jocului.  
- Gestionarea vieților și a stării jucătorului.  

## Tehnologii
- **OpenGL** pentru randare 2D.  
- C++ pentru logica jocului.  

## Cum rulezi proiectul
1. Compilează codul sursă C++ cu suport pentru OpenGL (GLUT/GLFW, GLEW).  
2. Rulează executabilul generat.  
3. Controlează jocul cu tastele definite (plasare apărare, acțiuni HUD).  

## Concluzie
Proiectul demonstrează integrarea conceptelor de **randare 2D, detecție de coliziuni și interacțiune utilizator** într-un joc simplificat realizat cu OpenGL.  
