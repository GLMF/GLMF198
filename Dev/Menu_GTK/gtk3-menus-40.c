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
// Création d'une barre d'outils
///////////////////////////////////////////////////////////////////////////////
void addToolBar(GtkWidget *window) {
  GtkWidget *content=gtk_bin_get_child(GTK_BIN(window));
  GtkWidget *toolBar=gtk_toolbar_new();
  gtk_container_add(GTK_CONTAINER(content),toolBar);
  // Outil pour quitter l'application
  GtkWidget *icon=gtk_image_new_from_icon_name("application-exit",
                                               GTK_ICON_SIZE_MENU);
  GtkToolItem *item=gtk_tool_button_new(icon,_("Quitter"));
  gtk_tool_item_set_tooltip_markup(item,_("Terminer l'<b>application</b>"));
  gtk_toolbar_insert(GTK_TOOLBAR(toolBar),item,-1); // Append
  gtk_actionable_set_action_name(GTK_ACTIONABLE(item),"app.quit");
  // Séparateur
  item=gtk_separator_tool_item_new();
  gtk_toolbar_insert(GTK_TOOLBAR(toolBar),item,-1);
  // Outil pour commencer une partie
  icon=gtk_image_new_from_icon_name("face-smile",
                                    GTK_ICON_SIZE_MENU);
  item=gtk_tool_button_new(icon,_("Nouvelle partie"));
  gtk_tool_item_set_is_important(item,TRUE); // Display label
  gtk_tool_item_set_tooltip_text(item,_("Démarrer une nouvelle partie"));
  gtk_toolbar_insert(GTK_TOOLBAR(toolBar),item,-1);
  gtk_actionable_set_action_name(GTK_ACTIONABLE(item),"app.newGame");
  // Outil pour activer le mode expert
  item=gtk_toggle_tool_button_new();
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(item),_("Mode expert"));
  gtk_tool_item_set_tooltip_text(item,_("Modifier le mode expert"));
  gtk_toolbar_insert(GTK_TOOLBAR(toolBar),item,-1);
  gtk_actionable_set_action_name(GTK_ACTIONABLE(item),"app.modeExpert");
  gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(item),FALSE);
}
///////////////////////////////////////////////////////////////////////////////
// Création de la fenêtre et de son contenu
///////////////////////////////////////////////////////////////////////////////
static void startApplication(GtkApplication *app,gpointer data) {
  loadCSS();
  GtkWidget *window=gtk_application_window_new(app);
  gtk_window_set_icon_name(GTK_WINDOW(window),PACKAGE);
  gtk_window_set_title(GTK_WINDOW(window),"MasterMind v4");
  gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window),550,400);
  gtk_container_set_border_width(GTK_CONTAINER(window),10);
  GtkWidget *content=gtk_box_new(GTK_ORIENTATION_VERTICAL,2);
  gtk_container_add(GTK_CONTAINER(window),content);
  addAccels(app,window);
  addToolBar(window);
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
  GtkApplication *app=gtk_application_new("fr.iutbeziers.menus-40",
                                          G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app,"activate",G_CALLBACK(startApplication),NULL);
  int status=g_application_run(G_APPLICATION(app),argc,argv);
  g_object_unref(app);
  return status;
}
