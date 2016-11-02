#include "midi.c"
#include "time.h"
#define noireblues 120
#define longueur   200

void ecrire_piste1(FILE *fichier) {   
    unsigned long marque = MIDI_ecrire_en_tete_piste(fichier) ;
    MIDI_tempo(fichier, 1000000) ;   
    MIDI_fin_de_la_piste(fichier) ;
    ecrire_taille_finale_piste(fichier, marque) ;    
}

void ecrire_piste2(FILE *fichier) {
    double p, delta ;
    
    unsigned long marque = MIDI_ecrire_en_tete_piste(fichier) ;
    MIDI_Control_Change(fichier, 0, reverb, 64) ;
    MIDI_Control_Change(fichier, 0, 1, 40) ;    // modulation
    MIDI_Program_Change(fichier, 0, 30) ; 

    const int nb_notes = 6 ;
    unsigned char gammes[128] = {0, 3, 5, 6, 7, 10} ;  // Blues
    int jmax = nb_notes - 1 ;
    int octave = 1 ;
    bool continuer = true ;
    do {
        for(int j=0 ; j <= nb_notes-1 ; j = j+1){    
            if (gammes[j] + octave*12 <= 127) {
                jmax = octave*nb_notes + j ;
                gammes[jmax] = gammes[j] + octave*12 ;
            }
            else continuer = false ;
        }
        octave = octave + 1 ;
    } while (continuer) ;
    
    srand(time(NULL));
    unsigned char duree = noireblues ;
    const unsigned char tonique = 24 ;  // do
    unsigned char note = tonique ;
    for(int i=1 ; i <= longueur ; i = i+1){
        Note_unique_avec_duree(fichier, 0, gammes[note], 40, duree) ;    
        
        p = rand() / ((double)RAND_MAX) ;
        delta = ((gammes[note] - gammes[tonique]) / 12.0) * 0.45 ;
        if (p >= 0.55+delta) {
            if (note < jmax) note = note + 1 ;
        }
        else if (p >= 0.1) {
            if (note > 0) note = note - 1 ;
        }
        
        p = rand() / ((double)RAND_MAX) ;
        if (p >= 0.75) duree = noireblues ;
        else duree = noireblues / 4 ;
    }
    MIDI_fin_de_la_piste(fichier) ;
    ecrire_taille_finale_piste(fichier, marque) ;    
}

void ecrire_piste_percu(FILE *fichier) {
    unsigned long marque = MIDI_ecrire_en_tete_piste(fichier) ;
    MIDI_Control_Change(fichier, percu, reverb, 64) ;

    for(int i=1 ; i <= longueur*3 ; i = i+1){
        MIDI_delta_time(fichier, 0) ;
        MIDI_Note(ON, fichier, percu, 42, 80) ; // Charleston
        if (i%6 == 4) {                         // Caisse claire
            MIDI_delta_time(fichier, 0) ;
            MIDI_Note(OFF, fichier, percu, 38, 92) ;
            MIDI_delta_time(fichier, 0) ;
            MIDI_Note(ON, fichier, percu, 38, 92) ;
        }
        else if ((i%6 == 1) || (i%12 == 6)) {   // Grosse caisse
            MIDI_delta_time(fichier, 0) ;
            MIDI_Note(OFF, fichier, percu, 35, 127) ;
            MIDI_delta_time(fichier, 0) ;
            MIDI_Note(ON, fichier, percu, 35, 127) ;
        }
        MIDI_delta_time(fichier, noireblues / 3) ;      
        MIDI_Note(OFF, fichier, percu, 42, 64) ;
    }   
    MIDI_fin_de_la_piste(fichier) ;
    ecrire_taille_finale_piste(fichier, marque) ;    
}

int main(void) {
    FILE *fichier_midi = fopen("blues.mid", "wb") ;
    MIDI_ecrire_en_tete(fichier_midi, 1, 3, noireblues) ;
    ecrire_piste1(fichier_midi) ;
    ecrire_piste2(fichier_midi) ; 
    ecrire_piste_percu(fichier_midi) ;
    fclose(fichier_midi) ;  
}