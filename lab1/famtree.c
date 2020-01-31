// Lab 1 -- Fields/dlist/rbtree primer
// COSC360
// This program builds a family tree given a file containing
// an unordered assortment of relationships
// Joey Lemon
// 2020-02-02

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

// find_person(): finds the person by name in the tree
// Return value:
//   a pointer to the person in the tree
Person* find_person(JRB tree, char* name);


// insert_person(): allocates and adds a new person to the tree with the given name
// Return value:
//   a pointer to the newly-added person
Person* insert_person(JRB tree, char* name);


// free_all_persons(): properly free all person structs in the tree
void free_all_persons(JRB tree);


// person_print_info(): prints information about the person
void person_print_info(Person* p);


// person_is_own_descendant(): checks if a person is listed as their own descendent
// Return values:
//   1 => when the person is listed as a descendent of themself
//   0 => all other cases
int person_is_own_descendant(Person* p);


// person_has_child(): checks if a person has the given child
// Return values:
//   1 => when the person has the given child in their children list
//   0 => all other cases
int person_has_child(Person* p, char *child_name);


// person_set_sex(): sets the person's sex to the new value
// Return values:
//   1 => when the new sex doesn't match the existing sex
//   0 => all other cases
int person_set_sex(Person* p, char sex);


// person_set_parent(): sets the person's parent
// Return values:
//   2 => when the parent's sex doesn't match the parent type
//   1 => when the person already has a different parent listed
//   0 => all other cases
int person_set_parent(Person* child, Person* parent, int father);


// person_add_child(): adds the child to the parent's list of children
void person_add_child(Person* parent, Person* child);


// person_parents_printed(): checks if the person's parents have been printed
// Return values:
//   1 => when all of the person's parents have been printed
//   0 => all other cases
int person_parents_printed(JRB tree, Person* p);


// get_line_value(): creates a string of all fields in the input struct
// Return value:
//   a string with all fields joined by spaces
char* get_line_value(IS is);


int main() {
    JRB t, tmp;
    Dllist dtmp;
    IS is;
    Person *p;

    int nsize, i;

    // Read input from stdin (NULL)
    is = new_inputstruct(NULL);
    t = make_jrb();

    while (get_line(is) >= 0) {
        // Skip useless lines
        if (is->NF <= 1) continue;

        // The keyword is the first word in the line
        char *keyword = is->fields[0];

        // Concatenate all the fields after the keyword
        char *value = get_line_value(is);

        if (strcmp(keyword, "PERSON") == 0) {
            p = find_person(t, value);

            // Create the person if they don't exist
            if (p == NULL) p = insert_person(t, value);
        } else if (strcmp(keyword, "FATHER_OF") == 0 || strcmp(keyword, "MOTHER_OF") == 0) {
            Person *child = find_person(t, value);

            // 0: person is a father     1: person is a mother
            int father = strcmp(keyword, "FATHER_OF") == 0 ? 1 : 0;

            // Set the parent's sex
            if (person_set_sex(p, father ? 'M' : 'F') == 1) {
                fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);

                // Since we're exiting early, free all memory now
                free_all_persons(t);
                free(value);
                jettison_inputstruct(is);
                exit(1);
            }

            // Create the person if they don't exist
            if (child == NULL) 
                child = insert_person(t, value);

            int set_parent_result = person_set_parent(child, p, father);
            if (set_parent_result == 1) {
                fprintf(stderr, "Bad input -- child with two %s on line %d\n", father ? "fathers" : "mothers", is->line);

                // Since we're exiting early, free all memory now
                free_all_persons(t);
                free(value);
                jettison_inputstruct(is);
                exit(1);
            } else if (set_parent_result == 2) {
                fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);

                // Since we're exiting early, free all memory now
                free_all_persons(t);
                free(value);
                jettison_inputstruct(is);
                exit(1);
            }

            person_add_child(p, child);

        } else if (strcmp(keyword, "FATHER") == 0 || strcmp(keyword, "MOTHER") == 0) {
            // 0: person is a father     1: person is a mother
            int father = strcmp(keyword, "FATHER") == 0 ? 1 : 0;

            Person *parent = find_person(t, value);

            // Create the person if they don't exist
            if (parent == NULL)
                parent = insert_person(t, value);

            // Set the parent's sex
            if (person_set_sex(parent, father ? 'M' : 'F') == 1) {
                fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);

                // Since we're exiting early, free all memory now
                free_all_persons(t);
                free(value);
                jettison_inputstruct(is);
                exit(1);
            }
            
            int set_parent_result = person_set_parent(p, parent, father);
            if (set_parent_result == 1) {
                fprintf(stderr, "Bad input -- child with two %s on line %d\n", father ? "fathers" : "mothers", is->line);

                // Since we're exiting early, free all memory now
                free_all_persons(t);
                free(value);
                jettison_inputstruct(is);
                exit(1);
            } else if (set_parent_result == 2) {
                fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);

                // Since we're exiting early, free all memory now
                free_all_persons(t);
                free(value);
                jettison_inputstruct(is);
                exit(1);
            }

            person_add_child(parent, p);
        } else if (strcmp(keyword, "SEX") == 0) {
            if (person_set_sex(p, is->fields[1][0]) == 1) {
                fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);

                // Since we're exiting early, free all memory now
                free_all_persons(t);
                free(value);
                jettison_inputstruct(is);
                exit(1);
            }
        }

        // Free the value of the line since we used malloc to get it
        free(value);
    }

    // Check for a cycle
    jrb_traverse(tmp, t) {
        // Check if the current person has a cycle of descendents
        p = (Person *)tmp->val.v;
        if (person_is_own_descendant(p)) {
            fprintf(stderr, "Bad input -- cycle in specification\n");

            // Since we're exiting early, free all memory now
            free_all_persons(t);
            jettison_inputstruct(is);
            exit(1);
        }
    }

    Dllist toprint;
    toprint = new_dllist();

    // Add all people to toprint
    jrb_traverse(tmp, t) {
        p = (Person *)tmp->val.v;
        dll_append(toprint, new_jval_v((void *)p));
    }

    while (!dll_empty(toprint)) {
        // Get next person at top of toprint
        p = (Person *)dll_first(toprint)->val.v;
        
        // Take p off top
        dll_delete_node(dll_first(toprint));

        if (!p->printed) {
            // Only print person if both parents have been printed
            if (person_parents_printed(t, p)) {
                person_print_info(p);
                p->printed = 1;

                // Add all children to list
                dll_traverse(dtmp, p->children) {
                    Person *child = (Person *)dtmp->val.v;
                    dll_append(toprint, new_jval_v((void *)child));
                }
            }
        }
    }

    // Free up all memory
    free_all_persons(t);
    jettison_inputstruct(is);
    free_dllist(toprint);

    exit(0);
}

