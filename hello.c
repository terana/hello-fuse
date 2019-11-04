#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <string.h>
#include <errno.h>

static struct hello_file {
        const char *name;
        const char *contents;
} hello_file;

static void *hello_init(struct fuse_conn_info *conn,
                        struct fuse_config *cfg)
{
        /* Disable flushing the cache of the file contents on every open,
         * since the file data is never changed. */
        cfg->kernel_cache = 1;

        return NULL;
}

static int hello_getattr(const char *path, struct stat *stbuf,
                         struct fuse_file_info *fi)
{
        int res = 0;
        memset(stbuf, 0, sizeof(*stbuf));

        if (strcmp(path, "/") == 0) {
                stbuf->st_mode = S_IFDIR | 0755;
                stbuf->st_nlink = 2;

        } else if (strcmp(path + 1, hello_file.name) == 0) {
                stbuf->st_mode = S_IFREG | 0444;
                stbuf->st_nlink = 1;
                stbuf->st_size = strlen(hello_file.contents);

        } else {
                res = -ENOENT;
        }

        return res;
}

static int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi,
                         enum fuse_readdir_flags flags)
{
        if (strcmp(path, "/") != 0) {
                return -ENOENT;
        }

        filler(buf, ".", NULL, 0, 0);
        filler(buf, "..", NULL, 0, 0);
        filler(buf, hello_file.name, NULL, 0, 0);

        return 0;
}

static int hello_open(const char *path, struct fuse_file_info *fi)
{
        if (strcmp(path + 1, hello_file.name) != 0) {
                return -ENOENT;
        }

        if ((fi->flags & O_ACCMODE) != O_RDONLY) {
                return -EACCES;
        }

        return 0;
}

static int hello_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
        size_t len;

        if(strcmp(path + 1, hello_file.name) != 0) {
                return -ENOENT;
        }

        len = strlen(hello_file.contents);
        if (offset < len) {
                if (offset + size > len) {
                        size = len - offset;
                }
                memcpy(buf, hello_file.contents + offset, size);

        } else {
                size = 0;
        }

        return size;
}
static struct fuse_operations hello_oper = {
        .init           = hello_init,
        .getattr        = hello_getattr,
        .readdir        = hello_readdir,
        .open           = hello_open,
        .read           = hello_read,
};

int main(int argc, char *argv[])
{
        int ret;
        struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

        hello_file.name = "hello";
        hello_file.contents = "Hello World!\n";

        ret = fuse_main(args.argc, args.argv, &hello_oper, NULL);
        fuse_opt_free_args(&args);

        return ret;
}
