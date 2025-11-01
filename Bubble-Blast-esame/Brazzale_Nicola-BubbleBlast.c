#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>    //valori booleani
#include <time.h>     //funzione di random

//Costanti
#define RIGHE 5         //numero di righe della matrice di gioco
#define COLONNE 6       //numero di colonne della matrice di gioco
#define MAX_BOLLE 9     //numero di bolle massime con cui popolare la matrice
#define file_r "risultati.txt"//creazione file di testo per i risultati

        //Definizione prototipi
//Matrici
    void crGri(int griglia[][COLONNE]);
    void stGri(int griglia[][COLONNE]);

//Grafica
    void spazio();
    void regole();
    void pulizia();
    int difficolta();
    int stDiff(int difficolta());
    int mosseBonus(int m);

//Mosse da battere
    int mossebattere(int griglia[][COLONNE], int r, int c);

    void inserisciBolle(int griglia[][COLONNE]);
    void copiaGri(void * grigliaDestinazione, void * grigliaOrigine);
    bool vittoria(int griglia[][COLONNE]);

//gestione esplosione
    void esplosione(int griglia[RIGHE][COLONNE], int rig, int col);
    void esposioneSx(int griglia[RIGHE][COLONNE], int r, int c);
    void esplosioneGiu(int griglia[RIGHE][COLONNE], int r, int c);
    void esplosioneDx(int griglia[RIGHE][COLONNE], int r, int c);
    void esplosioneSu(int griglia[RIGHE][COLONNE], int r, int c);



