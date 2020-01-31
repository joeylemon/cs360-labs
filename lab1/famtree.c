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

    // 0=unvisited
    // 1=visited, no error
    // 2=visited, error
    int visited;

    // 0=not printed
    // 1=printed
    int printed;
} Person;

// person_print_info(): prints information about the person
void person_print_info(Person* p) {
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

        Dllist dtmp;
        dll_traverse(dtmp, p->children) {
            Person *child = (Person *)dtmp->val.v;
            printf("    %s\n", child->name);
        }
    }

    printf("\n");
}

// person_is_own_descendant(): checks if a person is listed as their own descendent
// Return values:
//   1 => when the person is listed as a descendent of themself
//   0 => all other cases
int person_is_own_descendant(Person* p) {
    // If p is visited and none of it's children contain
    // p, return false
    if (p->visited == 1) return 0;

    // If p is visited while searching in it's own children
    // list, return true
    if (p->visited == 2) return 1;

    p->visited = 2;

    // Loop through all of p's children, recursively checking
    // for cycled descendants
    Dllist dtmp;
    dll_traverse(dtmp, p->children) {
        Person *child = (Person *)dtmp->val.v;
        if (person_is_own_descendant(child)) return 1;
    }
    
    p->visited = 1;

    return 0;
}

// person_has_child(): checks if a person has the given child
// Return values:
//   1 => when the person has the given child in their children list
//   0 => all other cases
int person_has_child(Person* p, char *child_name) {
    Dllist dtmp;
    dll_traverse(dtmp, p->children) {
        Person *child = (Person *)dtmp->val.v;
        if (strcmp(child->name, child_name) == 0) return 1;
    }

    return 0;
}

// person_set_sex(): sets the person's sex to the new value
// Return values:
//   1 => when the new sex doesn't match the existing sex
//   0 => all other cases
int person_set_sex(Person* p, char sex) {
    if (p->sex && p->sex != sex)
        return 1;

    p->sex = sex;
    return 0;
}

// person_set_parent(): sets the person's parent
// Return values:
//   2 => when the parent's sex doesn't match the parent type
//   1 => when the person already has a different parent listed
//   0 => all other cases
int person_set_parent(Person* child, Person* parent, int father) {
    if (parent->sex != (father ? 'M' : 'F')) {
        return 2;
    }

    if (father) {
        if (child->father == NULL) {
            child->father = strdup(parent->name);
            return 0;
        } else if(strcmp(child->father, parent->name) != 0) {
            // If the child already has a father with a different name,
            // return an error
            return 1;
        }
        return 0;
    } else {
        if (child->mother == NULL) {
            child->mother = strdup(parent->name);
            return 0;
        } else if(strcmp(child->mother, parent->name) != 0) {
            // If the child already has a mother with a different name,
            // return an error
            return 1;
        }
        return 0;
    }
}

// person_add_child(): adds the child to the parent's list of children
void person_add_child(Person* parent, Person* child) {
    if (!person_has_child(parent, child->name))
        dll_append(parent->children, new_jval_v((void *)child));
}

// find_person(): finds the person by name in the tree
// Return value:
//   a pointer to the person in the tree
Person* find_person(JRB tree, char* name) {
    JRB node = jrb_find_str(tree, name);
    if (node == NULL) return NULL;
    return (Person *)node->val.v;
}

// insert_person(): allocates and adds a new person to the tree with the given name
// Return value:
//   a pointer to the newly-added person
Person* insert_person(JRB tree, char* name) {
    Person *p = malloc(sizeof(Person));
    p->name = strdup(name);
    p->children = new_dllist();
    jrb_insert_str(tree, p->name, new_jval_v((void *)p));

    return p;
}

// get_line_value(): creates a string of all fields in the input struct
// Return value:
//   a string with all fields joined by spaces
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
    is = new_inputstruct("fam5");
    t = make_jrb();

    while (get_line(is) >= 0) {
        // Skip useless lines
        if (is->NF <= 1) continue;

        // Read the value of the field after the keyword
        char *value = get_line_value(is);

        if (strcmp(is->fields[0], "PERSON") == 0) {
            p = find_person(t, value);

            // Create the person if they don't exist
            if (p == NULL) p = insert_person(t, value);
        } else if (strcmp(is->fields[0], "FATHER_OF") == 0 || strcmp(is->fields[0], "MOTHER_OF") == 0) {
            Person *child = find_person(t, value);
            int father = strcmp(is->fields[0], "FATHER_OF") == 0 ? 1 : 0;

            // Set the parent's sex
            if (person_set_sex(p, father ? 'M' : 'F') == 1) {
                printf("Bad input - sex mismatch on line %d\n", is->line);
                exit(1);
            }

            // Create the person if they don't exist
            if (child == NULL) 
                child = insert_person(t, value);

            int set_parent_error = person_set_parent(child, p, father);
            if (set_parent_error == 1) {
                printf("Bad input -- child with two %s on line %d\n", father ? "fathers" : "mothers", is->line);
                exit(1);
            } else if (set_parent_error == 2) {
                printf("Bad input - sex mismatch on line %d\n", is->line);
                exit(1);
            }

            person_add_child(p, child);

        } else if (strcmp(is->fields[0], "FATHER") == 0 || strcmp(is->fields[0], "MOTHER") == 0) {
            int father = strcmp(is->fields[0], "FATHER") == 0 ? 1 : 0;

            Person *parent = find_person(t, value);

            // Create the person if they don't exist
            if (parent == NULL)
                parent = insert_person(t, value);

            // Set the parent's sex
            if (person_set_sex(parent, father ? 'M' : 'F') == 1) {
                printf("Bad input - sex mismatch on line %d\n", is->line);
                exit(1);
            }
            
            int set_parent_error = person_set_parent(p, parent, father);
            if (set_parent_error == 1) {
                printf("Bad input -- child with two %s on line %d\n", father ? "fathers" : "mothers", is->line);
                exit(1);
            } else if (set_parent_error == 2) {
                printf("Bad input - sex mismatch on line %d\n", is->line);
                exit(1);
            }

            person_add_child(parent, p);
        } else if (strcmp(is->fields[0], "SEX") == 0) {
            if (!p->sex) p->sex = is->fields[1][0];
        }

        // Free the value of the line since we used malloc to get it
        free(value);
    }

    // Check for a cycle
    jrb_traverse(tmp, t) {

        // Set all people to unvisited
        JRB tmp1;
        jrb_traverse(tmp1, t) {
            Person *p1 = (Person *)tmp->val.v;
            p1->visited = 0;
        }

        // Check if the current person has a cycle of descendents
        p = (Person *)tmp->val.v;
        if (person_is_own_descendant(p)) {
            printf("Bad input -- cycle in specification\n");
            exit(1);
        }
    }

    /* assume that there is an integer field called "printed" 
   in the Person struct, and that this field is initialized 
   to zero for all people */
 
    // while toprint is not empty
    //   take p off the head of toprint
    //   if p has not been printed, then 
    //     if p doesn't have parents, or if p's parents have been printed then
    //       print p
    //       for all of p's children, put the child at the end of toprint
    //     end if
    //   end if
    // end while

    Dllist toprint;
    while (!dll_empty(toprint)) {
        p = (Person *)toprint->val.v;
        if (!p->printed) {
            
        }
    }

    // Print out all information
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