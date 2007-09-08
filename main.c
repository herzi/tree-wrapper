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
#include <glib/gi18n.h>

enum {
	COL_TEXT,
	N_COLUMNS
};

static void
tree_cell_data_func (GtkTreeViewColumn* column,
		     GtkCellRenderer  * renderer,
		     GtkTreeModel     * model,
		     GtkTreeIter      * iter,
		     gpointer           data)
{
	GdkRectangle  rect = {0,0,0,0};
	gchar       * text = NULL;

	gtk_tree_model_get (model, iter,
			    COL_TEXT, &text,
			    -1);

	if (GTK_WIDGET_REALIZED (data)) {
		GtkTreePath * path;
		path = gtk_tree_model_get_path (model, iter);
		gtk_tree_view_get_cell_area (data,
					     path,
					     column,
					     &rect);
		gtk_tree_path_free (path);

		g_print ("%d\n", rect.width);
	} else {
		rect.width = 100;
	}

	g_object_set (renderer,
		      "text", text,
		      "wrap-width", rect.width - 6, // FIXME: Kris, what's this number? Where does it come from?
		      NULL);

	g_free (text);
}

static GtkTreeModel*
treemodel_new (void)
{
	GtkTreeStore* result;
	GtkTreeIter   iter;

	// text taken from http://209.85.135.104/search?q=cache:5GSj55F5I_YJ:www.newmediadesigner.de/38.htm+site:www.newmediadesigner.de+blindtext+archiv&hl=de&gl=de&strip=1
	result = gtk_tree_store_new (N_COLUMNS, G_TYPE_STRING);
	gtk_tree_store_append (result, &iter, NULL);
	gtk_tree_store_set    (result, &iter,
			       COL_TEXT, "This, of course, is not the real copy for this advertisement. The real words will be written once you have approved the headline. Rest assured, the words will expand the concept. With clarity. Conviction. And even a little wit. Because in today’s competitive marketing environment, the body copy of your advertisment must lead the reader through a series of disarmingly simple thoughts. All your supporting arguments must be communicated with simplicity and charm. And in such a way that the reader will read on. (After all, that’s a reader’s job: to read, isn’t it?) And by the time your readers have reached this point in the finished copy, you will have convinced them that you not only respect their intelligence, but you also understand their needs as consumers. As a result of which, your advertisment will repay your efforts. Take your sales; simply put, they will rise. Likewise your credibility. There’s every chance your competitors will wish they’d placed this advertisement, not you. While your customers will have probably forgotten that your competitors even exist. Which brings us, by a somewhat circuitous route, to another small point, but one which we feel should be raised. As a marketer, you probably don’t even believe in body copy. Let alone long body copy. (Unless you have a long body yourself.) Well, truth is, who‘s to blame you? Fact is, too much long body copy is dotted with such indulgent little phrases like truth is, fact is, and who’s to blame you. Trust us: we guarantee, with a hand over our heart, that no such indulgent rubbish will appear in your advertisement. That’s why God gave us big blue pencils. So we can expunge every expedient example of low-witted waffle. For you, the skies will be blue, the birds will sing, and your copy will be crafted by a dedicated little man whose wife will be sitting at home, knitting, wondering why your advertisement demands more of her husband‘s time than it should. But you will know why, won‘t you? You will have given her husband a chance to immortalise himself in print, writing some of the most persuasive prose on behalf of a truly enlightened purveyor of widgets. And so, while your dedicated reader, enslaved to each mellifluous paragraph, clutches his newspaper with increasing interest and intention to purchase, you can count all your increased profits and take pots of money to your bank. Sadly, this is not the real copy for this advertisement. But it could well be. All you have to do is look at the account executive sitting across your desk (the fellow with the lugubrious face and the calf-like eyes), and say ”Yes! Yes! Yes!“ And anything you want, body copy, dinners, women, will be yours. Couldn’t be fairer than that, could we?",
			       -1);

	return GTK_TREE_MODEL (result);
}

static void
tree_size_request (GtkTreeView   * treeview,
		   GtkRequisition* requisition,
		   gpointer        data)
{
	GtkCellRenderer* renderer = data;

	gtk_tree_view_column_set_fixed_width (gtk_tree_view_get_column (treeview, 0),
					      requisition->width - 1);
}

static gboolean
emit_row_changed (GtkTreeModel* model,
		  GtkTreePath * path,
		  GtkTreeIter * iter,
		  gpointer      unused)
{
	gtk_tree_model_row_changed (model, path, iter);
	return FALSE;
}

static void
tree_size_allocate (GtkTreeView  * treeview,
		    GtkAllocation* requisition,
		    gpointer       data)
{
	GtkCellRenderer* renderer = data;

	// queue size updates for the model
	gtk_tree_model_foreach (gtk_tree_view_get_model (treeview),
				emit_row_changed,
				NULL);
}

static GtkWidget*
treeview_new (void)
{
	GtkCellRenderer* renderer;
	GtkTreeModel   * model;
	GtkWidget      * result;

	model  = treemodel_new ();
	result = gtk_tree_view_new_with_model (model);
	g_object_unref (model);

	renderer = gtk_cell_renderer_text_new ();

	g_signal_connect       (result, "size-request",
				G_CALLBACK (tree_size_request), renderer);
	g_signal_connect_after (result, "size-allocate",
				G_CALLBACK (tree_size_allocate), renderer);

	g_object_set (renderer,
		      "wrap-mode", PANGO_WRAP_WORD_CHAR,
		      NULL);

	gtk_tree_view_insert_column_with_data_func (GTK_TREE_VIEW (result), -1,
						    _("Text Column"),
						    renderer,
						    tree_cell_data_func,
						    result, NULL);

	return result;
}

static GtkWidget*
mainwin_new (void)
{
	GtkWidget* result = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	GtkWidget* scrolled;
	GtkWidget* treeview;

	g_signal_connect_after (result, "destroy",
				G_CALLBACK (gtk_main_quit), NULL);

	treeview = treeview_new ();
	gtk_widget_show   (treeview);

	scrolled = gtk_scrolled_window_new (NULL, NULL);
	gtk_container_add (GTK_CONTAINER (scrolled),
			   treeview);
	gtk_widget_show   (scrolled);

	gtk_container_add (GTK_CONTAINER (result),
			   scrolled);

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