int main()
{
    //definizione variabili
    int griglia[RIGHE][COLONNE];        //matrice di partenza
    int grigliaMod[RIGHE][COLONNE];     //aggiornamento della matrice dopo ogni mossa
    bool end;                           //valore per vincere
    int rigaIns;                          //riga inserita dal giocatore
    int colIns;                          //colonna inserita dal giocatore
    char continua = "";                 //variabile per far continuare il gioco
    int livello = 1;                    //contatore di livelli completati inizializzato a 1
    bool fineGioco;                     //condizione per far finire il gioco
    bool sconfitta;                     //valore per perdere per esaurimento mosse
    FILE *file_risultati;
    char risultati[200];
    bool vuoto;
    char nome_giocatore[25];
    time_t giorno;
    time(&giorno);
    int bonus = 0;                          //variabile per impostare la difficolta'
    int contMB = 0;                   //contatore mosse da battere
    int contMG = 0;                   //contatore mosse effettuate dal giocatore
    struct tm* gioOut;
    gioOut = localtime(&giorno);
    char giornoMeseAnno[12];
    strftime(giornoMeseAnno,12,"%d-%m-%Y",gioOut);
    time_t ora;
    time(&ora);
    struct tm* oraOut;
    oraOut = localtime(&ora);
    char oraMinSec[12];
    strftime(oraMinSec, 12, "%H:%M:%S", oraOut);


    regole();   //stampa regole gioco
    impostaNome(nome_giocatore);
    bonus = difficolta();    //seleziona la difficolta' del gioco
    pulisci();
    attendi();

    do
    {
        contMB=0;
        contMG=0;
        end = false;
        fineGioco = false;
        creaGri(griglia);
        inserisciBolle(griglia);
        pulisci();
        printf("Livello: %d", livello);
        spazio();
        copiaGri(grigliaMod, griglia); //copia la griglia di gioco nella griglia che verra' modificata
        stampaGri(griglia); //stampa la griglia di gioco
        contMB = calcoloMosse(griglia, RIGHE, COLONNE);  //contatore delle mosse da battere
        printf("Mosse calcolate dall'algoritmo: %d\t\t", contMB); //stampa mosse da battere
        contMB += mosseBonus(bonus); //aggiunta mosse bonus
        printf("Mosse disponibili all'utente: %d (+%d)", contMB, mosseBonus(bonus)); //stampa mosse disponibili
        spazio();
        do
        {
             
            printf("Inserisci la riga della bolla da colpire! (Ricorda: la prima riga e' la numero zero) --> ");
            scanf("%d", &rigaIns);
            if (rigaIns >= 0 && rigaIns < RIGHE){}else{ //controllo input riga
                printf("Hai inserito un numero fuori dalla griglia, hai sprecato la tua mossa\n");
            }

            
            printf("Inserisci la colonna della bolla da colpire! (Ricorda: la prima colonna e' la numero zero) --> ");
            scanf("%d", &colIns);
            if (colIns >= 0 && colIns < COLONNE){}else{ //controllo input colonna
                printf("Hai inserito un numero fuori dalla griglia, hai sprecato la tua mossa\n");
            }
            spazio();
            //stampa valori inseriti
            printf("Hai inserito: %d e %d\n\n", rigaIns, colIns);
            contMG++;

            if (grigliaMod[rigaIns][colIns] == 1){ 
                printf("Hai colpito una bolla in procinto di esplodere!\nBOOOM!\n");
                grigliaMod[rigaIns][colIns]--;
                esplosione(grigliaMod, rigaIns, colIns);
            } else if (grigliaMod[rigaIns][colIns] == 2){ 
                printf("Hai colpito una bolla gonfia a meta'! Ora e' in procinto di esplodere.\n");
                grigliaMod[rigaIns][colIns]--;
            } else if (grigliaMod[rigaIns][colIns] == 3){  
                printf("Hai colpito una bolla sgonfia! Ora è gonfia a meta'.\n");
                grigliaMod[rigaIns][colIns]--;
            }else{printf("Non hai colpito nessuna bolla!\n");} 
            
           
            stampaGri(grigliaMod);
            spazio();
            //stampa contatori mosse
            printf("Il tuo numero di mosse: %d\t Mosse da battere: %d\t Mosse rimanenti: %d\n\n", contMG, contMB, contMB-contMG);

            end = vittoria(grigliaMod);

        } while (end != true && contMB-contMG > 0);
        if (end == true)
        {
                livello++;
            do
            {
                pulisci();
                printf("Hai terminato il livello %d\n", livello-1);
                printf("Vuoi continuare? Premi y per continuare oppure n per terminare. ");
                scanf("%c", &continua);
                if (continua == 'y')
                {attendi(); fineGioco = false; break;}
                else if (continua == 'n')
                {fineGioco = true;}
            }while (fineGioco != true);
        } else {printf("Hai perso!\n"); fineGioco = true; sconfitta = true;}
    } while (fineGioco != true);



    file_risultati = fopen(file_r, "a+");
    if (file_risultati == NULL)
        {
        printf("Impossibile aprire il file!\n");
        exit(1);
        } else
        {
            fprintf(file_risultati, "Utente: %s\tData: %s - %s\t Livelli completati: %d\n", nome_giocatore, giornoMeseAnno, oraMinSec, livello-1);
        }
    fclose(file_risultati);
}


    void regole(){
        //Funzione che stampa le regole del gioco
        printf("\tB U B B L E\t B L A S T\n");
        printf("Lo scopo del gioco e' far esplodere tutte le bolle prima di esaurire i tentativi.\n");
        printf("Ci sono tre tipi di bolle:\n \t 1) Bolla in procinto di esplodere;\n\t 2) Bolla gonfia a meta';\n\t 3) Bolla sgonfia.\n");
        printf("Quando colpirai una bolla sgonfia (3) la gonfierai a meta',\nquando colpirai una bolla gonfia a meta'(2) la renderai in procinto di esplodere \nmentre quando colpirai una bolla in procinto di esplodere (1) la farai esplodere.\n");
        printf("Facendo esplodere una bolla in procinto di esplodere creerai un'esplosione che si propaghera' nelle quattro direzioni lungo tutta la griglia!\n");
        printf("!!!Le matrici partono da zero quindi se vuoi selezionare la prima colonna o la prima riga dovrai inserire 0 e non 1!!!\n");
        printf("Vinci quando fai esplodere tutte le bolle, perdi se non riesci nei tentativi possibili.\n\nINIZIAMO!\n\n");
}

    void spazio(){
    //ordina la console
    int i;
    printf("\n");
    for (i=0; i<30; i++)
        {
        printf("***");
    }
    printf("\n");
}

    void attendi(){
        //funzione che dopo aver pulito lo schermo fa comparire un messaggio di attesa
        printf("\nAttendi il carimento della griglia...\n\n");
}

    void pulisci(){
    //funzione che pulisce lo schermo per un maggior ordine
     system("@cls||clear");
}

    void creaGri(int griglia[RIGHE][COLONNE]){
    //funzione che crea la griglia inizializzata a zero
    int i, j;
    for (i=0; i<RIGHE; i++){
        for (j=0; j<COLONNE; j++){
            griglia[i][j] = 0;
        }
    }
}

    void stampaGri(int griglia[RIGHE][COLONNE]){
    //funzione che stampa la griglia
    int i, j;
    printf("\n");
    for (i=0; i<RIGHE; i++){
        for (j=0; j<COLONNE; j++){
            printf("\t");
            printf("%d", griglia[i][j]);
        }
        printf("\n\n");
    }
}

    void inserisciBolle(int griglia[RIGHE][COLONNE]){
        //funzione che inserisce le bolle 
        int bolle_max = MAX_BOLLE; //contatore che indica il numero massimo di bolle da inserire nella matrice
        do
        {
            int insRig; 
            int insCol; 
            int bolla;
            srand((unsigned)time(NULL));
            insRig = rand() % RIGHE;
            srand((unsigned)time(NULL));
            insCol = rand () % COLONNE;
            srand((unsigned)time(NULL));
            bolla = rand()%3+1;
            if (griglia[insRig][insCol] == 0){ //controllo che se il valore in una data posizione e' lo cambia con un stato random fino a raggiungere il numero massimo di bolle
            griglia[insRig][insCol] = bolla; 
            bolle_max--; 
            }
        } while (bolle_max != 0); //ripeti finche' le bolle disponibili non sono esaurite
}

    void copiaGri(void * grigliaDestinazione, void * grigliaOrigine){
        //funzione che alla fine di ogni mossa copia la matrice
        
        memcpy(grigliaDestinazione, grigliaOrigine, RIGHE*COLONNE*sizeof(int));//funzione che copia n caratteri dall'area di memoria src all'area di memoria dest 
      
}

    int difficolta(){
    //funzione che regola la difficolta' del gioco diminuendo le mosse a disposizione
    int d;
    printf("Scegli la difficolta' di gioco:\n");
    do
        {
        printf("Inserisci un numero da 1 a 3: \n\t 1) Facile; \n\t 2) Medio; \n\t 3) Difficile.\n");
        scanf("%d", &d);
        } while (d <= 0 || d>4);
        spazio();
        return d;
}

    int calcoloMosse(int griglia[RIGHE][COLONNE], int r, int c){ //funzione che risolve automaticamente la griglia e calcola quante mosse ci vogliono per vincere
        int mosse = 0; //contatore che contiene le mosse da battere
        bool fine;    //operatore usato come condizione nel ciclo per sapere quando fermarsi

        do
            {
                fine = true;
                for (r=0; r<RIGHE; r++){
                    for (c=0; c<COLONNE; c++)
                        if (griglia[r][c] == 1){
                            griglia[r][c]--;
                            esplosione(griglia, r, c);
                            mosse++;
                            fine = false;
                        }
                        else if (griglia[r][c] == 2 || griglia[r][c] == 3){
                            griglia[r][c]--;
                            mosse++;
                            fine = false;
                        }
            }
     }while (fine == false);
        return mosse;
 }

    bool vittoria(int griglia[RIGHE][COLONNE]){    
        int i, j;
        bool trovato = true; 
        for (i=0; i<RIGHE; i++){
            for(j=0; j<COLONNE; j++){
                if (griglia[i][j] != 0){trovato = false;} //cerca un qualsiasi elemento maggiore di zero quindi ritorna falso
            }
        }
        return trovato;
}

    int mosseBonus(int m){
        //aggiunge le mosse a seconda del valore di difficolta 
        int aggiungi;
        switch (m)
        {
            case 1 : aggiungi = 3; break; 
            case 2 : aggiungi = 2; break;  
            case 3 : aggiungi = 0; break;  
        }
        return aggiungi;
}

    void esplosione(int griglia[RIGHE][COLONNE], int rig, int col){
        //funzione che innesca l'esplosione sulle 4 coordinate rispetto alla cella selezionata
        esplosioneSu(griglia, rig, col);
        esplosioneGiu(griglia, rig, col);
        esplosioneDx(griglia, rig, col);
        esplosioneSx(griglia, rig, col);

}

    void impostaNome(char nome[30]){
        printf("Inserisci nome giocatore: ");
        scanf("%s", nome);
        printf("Ciao %s!\n", nome);
        return nome[30];
}

    void esplosioneSu (int griglia[RIGHE][COLONNE], int rig, int col){
    bool bordoSu = false;

    if (rig > 0){
        rig--;
    do
        {
        if (griglia[rig][col] != 0) {bordoSu = true;
        if (griglia[rig][col] == 1) {griglia[rig][col]--; esplosione(griglia, rig++, col);}
        else {griglia[rig][col]--;}
        }else rig--;
        if (rig < 0)
           { bordoSu = true;}
        } while (bordoSu != true);
     } else return;
}

    void esplosioneGiu (int griglia[RIGHE][COLONNE], int rig, int col){
    bool bordoGiu = false;

    if (rig < RIGHE-1){
        rig++;
    do
        {
        if (griglia[rig][col] != 0){bordoGiu = true;
        if (griglia[rig][col] == 1){griglia[rig][col]--; esplosione(griglia, rig--, col);}
        else {griglia[rig][col]--;}
        }else rig++;
        if (rig == RIGHE){ bordoGiu = true;}
        } while (bordoGiu != true);
     }else  return;
}

    void esplosioneDx (int griglia[RIGHE][COLONNE], int rig, int col){
    bool bordoDestra = false;

    if (rig < COLONNE-1){   
        col++;              
    do
        {
        if (griglia[rig][col] != 0) {bordoDestra = true; 
        if (griglia[rig][col] == 1) {griglia[rig][col]--; esplosione(griglia, rig, col--);} 
        else {griglia[rig][col]--;} 
        }else col++; //se non e' ne 1 ne 2 ne 3 aumenta l'indice di colonna per vedere la prossima colonna
        if (col == COLONNE)    { bordoDestra = true;} //se la cella dopo aver aumentato il contatore e' sul bordo destro esce dal ciclo
        } while (bordoDestra != true); 
     }else  return; 
}

    void esplosioneSx (int griglia[RIGHE][COLONNE], int rig, int col){
    bool bordoSinistra = false;

     if (rig > 0){
        col--;
    do
        {
        if (griglia[rig][col] != 0) {bordoSinistra = true;
        if (griglia[rig][col] == 1) {griglia[rig][col]--; esplosione(griglia, rig, col++);}
        else {griglia[rig][col]--;}
       } else col--;
        if (col < 0)    {bordoSinistra = true;}
        } while (bordoSinistra != true);
     }else  return;
}


