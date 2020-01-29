#include <stdlib.h>
#include <string.h>
#include "fields.h"
#include "jrb.h"

typedef struct {
    char *name;
    char sex;
} Person;

int main() {
    JRB t, tmp;
    IS is;
    Person *p;

    int nsize, i;

    // TODO: change this to new_inputstruct(NULL) for stdin
    is = new_inputstruct("fam1");
    t = make_jrb();

    while (get_line(is) >= 0) {
        // Skip useless lines
        if (is->NF <= 1) continue;

        if (strcmp(is->fields[0], "PERSON") == 0) {
            p = malloc(sizeof(Person));

            nsize = strlen(is->fields[0]);
            for (i = 1; i < is->NF - 1; i++) nsize += (strlen(is->fields[i]) + 1);

            p->name = (char *)malloc(sizeof(char) * (nsize + 1));
            strcpy(p->name, is->fields[0]);

            nsize = strlen(is->fields[0]);
            for (i = 1; i < is->NF - 1; i++) {
                p->name[nsize] = ' ';
                strcpy(p->name + nsize + 1, is->fields[i]);
                nsize += strlen(p->name + nsize);
            }

            jrb_insert_str(t, p->name, new_jval_v((void *)p));
        }
    }

    jrb_traverse(tmp, t) {
        p = (Person *)tmp->val.v;
        printf("%-40s\n", p->name);
    }
    exit(0);
}