/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu)
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 *
 */

#include "../btree.h"
#include "../iter/stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 *
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v
 * uložen ve stromu.
 *
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 *
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 *
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
 */
char convertChar(int c)
{
    if (c >= 97 && c <= 122)
        return (char)c;
    else if (c == ' ')
        return ' ';
    else
        return '_';
}
void letter_count(bst_node_t **tree, char *input)
{
    bst_init(tree);
    size_t len = strlen(input);
    int val = 0;
    for (size_t i = 0; i < len; i++)
    {
        char c = convertChar(tolower(input[i]));
        // if found in tree add one more into value in tree
        if (bst_search(*tree, c, &val))
            bst_insert(tree, c, val + 1);
        else
            bst_insert(tree, c, 1);
    }
    fprintf(stderr, "%s\n", input);
}

/**
 * Vyvážení stromu.
 *
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 *
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
 */
void inorderStack(bst_node_t **tree, stack_bst_t *stack)
{
    if (*tree)
    {
        inorderStack(&((*tree)->left), stack);
        stack_bst_push(stack, *tree);
        inorderStack(&((*tree)->left), stack);
    }
}
void buildTree(bst_node_t **tree, stack_bst_t *stack, int start, int end)
{
    *tree = stack->items[(start + end) / 2];
    buildTree(&((*tree)->right), stack, start, end / 2);
    buildTree(&((*tree)->left), stack, (start + end) / 2, end);
}
void bst_balance(bst_node_t **tree)
{
    stack_bst_t treeArr;
    stack_bst_init(&treeArr);
    inorderStack(tree, &treeArr);
    buildTree(tree, &treeArr, 0, treeArr.top);

}
