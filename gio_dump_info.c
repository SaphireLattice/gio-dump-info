#include <glib-2.0/glib.h>
#include <gio/gio.h>
#include <gio/gdesktopappinfo.h>
#include <stdbool.h>
#include <stdio.h>

#include "copied_from_gcontenttype.h"

static gboolean print_all_supported = FALSE;
static gchar** mimetype_arg = NULL;

static GOptionEntry entries[] =
{
  { G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_STRING_ARRAY, &mimetype_arg, "Mimetypes to query", "MIMETYPES" },
  { "supported", 0, 0, G_OPTION_ARG_NONE, &print_all_supported, "Print supported mimetypes for each associated application", NULL },
  { NULL }
};

void print_app_info_list(GList* list, const char* desired_type);

int main(int argc, char** argv) {
    GError *error = NULL;
    GOptionContext *context;

    context = g_option_context_new ("- print information about GIO file associations");
    g_option_context_add_main_entries (context, entries, NULL);
    if (!g_option_context_parse (context, &argc, &argv, &error)) {
        g_print ("option parsing failed: %s\n", error->message);
        exit (1);
    }

    int arg_count = 0;
    for (gchar** arg = mimetype_arg; *arg != NULL; arg += 1) {
        arg_count++;
    }

    GHashTable *mimetypes = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);

    const char * const *dirs = g_content_type_get_mime_dirs ();
    for (int i = 0; dirs[i] != NULL; i++)
        enumerate_mimetypes_dir (dirs[i], mimetypes);


    if (arg_count < 1) {
        printf("Mimetype not given, printing all registered types\n");

        GHashTableIter iter;
        gpointer key;
        gpointer value;

        g_hash_table_iter_init (&iter, mimetypes);
        while (g_hash_table_iter_next (&iter, &key, &value))
        {
            printf("  - %s %s\n", (char*) key, (char*) value);
        }

        g_hash_table_destroy (mimetypes);

        return 0;
    }

    const char* mimetype = mimetype_arg[0];

    printf("File associations for \"%s\"\n", mimetype);

    if (g_hash_table_contains(mimetypes, mimetype))
        printf(" - Relevant file (might contain alias and subclass info): %s\n", (char*) g_hash_table_lookup(mimetypes, mimetype));

    printf(" - Content type: %s\n", g_content_type_from_mime_type(mimetype));
    printf(" - Mime type: %s\n", g_content_type_get_mime_type(mimetype));

    GList* list = g_app_info_get_all_for_type(mimetype);
    print_app_info_list(list, mimetype);
    g_list_free(list);

    return 0;
}

void print_app_info_list(GList* list, const char* desired_type) {
    GList* next = list;
    int counter = 0;

    printf("Elements: %i\n", g_list_length(list));

    while (next != NULL) {
        GAppInfo* info = next->data;
        printf(
            "%i: %s - %s (%s)", counter++,
            g_app_info_get_name(info),
            g_app_info_get_id(info),
            g_app_info_get_executable(info)
        );
        bool direct_support = false;
        const char** supported_types = g_app_info_get_supported_types(info);

        const char** supported_types_iter = supported_types;
        while (*supported_types_iter != NULL) {
            if (strcmp(*supported_types_iter, desired_type) == 0)
                direct_support = true;
            supported_types_iter = supported_types_iter + 1;
        }

        if (!direct_support)
            printf(" - SUBCLASSED/ALIASED");

        printf("\n");

        supported_types_iter = supported_types;
        while (*supported_types_iter != NULL) {
            if (print_all_supported)
                printf("  - %s\n", *supported_types_iter);
            supported_types_iter = supported_types_iter + 1;
        }

        next = next->next;
    }
}