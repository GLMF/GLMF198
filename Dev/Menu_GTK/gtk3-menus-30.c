#include "lib-mastermind.h"

///////////////////////////////////////////////////////////////////////////////
// Fonction de modification de l'état du mode expert
///////////////////////////////////////////////////////////////////////////////
static void changeModeExpert(GSimpleAction *action,
                             GVariant *state,gpointer data) {
  gboolean enabled=g_variant_get_boolean(state);
  nbCases=(enabled)?6:4;
  printf("changeModeExpert(%d): %d\n",enabled,nbCases);
  g_simple_action_set_state(action,state);
}
///////////////////////////////////////////////////////////////////////////////
// Action appelée pour changer la difficulté (mode expert)
///////////////////////////////////////////////////////////////////////////////
static void actionExpert(GSimpleAction *action,
                         GVariant *parameter,gpointer data) {
  GVariant *state=g_action_get_state(G_ACTION(action));
  gboolean enabled=g_variant_get_boolean(state);
  printf("actionExpert(%d)\n",enabled);
  g_action_change_state(G_ACTION(action),g_variant_new_boolean(!enabled));
  g_variant_unref(state);
}
///////////////////////////////////////////////////////////////////////////////
// Fonction de modification de l'état du temps
///////////////////////////////////////////////////////////////////////////////
static void changeTime(GSimpleAction *action,
                       GVariant *state,gpointer data) {
  gboolean enabled=g_variant_get_boolean(state);
  timeout=(enabled)?0.0:TIMEOUT;
  printf("changeTime(%d): %.1f\n",enabled,timeout);
  g_simple_action_set_state(action,state);
}
///////////////////////////////////////////////////////////////////////////////
// Action appelée pour changer la difficulté (mode expert)
///////////////////////////////////////////////////////////////////////////////
static void actionTime(GSimpleAction *action,
                         GVariant *parameter,gpointer data) {
  GVariant *state=g_action_get_state(G_ACTION(action));
  gboolean enabled=g_variant_get_boolean(state);
  printf("actionTime(%d)\n",enabled);
  g_action_change_state(G_ACTION(action),g_variant_new_boolean(!enabled));
  g_variant_unref(state);
}
///////////////////////////////////////////////////////////////////////////////
// Action appelée pour démarrer le jeu
///////////////////////////////////////////////////////////////////////////////
static void actionNewGame(GSimpleAction *action,
                          GVariant *parameter,gpointer data) {
  if (data!=NULL) {
    GtkWidget *window=(GtkWidget *)data;
    newGame(NULL,window);
  }
}
///////////////////////////////////////////////////////////////////////////////
// Action appelée pour terminer l'application
///////////////////////////////////////////////////////////////////////////////
static void actionQuit(GSimpleAction *action,
                       GVariant *parameter,gpointer data) {
  GtkWindow *window=NULL;
  GtkApplication *app=NULL;
  if (data!=NULL) {
    window=(GtkWindow *)data;
    app=gtk_window_get_application(window);
  }
  if (window!=NULL&&app!=NULL) {
    gtk_application_remove_window(app,window);
  }
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static GActionEntry appEntries[]={
  {"modeExpert",actionExpert,NULL,"false",changeModeExpert},
  {"noTime",actionTime,NULL,"false",changeTime},
  {"newGame",actionNewGame,NULL,NULL,NULL},
  {"quit",actionQuit,NULL,NULL,NULL}
};
///////////////////////////////////////////////////////////////////////////////
// Ajout des accélérateurs pour l'application
// Fixe le paramètre utilisateur avec data
///////////////////////////////////////////////////////////////////////////////
void addAccels(GtkApplication *app,gpointer data) {
  g_action_map_add_action_entries(G_ACTION_MAP(app),
                                  appEntries,G_N_ELEMENTS(appEntries),
                                  data);
  const gchar *newGameAccels[2]={"<Ctrl>N",NULL};
  gtk_application_set_accels_for_action(GTK_APPLICATION(app),
                                        "app.newGame",newGameAccels);
  const gchar *quitAccels[3]={"<Ctrl>Q","<Control><Shift>X",NULL};
  gtk_application_set_accels_for_action(GTK_APPLICATION(app),
                                        "app.quit",quitAccels);
}
///////////////////////////////////////////////////////////////////////////////
// Création du GMenu
///////////////////////////////////////////////////////////////////////////////
void addGMenu(GtkApplication *app) {
  g_set_application_name(_("_Jeu"));
  GMenu *gMenu=g_menu_new();
  g_menu_append(gMenu,_("_Nouvelle partie"),"app.newGame");
  GMenu *prefMenu=g_menu_new();
  g_menu_append(prefMenu,_("_Mode expert"),"app.modeExpert");
  g_menu_append(prefMenu,_("_Temps illimité"),"app.noTime(false)");
  g_menu_append_submenu(gMenu,_("_Péférences"),G_MENU_MODEL(prefMenu));
  g_object_unref(prefMenu);
  GMenuItem *gMenuItem=g_menu_item_new(_("_Quitter"),"app.quit");
  GError *error=NULL;
  GIcon *icon=g_icon_new_for_string("application-exit",&error);
  if (error!=NULL) {
    fprintf(stderr,"Unable to load icon : %s !\n",error->message);
    g_error_free(error);
  }
  g_menu_item_set_icon(gMenuItem,icon);
  g_menu_append_item(gMenu,gMenuItem);
  g_object_unref(gMenuItem);
  gtk_application_set_app_menu(app,G_MENU_MODEL(gMenu));
  g_object_unref(gMenu);
}
///////////////////////////////////////////////////////////////////////////////
// Création de la fenêtre et de son contenu
///////////////////////////////////////////////////////////////////////////////
static void startApplication(GtkApplication *app,gpointer data) {
  loadCSS();
  GtkWidget *window=gtk_application_window_new(app);
  gtk_window_set_icon_name(GTK_WINDOW(window),PACKAGE);
  gtk_window_set_title(GTK_WINDOW(window),"MasterMind v3");
  gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window),550,400);
  gtk_container_set_border_width(GTK_CONTAINER(window),10);
  GtkWidget *content=gtk_box_new(GTK_ORIENTATION_VERTICAL,2);
  gtk_container_add(GTK_CONTAINER(window),content);
  addAccels(app,window);
  addGMenu(app);
  // Barre de progression du temps restant
  GtkWidget *progressBar=gtk_progress_bar_new();
  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressBar),0.0);
  gtk_widget_set_name(GTK_WIDGET(progressBar),PBAR_NAME);
  gtk_container_add(GTK_CONTAINER(content),progressBar);
  // Zone de jeu scrollable
  GtkWidget *scroll=gtk_scrolled_window_new(NULL,NULL);
  gtk_widget_set_size_request(scroll,530,380);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
                                 GTK_POLICY_AUTOMATIC,GTK_POLICY_ALWAYS);
  // Zone pour les combinaisons
  GtkWidget *vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,5);
  gtk_widget_set_name(GTK_WIDGET(vbox),VBOX_NAME);
  gtk_container_set_border_width(GTK_CONTAINER(vbox),2);
  gtk_container_add(GTK_CONTAINER(scroll),vbox);
  int expand=TRUE,fill=TRUE,padding=0;
  gtk_box_pack_start(GTK_BOX(content),scroll,expand,fill,padding);

  gtk_widget_show_all(window);
}
///////////////////////////////////////////////////////////////////////////////
// Programme principal
///////////////////////////////////////////////////////////////////////////////
int main(int argc,char *argv[]) {
  loadInternationalisation(PACKAGE,LOCALE_DIR);
  GtkApplication *app=gtk_application_new("fr.iutbeziers.menus-30",
                                          G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app,"activate",G_CALLBACK(startApplication),NULL);
  int status=g_application_run(G_APPLICATION(app),argc,argv);
  g_object_unref(app);
  return status;
}
