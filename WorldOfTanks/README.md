In cadrul acestei teme am implementat o versiune simplificata, single-player, a jocului World of Tanks.
Scena jocului reprezinta un cadru 3D, cu camera pozitionata putin deasupra si in spatele tancului, orientata spre acesta.
Tancul se poate misca in toate directiile (WASD); camera se poate misca de asemenea de jur imprejurul tancului (RMB HOLD).
Am creat tancul prin randarea tuturor componentelor sale, create separat: senile, corp, turela si tun, pe care le misc
in mod sincron pentru a reda ansamlul de obiecte. Aceasta compartimentizare a fost utila atat in crearea geometriei, cat si
pentru a avea anumite mecanici care sa corespunda partilor individuale ale tankului: turela + tunul se pot roti separat de corp
pentru ca jucatorul sa poata tinti, tunul poate "genera" proiectile.
Am randat cladiri distribuite aleator pe harta, de dimensiuni si culori, de asemenea, aleatoare; am creat si inamici care se misca
pe o traiectorie predefinita. Tancul jucatorului poate "trage" in tancurile inamice. Detectia coliziunii proiectulului cu inamicul
este realizata pastrand coordonatele tancurilor la fiecare moment de timp si calculand coordonatele marginii fiecarui corp al fiecarui tanc;
astfel stiu cand suprafata proiectilului intersecteaza suprafata corpului tancului inamic. Intr-un mod similar detectez si coliziunea proiectilului cu cladirile.
