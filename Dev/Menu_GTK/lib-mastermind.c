/************************************************
Programme de Master Mind
Christophe BORELLY - 2014-10-08 - IUT de Beziers
************************************************/
#define LIB_MASTER_MIND_SRC
#include "lib-mastermind.h"

///////////////////////////////////////////////////////////////////////////////
// Arrete le timer
///////////////////////////////////////////////////////////////////////////////
static void stopTimer() {
  if (timer!=NULL) {
    if (debug) printf("stopTimer(%p)...\n",timer);
    g_timer_destroy(timer);
  }
  timer=NULL;
}
///////////////////////////////////////////////////////////////////////////////
// Demarre le timer
///////////////////////////////////////////////////////////////////////////////
static void startTimer() {
  if (debug) printf("startTimer()...\n");
  stopTimer();
  timer=g_timer_new();
  if (debug) printf("Timer:%p\n",timer);
}
///////////////////////////////////////////////////////////////////////////////
static void finDePartie() {
  if (debug) printf("finDePartie()...\n");
  stopTimer();
  freeArray(&code);
  freeArray(&combinaison);
}
///////////////////////////////////////////////////////////////////////////////
// Mise a jour de la barre de progression
// Retourner FALSE pour arreter la mise à jour et TRUE pour continuer
///////////////////////////////////////////////////////////////////////////////
static gboolean progressUpdate(gpointer data) {
  GtkWidget *window=(GtkWidget *)data;
  GtkWidget *progressBar=findChildByName(window,PBAR_NAME);
  if (progressBar==NULL) return FALSE; // Stop - NO PROGRESS BAR
  if (timer==NULL) return FALSE; // Stop - timer has stop
  if (timeout==0.0) { // No time limits
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressBar),0.0);
    return FALSE;
  } else {
    gdouble val=gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(progressBar));
    gulong tmp;
    gdouble t=g_timer_elapsed(timer,&tmp);
    if (debug) printf("Time %.2f s (Reste %.2f s), Val : %.2f %%\n",
                      t,timeout-t,100*val);
    val=t/timeout;
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressBar),val);
    if (val>=1.0) { // Fin de la partie
      GtkWidget *vbox=findChildByName(window,VBOX_NAME);
      if (vbox!=NULL) {
        GList *liste=gtk_container_get_children(GTK_CONTAINER(vbox));
        GList *last=g_list_last(liste); // Dernier element
        if (last!=NULL) {
          GtkWidget *hbox=GTK_WIDGET(last->data);
          disableButtons(hbox);
          afficheResultat(hbox);
          affichePerdu(vbox);
        }
        g_list_free(liste);
      }
      return FALSE;
    }
  }
  return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
