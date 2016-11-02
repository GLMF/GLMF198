// A partir des premières mesures du canon de Pachelbel

#include "midi.c"

void ecrire_piste1(FILE *fichier) {   
    unsigned long marque = MIDI_ecrire_en_tete_piste(fichier) ;
    MIDI_tempo(fichier, 1000000) ;   
    MIDI_fin_de_la_piste(fichier) ;
    ecrire_taille_finale_piste(fichier, marque) ;    
}

// Basse obstinée
void ecrire_piste_basse(FILE *fichier) {
    unsigned long marque = MIDI_ecrire_en_tete_piste(fichier) ;
    MIDI_Control_Change(fichier, 0, reverb, 64) ;
    MIDI_Program_Change(fichier, 0, 48) ;
    
    unsigned char basse[8] = {50, 45, 47, 42, 43, 38, 43, 45} ;   
    for(int j = 1 ; j <= 30 ; j = j + 1) {
        for(int i = 0 ; i < 8 ; i = i + 1){
            Note_unique_avec_duree(fichier, 0, basse[i], 64, noire) ;
        }
    }
    MIDI_fin_de_la_piste(fichier) ;
    ecrire_taille_finale_piste(fichier, marque) ;    
}

// Canon à trois voix
void ecrire_pistes_canon(FILE *fichier) {
    unsigned char instrument[17] = {40, 41, 42, 44, 45, 48, 49, 51, 52, 89, 90, 91, 92, 94, 95, 99, 100} ;
    unsigned char theme[16] = {78, 76, 74, 73, 71, 69, 71, 73, 74, 73, 71, 69, 67, 66, 67, 64} ;
    
    for(int piste = 3 ; piste <= 5 ; piste = piste + 1) {
        unsigned long marque = MIDI_ecrire_en_tete_piste(fichier) ;
        MIDI_Control_Change(fichier, piste, reverb, 64) ;    
        Note_unique_avec_duree(fichier, piste, 0, 0, 8*noire*(piste - 2)) ;   
        for(int j = 0 ; j < 15 ; j = j + 1) {
            MIDI_Program_Change(fichier, piste, instrument[(piste - 3) + j]) ;
            for(int i = 0 ; i < 16 ; i = i + 1){
                Note_unique_avec_duree(fichier, piste, theme[i], 64, noire) ;
            }
        }        
        MIDI_fin_de_la_piste(fichier) ;
        ecrire_taille_finale_piste(fichier, marque) ;           
    }
}

int main(void) {
    FILE *fichier_midi = fopen("canon.mid", "wb") ;
    MIDI_ecrire_en_tete(fichier_midi, 1, 5, noire) ;
    ecrire_piste1(fichier_midi) ;
    ecrire_piste_basse(fichier_midi) ; 
    ecrire_pistes_canon(fichier_midi) ; 
    fclose(fichier_midi) ;  
}