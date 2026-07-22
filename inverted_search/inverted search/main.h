#define SIZE 27
#define WORD_SIZE 20
#define FILE_SIZE 20
#define SUCCESS 1
#define FAILURE 0 

typedef struct subnode
{
    int wordcount;
    char filename[FILE_SIZE];
    struct subnode *slink;
} snode;

typedef struct mainnode
{
    int filecount;
    char word[WORD_SIZE];
    snode *sublink;
    struct mainnode *mainlink;
} mnode;

typedef struct hash
{
    int index;
    mnode *link;
} hash_t;

typedef struct node
{
    char filename[SIZE];
    struct node *link;
} Flist;

int read_and_validation(int argc, char *argv[], Flist **head);
int insert_last(Flist **head, char *filename);
int create_database(hash_t *arr, Flist *head);
int display_database(hash_t *arr);
int search_database(hash_t *arr);
int save_database(hash_t *arr);
int update_database(hash_t *arr, Flist **head);