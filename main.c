/* This file is part of herzi's playground
 *
 * AUTHORS
 *     Sven Herzberg  <herzi@gnome-de.org>
 *
 * Copyright (C) 2007  Sven Herzberg
 *
 * This work is provided "as is"; redistribution and modification
 * in whole or in part, in any medium, physical or electronic is
 * permitted without restriction.
 *
 * This work is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * In no event shall the authors or contributors be liable for any
 * direct, indirect, incidental, special, exemplary, or consequential
 * damages (including, but not limited to, procurement of substitute
 * goods or services; loss of use, data, or profits; or business
 * interruption) however caused and on any theory of liability, whether
 * in contract, strict liability, or tort (including negligence or
 * otherwise) arising in any way out of the use of this software, even
 * if advised of the possibility of such damage.
 */

#include <gtk/gtk.h>

static GtkWidget*
mainwin_new (void)
{
	GtkWidget* result = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect_after (result, "destroy",
				G_CALLBACK (gtk_main_quit), NULL);
	return result;
}

int
main (int   argc,
      char**argv)
{
	gtk_init (&argc, &argv);
	gtk_widget_show (mainwin_new ());
	gtk_main ();
	return 0;
}

