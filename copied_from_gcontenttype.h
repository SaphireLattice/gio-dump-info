#include <glib-2.0/glib.h>
#include <gio/gio.h>
#include <gio/gdesktopappinfo.h>

static void
enumerate_mimetypes_subdir (const char *dir,
                            const char *prefix,
                            GHashTable *mimetypes)
{
  DIR *d;
  struct dirent *ent;
  char *mimetype;

  d = opendir (dir);
  if (d)
    {
      while ((ent = readdir (d)) != NULL)
        {
          if (g_str_has_suffix (ent->d_name, ".xml"))
            {
              mimetype = g_strdup_printf ("%s/%.*s", prefix, (int) strlen (ent->d_name) - 4, ent->d_name);

              // Except this part, I added that to actually debug the damn thing
              char *value = g_strdup_printf("%s/%s", dir, ent->d_name);
              g_hash_table_replace (mimetypes, mimetype, value);
            }
        }
      closedir (d);
    }
}

static void
enumerate_mimetypes_dir (const char *dir,
                         GHashTable *mimetypes)
{
  DIR *d;
  struct dirent *ent;
  const char *mimedir;
  char *name;

  mimedir = dir;

  d = opendir (mimedir);
  if (d)
    {
      while ((ent = readdir (d)) != NULL)
        {
          if (strcmp (ent->d_name, "packages") != 0)
            {
              name = g_build_filename (mimedir, ent->d_name, NULL);
              if (g_file_test (name, G_FILE_TEST_IS_DIR))
                enumerate_mimetypes_subdir (name, ent->d_name, mimetypes);
              g_free (name);
            }
        }
      closedir (d);
    }
}