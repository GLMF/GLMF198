/************************************************
Programme de Master Mind
Christophe BORELLY - 2014-10-08 - IUT de Beziers
************************************************/
#ifndef LIB_MASTER_MIND
#define LIB_MASTER_MIND

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "lib-gtk-utils.h"

#define PACKAGE    "cb-mastermind"
#define SHARE_DIR   "/usr/share"
#define ICONS_DIR   SHARE_DIR "/icons"
#define LOCALE_DIR  SHARE_DIR "/locale"

#define NB_COLORS   9
#define BTN_SIZE    50
#define LBL_SIZE    100
#define TIMEOUT     5.0 // 2 min
#define PBAR_NAME   "pbar"
#define VBOX_NAME   "vbox"

#define SPAN(x,y,msg) \
        "<span font=\"" #x "\" foreground=\"" #y "\">" #msg "</span>"

typedef struct {
  int size;
  int *data;
} Array;

#ifdef LIB_MASTER_MIND_SRC
  gboolean debug=FALSE;
  int nbBienPlace=0,nbMalPlace=0,nbCoups=0,nbCases=4;
  GTimer *timer=NULL;
  gdouble timeout=TIMEOUT;
  Array *code=NULL;
  Array *combinaison=NULL;
  const char *LABELS[NB_COLORS]={"0","1","2","3","4","5","6","7","8"};
#else
  extern gboolean debug;
  extern int nbBienPlace,nbMalPlace,nbCoups,nbCases;
  extern GTimer *timer;
  extern gdouble timeout;
  extern Array *code;
  extern Array *combinaison;
  extern const char *LABELS[NB_COLORS];
#endif

void printArray(Array *tab);
Array *newArray(int size);
void freeArray(Array **array);
void newCode();
void newGame(GtkWidget *b,gpointer data);
void addNewCombinaison(GtkWidget *vbox);
void clickColor(GtkWidget *b,gpointer data);
void disableButtons(GtkWidget *hbox);
void clickOK(GtkWidget *b,gpointer data);
gboolean checkCombinaison(); // Voir lib-mastermind-check.c
gboolean afficheResultat(GtkWidget *hbox);
//void fin();
void afficheGagne(GtkWidget *vbox);
void affichePerdu(GtkWidget *vbox);

#endif
