#include "lib-mastermind.h"

///////////////////////////////////////////////////////////////////////////////
// Fonction appelée lors du clic sur le menu "Mode expert"
///////////////////////////////////////////////////////////////////////////////
static void menuExpert(GtkCheckMenuItem *m,gpointer data) {
  gboolean state=gtk_check_menu_item_get_active(m);
  nbCases=(state)?6:4;
  printf("menuExpert(%d): %d\n",state,nbCases);
}
///////////////////////////////////////////////////////////////////////////////
// Fonction appelée lors du clic sur le menu "Temps illimité"
///////////////////////////////////////////////////////////////////////////////
static void menuTime(GtkCheckMenuItem *m,gpointer data) {
  gboolean state=gtk_check_menu_item_get_active(m);
  timeout=(state)?0.0:TIMEOUT;
  printf("menuTime(%d): %.1f\n",state,timeout);
}
///////////////////////////////////////////////////////////////////////////////
// Fonction appelée pour terminer l'application
///////////////////////////////////////////////////////////////////////////////
static void menuQuit(GtkWidget *m,gpointer data) {
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
// Création et ajout du menu avec raccourcis et markup
///////////////////////////////////////////////////////////////////////////////
void addMenu(GtkWidget *window) {
  GtkAccelGroup *accelGroup=gtk_accel_group_new(); // Groupe d'accélérateurs
  gtk_window_add_accel_group(GTK_WINDOW(window),accelGroup);
  GtkWidget *content=gtk_bin_get_child(GTK_BIN(window));
  GtkWidget *menuBar=gtk_menu_bar_new();
  gtk_container_add(GTK_CONTAINER(content),menuBar);
  GtkWidget *menuItem=gtk_menu_item_new_with_mnemonic(_("_Jeu"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menuBar),menuItem);
  GtkWidget *menuNiveau1=gtk_menu_new();
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem),menuNiveau1);
  menuItem=gtk_menu_item_new_with_mnemonic(_("_Nouvelle partie"));
  // Ajout d'un raccourci clavier
  gtk_widget_add_accelerator(menuItem,"activate",accelGroup,
                             GDK_KEY_n,GDK_CONTROL_MASK,
                             GTK_ACCEL_VISIBLE);
  // Utilisation du langage à balises de Pango
  GtkWidget *child=gtk_bin_get_child(GTK_BIN(menuItem));
  gtk_label_set_use_underline(GTK_LABEL(child),TRUE);
  const char *format=SPAN(12.0,blue,%s) " with "
                     SPAN(10.0,red,<i>Pango markup</i>);
  const gchar *label=gtk_menu_item_get_label(GTK_MENU_ITEM(menuItem));
  char *markup=g_markup_printf_escaped(format,label);
  gtk_label_set_markup_with_mnemonic(GTK_LABEL(child),markup);
  g_free(markup);
  gtk_menu_shell_append(GTK_MENU_SHELL(menuNiveau1),menuItem);
  g_signal_connect_after(menuItem,"activate",G_CALLBACK(newGame),window);
  menuItem=gtk_menu_item_new_with_mnemonic(_("_Péférences"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menuNiveau1),menuItem);
  GtkWidget *menuNiveau2=gtk_menu_new(); // Sous menu
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem),menuNiveau2);
  menuItem=gtk_separator_menu_item_new(); // Separateur
  gtk_menu_shell_append(GTK_MENU_SHELL(menuNiveau1),menuItem);
  menuItem=gtk_menu_item_new_with_mnemonic(_("_Quitter"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menuNiveau1),menuItem);
  g_signal_connect(menuItem,"activate",G_CALLBACK(menuQuit),window);
  gtk_widget_add_accelerator(menuItem,"activate",accelGroup,
                             GDK_KEY_q,GDK_CONTROL_MASK,
                             GTK_ACCEL_VISIBLE);
  // Sous menu de préférences
  menuItem=gtk_check_menu_item_new_with_mnemonic(_("_Mode expert"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menuNiveau2),menuItem);
  g_signal_connect(menuItem,"activate",G_CALLBACK(menuExpert),window);
  menuItem=gtk_check_menu_item_new_with_mnemonic(_("_Temps illimité"));
  gtk_menu_shell_append(GTK_MENU_SHELL(menuNiveau2),menuItem);
  g_signal_connect(menuItem,"activate",G_CALLBACK(menuTime),window);
  gtk_widget_set_sensitive(menuItem,FALSE); // Desactive le menu
}
///////////////////////////////////////////////////////////////////////////////
// Création de la fenêtre et de son contenu
///////////////////////////////////////////////////////////////////////////////
static void startApplication(GtkApplication *app,gpointer data) {
  loadCSS();
  GtkWidget *window=gtk_application_window_new(app);
  gtk_window_set_icon_name(GTK_WINDOW(window),PACKAGE);
  gtk_window_set_title(GTK_WINDOW(window),"MasterMind v1.2");
  gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window),550,400);
  gtk_container_set_border_width(GTK_CONTAINER(window),10);
  GtkWidget *content=gtk_box_new(GTK_ORIENTATION_VERTICAL,2);
  gtk_container_add(GTK_CONTAINER(window),content);
  addMenu(window);
  // Barre de progression pour le temps restant
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
  GtkApplication *app=gtk_application_new("fr.iutbeziers.menus-12",
                                          G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app,"activate",G_CALLBACK(startApplication),NULL);
  int status=g_application_run(G_APPLICATION(app),argc,argv);
  g_object_unref(app);
  return status;
}