// Fonction d'affichage du code ou d'une combinaison
// Exemple :
//   printArray(code);
///////////////////////////////////////////////////////////////////////////////
void printArray(Array *array) {
  if (array!=NULL) {
    for(int i=0;i<array->size;i++) {
      if (i>0) printf(":");
      printf("%d",array->data[i]);
    }
    printf("\n");
  }
}
///////////////////////////////////////////////////////////////////////////////
// Allocation memoire pour un tableau
///////////////////////////////////////////////////////////////////////////////
Array *newArray(int size) {
  Array *array=NULL;
  if (size>0&&size<10) { // Limit size
    array=malloc(sizeof(Array));
    if (array==NULL) return NULL;
    array->size=size;
    array->data=malloc(size*sizeof(int));
  }
  //if (debug) printf("newArray(%d): %p...\n",size,array);
  return array;
}
///////////////////////////////////////////////////////////////////////////////
// Liberation memoire d'un tableau
///////////////////////////////////////////////////////////////////////////////
void freeArray(Array **array) {
  //if (debug) printf("freeArray(%p)...\n",*array);
  if (*array!=NULL) {
    free((*array)->data);
    free(*array);
    *array=NULL;
  }
}
///////////////////////////////////////////////////////////////////////////////
// Genere un nouveau code aleatoirement
///////////////////////////////////////////////////////////////////////////////
void newCode() {
  if (debug) printf("newCode()...\n");
  if (code==NULL) {
    code=newArray(nbCases);
    combinaison=newArray(nbCases);
  }
  if (code!=NULL&&combinaison!=NULL) {
    for(int i=0;i<code->size;i++) {
      combinaison->data[i]=0;          // Remise a zero de la combinaison
      code->data[i]=rand()%NB_COLORS;  // Nouveau code à trouver
    }
    printf("CODE=");printArray(code);
  }
}
///////////////////////////////////////////////////////////////////////////////
// Fonction de remise a zero du jeu
// 1- Efface le contenu de la boite verticale
// 2- Genere un nouveau code
// 3- Ajoute une ligne par defaut
// 4- Affiche le contenu de la fenetre
// 5- Demarre le compteur de temps s'il est active (timeout>0)
///////////////////////////////////////////////////////////////////////////////
void newGame(GtkWidget *b,gpointer data) {
  if (debug) printf("newGame()...\n");
  nbCoups=0;
  startTimer();
  if (data!=NULL) {
    GtkWidget *window=GTK_WIDGET(data);
    GtkWidget *vbox=findChildByName(window,VBOX_NAME);
    if (vbox!=NULL) {
      //if (debug) printf("vbox:%p\n",vbox);
      newCode();               // Genere un nouveau code
      removeAll(vbox);         // Enleve les combinaisons precedentes
      addNewCombinaison(vbox); // Ajoute la premiere combinaison
      gtk_widget_show_all(vbox);
    }
    GtkWidget *progressBar=findChildByName(window,PBAR_NAME);
    if (progressBar!=NULL) {
      //if (debug) printf("progressBar:%p\n",progressBar);
      if (timeout==0.0) { // No time limits
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressBar),0.0);
      } else {
        gdouble val=gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(progressBar));
        if (val==0.0||val>=1.0) {
          //if (debug) printf("Start progressBar...\n");
          g_timeout_add(1000,progressUpdate,window); // Restart
        }
      }
    }
  }
}
///////////////////////////////////////////////////////////////////////////////
// Ajoute une nouvelle combinaison dans vbox
// 1- Cree un boite horizontale (hbox)
// 2- Ajoute les couleurs et le bouton OK dans hbox
// 3- Ajoute la boite dans vbox
///////////////////////////////////////////////////////////////////////////////
void addNewCombinaison(GtkWidget *vbox) {
  if (code==NULL) return;
  GtkWidget *hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,5);
  //if (debug) printf("hbox:%p\n",hbox);
  gtk_widget_set_size_request(hbox,(nbCases+1)*BTN_SIZE+LBL_SIZE,BTN_SIZE);
  for (int i=0;i<=code->size;i++) { // nbCases + 1 (OK)
    GtkWidget *button=NULL;
    if (i<code->size) {
      int id=combinaison->data[i];
      button=gtk_button_new_with_label(LABELS[id]);
      g_signal_connect_after(button,"clicked",
                            (GCallback)clickColor,&(combinaison->data[i]));
      if (id>0) {
        gtk_widget_set_name(GTK_WIDGET(button),LABELS[id]);
      }
    } else {
      button=gtk_button_new_with_label("OK");
      g_signal_connect_after(button,"clicked",
                            (GCallback)clickOK,hbox);
    }
    gtk_widget_set_size_request(button,BTN_SIZE,BTN_SIZE);
    gtk_box_pack_start(GTK_BOX(hbox),button,FALSE,FALSE,0);
  }
  gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,0);
}
///////////////////////////////////////////////////////////////////////////////
// Fonction appelee lors d'un click sur une case de la combinaison
// data pointe sur la case correspondante dans le tableau de combinaison
// 1- Incremente la case pointée par data
// 2- Modifie les couleurs du bouton b
///////////////////////////////////////////////////////////////////////////////
void clickColor(GtkWidget *b,gpointer data) {
  if (data!=NULL) {
    int *caseCombi=(int *)data; // Case du tableau de combinaison
    // if (debug) printf("Click on (%s) -> ",
    //                   gtk_button_get_label(GTK_BUTTON(b)));
    *caseCombi+=1;         // Couleur suivante
    *caseCombi%=NB_COLORS; // Remise a zero si besoin
    if (*caseCombi==0) {
      gtk_widget_set_name(GTK_WIDGET(b),NULL);
      gtk_button_set_label(GTK_BUTTON(b),LABELS[*caseCombi]);
    } else {
      gtk_widget_set_name(GTK_WIDGET(b),LABELS[*caseCombi]);
      gtk_button_set_label(GTK_BUTTON(b),LABELS[*caseCombi]);
    }
  }
}
///////////////////////////////////////////////////////////////////////////////
// Desactive les boutons de la boite horizontale indiquee
// Indique le numero de la combinaison sur le bouton OK
///////////////////////////////////////////////////////////////////////////////
void disableButtons(GtkWidget *hbox) {
  if (GTK_IS_CONTAINER(hbox)) {
    //if (debug) printf("Desactive les boutons (hbox:%p)...\n",hbox);
    GList *liste,*child;
    child=liste=gtk_container_get_children(GTK_CONTAINER(hbox));
    for (int i=0;child!=NULL&&i<=code->size;child=g_list_next(child),i++) {
      gtk_widget_set_sensitive(child->data,FALSE);
      if (i==code->size) { // Bouton OK
        gchar str[4];
        snprintf(str,sizeof(str),"%02d",nbCoups+1);
        gtk_button_set_label(GTK_BUTTON(child->data),str);
      }
    }
    g_list_free(liste);
  }
}
///////////////////////////////////////////////////////////////////////////////
// Fonction appelee lors d'un click sur OK
// 1- Recupere les enfants de la boite horizontale pointee par data
// 2- Desactive tous les boutons
// 3- Affiche le resultat de la combinaison
// 4a- Affiche gagne si le code a ete trouve
// 4b- Ajoute une nouvelle combinaison si le code n'a pas ete trouve
// 5- Affiche le contenu de la fenetre
///////////////////////////////////////////////////////////////////////////////
void clickOK(GtkWidget *b,gpointer data) {
  gtk_widget_set_sensitive(b,FALSE);
  GtkWidget *hbox=GTK_WIDGET(data);
  if (GTK_IS_CONTAINER(hbox)) {
    disableButtons(hbox);
    // Recupere le conteneur parent
    GtkWidget* vbox=gtk_widget_get_parent(hbox);
    if (afficheResultat(hbox)==1) { // Ici, on a gagne !
      afficheGagne(vbox);
    } else {
      addNewCombinaison(vbox);
    }
    gtk_widget_show_all(vbox);
  }
}
///////////////////////////////////////////////////////////////////////////////
// Affiche le resultat de la verification de la combinaison
// Renvoi TRUE si le code est trouve et FALSE sinon
///////////////////////////////////////////////////////////////////////////////
gboolean afficheResultat(GtkWidget *hbox) {
  gboolean r=checkCombinaison();
  if (debug) printf("afficheResultat(%d) BP=%d:MP=%d...\n",r,
                    nbBienPlace,nbMalPlace);
  GtkWidget* label=gtk_label_new(NULL);
  gtk_label_set_use_markup(GTK_LABEL(label),TRUE);
  gchar *msg=SPAN(10.0,blue,%s : <b>%d</b>) "\n"
             SPAN(10.0,red,%s : <b>%d</b>);
  gchar *str=g_markup_printf_escaped(msg,
            _("Bien placées"),nbBienPlace,
            _("Mal placées"),nbMalPlace);
  gtk_label_set_markup(GTK_LABEL(label),str);
  gtk_label_set_justify(GTK_LABEL(label),GTK_JUSTIFY_RIGHT);
  g_free(str);
  gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,0);
  return r;
}
///////////////////////////////////////////////////////////////////////////////
// Affiche le message GAGNE
///////////////////////////////////////////////////////////////////////////////
void afficheGagne(GtkWidget *vbox) {
  if (debug) printf("afficheGagne()...\n");
  gulong tmp;
  gdouble t=g_timer_elapsed(timer,&tmp);
  GtkWidget* label=gtk_label_new(NULL);
  gtk_label_set_use_markup(GTK_LABEL(label),TRUE);
  gchar *msg=SPAN(15.0,blue,%s <b>%d</b> %s) "\n"
             SPAN(12.0,blue,%s <b>%.2f</b> %s);
  gchar *str=g_markup_printf_escaped(msg,
            _("Gagné en"),nbCoups,_("coups"),
            _("et"),t,_("secondes"));
  gtk_label_set_markup(GTK_LABEL(label),str);
  gtk_label_set_justify(GTK_LABEL(label),GTK_JUSTIFY_CENTER);
  g_free(str);
  gtk_box_pack_start(GTK_BOX(vbox),label,FALSE,FALSE,0);
  gtk_widget_show_all(vbox);
  finDePartie();
}
///////////////////////////////////////////////////////////////////////////////
// Affiche le message PERDU
///////////////////////////////////////////////////////////////////////////////
void affichePerdu(GtkWidget *vbox) {
  if (debug) printf("affichePerdu()...\n");
  GtkWidget* label=gtk_label_new(NULL);
  gtk_label_set_use_markup(GTK_LABEL(label),TRUE);
  gchar *msg=SPAN(15.0,red,%s <b>%d</b> %s);
  gchar *str=g_markup_printf_escaped(msg,
            _("Perdu en"),nbCoups,_("coups"));
  gtk_label_set_markup(GTK_LABEL(label),str);
  gtk_label_set_justify(GTK_LABEL(label),GTK_JUSTIFY_CENTER);
  g_free(str);
  gtk_box_pack_start(GTK_BOX(vbox),label,FALSE,FALSE,0);
  gtk_widget_show_all(vbox);
  finDePartie();
}