Person* find_person(JRB tree, char* name) {
    if (name == NULL) return NULL;

    JRB node = jrb_find_str(tree, name);
    if (node == NULL) return NULL;

    return (Person *)node->val.v;
}

Person* insert_person(JRB tree, char* name) {
    Person *p = malloc(sizeof(Person));
    p->name = strdup(name);
    p->children = new_dllist();
    jrb_insert_str(tree, p->name, new_jval_v((void *)p));

    return p;
}

void free_all_persons(JRB tree) {
    JRB tmp;
    Person *p;

    // Free all persons and their respectable fields
    jrb_traverse(tmp, tree) {
        p = (Person *)tmp->val.v;
        free(p->name);

        if (p->father) free(p->father);
        if (p->mother) free(p->mother);
        free_dllist(p->children);

        free(p);
    }

    jrb_free_tree(tree);
}

void person_print_info(Person* p) {
    printf("%s\n", p->name);

    if (p->sex == 'M')      printf("  Sex: Male\n");
    else if (p->sex == 'F') printf("  Sex: Female\n");
    else                    printf("  Sex: Unknown\n");

    if (p->father)          printf("  Father: %s\n", p->father);
    else                    printf("  Father: Unknown\n");

    if (p->mother)          printf("  Mother: %s\n", p->mother);
    else                    printf("  Mother: Unknown\n");

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

int person_has_child(Person* p, char *child_name) {
    Dllist dtmp;
    dll_traverse(dtmp, p->children) {
        Person *child = (Person *)dtmp->val.v;
        if (strcmp(child->name, child_name) == 0) return 1;
    }

    return 0;
}

int person_set_sex(Person* p, char sex) {
    if (p->sex && p->sex != sex)
        return 1;

    p->sex = sex;
    return 0;
}

int person_set_parent(Person* child, Person* parent, int father) {
    if (parent->sex && parent->sex != (father ? 'M' : 'F')) {
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

void person_add_child(Person* parent, Person* child) {
    if (!person_has_child(parent, child->name))
        dll_append(parent->children, new_jval_v((void *)child));
}

int person_parents_printed(JRB tree, Person* p) {
    if (p->mother == NULL && p->father == NULL) return 1;

    Person *mother = find_person(tree, p->mother);
    Person *father = find_person(tree, p->father);

    if (mother && !mother->printed) return 0;
    if (father && !father->printed) return 0;

    return 1;
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