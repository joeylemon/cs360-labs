#include <stdlib.h>
#include <string.h>
#include "fields.h"
#include "jrb.h"
#include "dllist.h"

typedef struct {
    char *name;
    char sex;
    char *father;
    char *mother;

    Dllist children;
} Person;

Person* find_person(JRB tree, char* name) {
    JRB node = jrb_find_str(tree, name);
    if (node == NULL) return NULL;
    return (Person *)node->val.v;
}

unsigned char person_has_child(Person* p, char *child_name) {
    Dllist dtmp;
    dll_traverse(dtmp, p->children) {
        Person *child = (Person *)dtmp->val.v;
        if (strcmp(child->name, child_name) == 0) return 1;
    }

    return 0;
}

char* get_line_value(IS is) {
    int nsize, i;
    char *value;

    // Calculate the size of the upcoming string
    nsize = strlen(is->fields[1]);
    for (i = 2; i < is->NF; i++) nsize += (strlen(is->fields[i]) + 1);

    // Allocate memory for the string
    value = (char *)malloc(sizeof(char) * (nsize + 1));
    strcpy(value, is->fields[1]);

    // Copy all of the interesting fields into the string
    nsize = strlen(is->fields[1]);
    for (i = 2; i < is->NF; i++) {
        value[nsize] = ' ';
        strcpy(value + nsize + 1, is->fields[i]);
        nsize += strlen(value + nsize);
    }

    return value;
}

int main() {
    JRB t, tmp;
    Dllist dtmp;
    IS is;
    Person *p;

    int nsize, i;

    // TODO: change this to new_inputstruct(NULL) for stdin
    is = new_inputstruct("fam2");
    t = make_jrb();

    while (get_line(is) >= 0) {
        // Skip useless lines
        if (is->NF <= 1) continue;

        // Read the value of the field after the keyword
        char *value = get_line_value(is);

        if (strcmp(is->fields[0], "PERSON") == 0) {
            p = find_person(t, value);

            // Don't create a new person if they already exist
            if (p == NULL) {
                p = malloc(sizeof(Person));
                p->name = strdup(value);
                p->children = new_dllist();
                jrb_insert_str(t, p->name, new_jval_v((void *)p));
            }
        } else if (strcmp(is->fields[0], "FATHER_OF") == 0 || strcmp(is->fields[0], "MOTHER_OF") == 0) {
            Person *child = find_person(t, value);
            unsigned char father = strcmp(is->fields[0], "FATHER_OF") == 0 ? 1 : 0;

            // Set the parent's sex
            p->sex = father ? 'M' : 'F';

            // If the child doesn't exist, create it and set the values
            if (child == NULL) {
                child = malloc(sizeof(Person));
                child->name = strdup(value);
                child->children = new_dllist();

                if (father) child->father = strdup(p->name);
                else child->mother = strdup(p->name);

                jrb_insert_str(t, child->name, new_jval_v((void *)child));

            // If the child already exists, update parent value
            } else {
                if (father) {
                    // If the child already has a father, don't do anything
                    if (child->father == NULL) child->father = strdup(p->name);
                } else {
                    // If the child already has a mother, don't do anything
                    if (child->mother == NULL) child->mother = strdup(p->name);
                }
            }

            if (!person_has_child(p, child->name))
                dll_append(p->children, new_jval_v((void *)child));

        } else if (strcmp(is->fields[0], "FATHER") == 0 || strcmp(is->fields[0], "MOTHER") == 0) {
            unsigned char father = strcmp(is->fields[0], "FATHER") == 0 ? 1 : 0;

            Person *parent = find_person(t, value);

            // Don't create a new person if they already exist
            if (parent == NULL) {
                parent = malloc(sizeof(Person));

                parent->name = strdup(value);
                parent->sex = father ? 'M' : 'F';
                parent->children = new_dllist();
                dll_append(parent->children, new_jval_v((void *)p));

                jrb_insert_str(t, parent->name, new_jval_v((void *)parent));
            } else {
                parent->sex = father ? 'M' : 'F';

                if (!person_has_child(parent, p->name))
                    dll_append(parent->children, new_jval_v((void *)p));
            }
        } else if (strcmp(is->fields[0], "SEX") == 0) {
            if (!p->sex) p->sex = is->fields[1][0];
        }

        // Free the value of the line since we used malloc to get it
        free(value);
    }

    jrb_traverse(tmp, t) {
        p = (Person *)tmp->val.v;
        printf("%s\n", p->name);

        if (p->sex == 'M')  printf("  Sex: Male\n");
        else                printf("  Sex: Female\n");

        if (p->father)      printf("  Father: %s\n", p->father);
        else                printf("  Father: Unknown\n");

        if (p->mother)      printf("  Mother: %s\n", p->mother);
        else                printf("  Mother: Unknown\n");

        if (dll_empty(p->children)) {
            printf("  Children: None\n");
        } else {
            printf("  Children:\n");
            dll_traverse(dtmp, p->children) {
                Person *child = (Person *)dtmp->val.v;
                printf("    %s\n", child->name);
            }
        }

        printf("\n");
    }

    // Free all persons and their respectable fields
    jrb_traverse(tmp, t) {
        p = (Person *)tmp->val.v;
        free(p->name);

        if (p->father) free(p->father);
        if (p->mother) free(p->mother);
        free_dllist(p->children);

        free(p);
    }

    // Free the JRB and IS
    jrb_free_tree(t);
    jettison_inputstruct(is);

    exit(0);
}