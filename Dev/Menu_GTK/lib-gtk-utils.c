/************************************************
Utilitaires pour applications GTK+
Christophe BORELLY - 2014-10-08 - IUT de Beziers
************************************************/
#include "lib-gtk-utils.h"

///////////////////////////////////////////////////////////////////////////////
// Efface le contenu d'un conteneur
///////////////////////////////////////////////////////////////////////////////
void removeAll(GtkWidget *container) {
  if (GTK_IS_CONTAINER(container)) {
    GList *liste=gtk_container_get_children(GTK_CONTAINER(container));
    GList *child;
    for (child=liste;child!=NULL;child=g_list_next(child)) {
      gtk_widget_destroy(child->data);
    }
    g_list_free(liste);
  }
}
///////////////////////////////////////////////////////////////////////////////
// Recherche recursive d'un nom dans un conteneur
///////////////////////////////////////////////////////////////////////////////
GtkWidget* findChildByName(GtkWidget *container,const gchar *name) {
  if (GTK_IS_WIDGET(container)) {
    const gchar *cName=gtk_widget_get_name(container);
    if (g_strcmp0(cName,name)==0) { // Le nom du conteneur correspond
      return container;
    }
    if (GTK_IS_BIN(container)) { // Conteneur avec un seul enfant
      GtkWidget *child=gtk_bin_get_child(GTK_BIN(container));
      return findChildByName(child,name);
    }
    if (GTK_IS_CONTAINER(container)) { // Recherche dans la liste des enfants
      GList *childs=gtk_container_get_children(GTK_CONTAINER(container));
      GList *item;
      for(item=childs;item!=NULL;item=g_list_next(item)) {
        GtkWidget *widget=findChildByName(item->data,name);
        if (widget!=NULL) {
          g_list_free(childs);
          return widget;
        }
      }
      if (childs!=NULL) g_list_free(childs);
    }
  }
  return NULL;
}
///////////////////////////////////////////////////////////////////////////////
// Chargement de la mise en forme CSS
///////////////////////////////////////////////////////////////////////////////
void loadCSS() {
  GtkCssProvider *provider=gtk_css_provider_new();
  GdkDisplay *display=gdk_display_get_default();
  GdkScreen *screen=gdk_display_get_default_screen(display);
  gtk_style_context_add_provider_for_screen(screen,
                                            GTK_STYLE_PROVIDER(provider),
                                            GTK_STYLE_PROVIDER_PRIORITY_USER);
  GError *error=NULL;
  gtk_css_provider_load_from_path(provider,"style.css",&error);
  if (error!=NULL) {
    fprintf(stderr,"Unable to load CSS file: %s !\n",error->message);
    g_error_free(error);
  }
  g_object_unref(provider);
}
///////////////////////////////////////////////////////////////////////////////
// Prise en compte des fichiers de traduction
///////////////////////////////////////////////////////////////////////////////
void loadInternationalisation(char *package,char *locale) {
  bindtextdomain(package,locale);
  bind_textdomain_codeset(package,"UTF-8");
  textdomain(package);
}
