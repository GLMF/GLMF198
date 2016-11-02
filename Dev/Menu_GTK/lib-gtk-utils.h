/************************************************
Christophe BORELLY - 2014-10-08 - IUT de Beziers
************************************************/
#ifndef LIB_GTK_UTILS
#define LIB_GTK_UTILS

#include <gtk/gtk.h>
#include <glib/gi18n.h>

void removeAll(GtkWidget *container);
GtkWidget* findChildByName(GtkWidget *container,const gchar *name);
void loadCSS();
void loadInternationalisation(char *package,char *locale);

#endif
