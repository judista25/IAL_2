/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h
 * a připravených koster funkcí implementujte binární vyhledávací
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci.
 */
void bst_init(bst_node_t **tree)
{
  *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value)
{
  while (tree)
  {
    if (tree->key < key)
      tree = tree->right;
    else if (tree->key > key)
      tree = tree->left;
    else
    {
      *value = tree->value;
      return true;
    }
  }
  return false;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value)
{
  while (*tree)
  {
    if ((*tree)->key == key)
    {
      (*tree)->value = value;
      return;
    }
    // key is smaller go left
    else if ((*tree)->key > key)
    {
      tree = &((*tree)->left);
    }
    else
    {
      tree = &((*tree)->right);
    }
    // if next is empty insert
  }
  bst_node_t *tmp = malloc(sizeof(struct bst_node));
  tmp->key = key;
  tmp->value = value;
  tmp->left = NULL;
  tmp->right = NULL;
  (*tree) = tmp;
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  while (*tree)
  {
    if ((*tree)->right == NULL)
    {
      target->key = (*tree)->key;
      target->value = (*tree)->value;
      // free the element
      if ((*tree)->left)
      {
        // if left tree exist conect it
        fprintf(stderr, "free shit in replace\n");
        bst_node_t *tmp = (*tree);
        (*tree) = (*tree)->left;
        free(tmp);
      }
      else
      {
        free((*tree));
        (*tree) = NULL;
      }
      return;
    }
    else
      tree = &((*tree)->right);
  }
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 *
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key)
{
  while (*tree)
  {
    if ((*tree)->key == key)
    {
      if ((*tree)->left)
      {
        // change value of current node for right most
        bst_replace_by_rightmost((*tree), &((*tree)->left));
      }
      else if ((*tree)->right)
      {
        bst_node_t *tmp = (*tree);
        (*tree) = (*tree)->right;
        free(tmp);
      }
      else
      {
        // leaf
        free(*tree);
        (*tree) = NULL;
      }
      return;
    }
    else if ((*tree)->key > key)
      tree = &((*tree)->left);
    else
      tree = &((*tree)->right);
  }
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree)
{
  if (!(*tree))
    return;

  stack_bst_t stack;
  stack_bst_init(&stack);
  stack_bst_push(&stack, *tree);

  while (stack.top >= 0)
  {
    bst_node_t *head = stack_bst_top(&stack);
    stack_bst_pop(&stack);

    if (head->left)
      stack_bst_push(&stack, head->left);
    if (head->right)
      stack_bst_push(&stack, head->right);

    free(head);
  }
  // init tree
  *tree = NULL;
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items)
{
  while (tree)
  {
    bst_add_node_to_items(tree, items);
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
  if (!tree)
    return;
  stack_bst_t stack;
  stack_bst_init(&stack);
  // fill stack with left most
  bst_leftmost_preorder(tree, &stack, items);

  while (!stack_bst_empty(&stack))
  {
    bst_node_t *current = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    //bst_add_node_to_items(current, items);

    // If right subtree exist
    if (current->right)
      bst_leftmost_preorder(current->right, &stack, items);
  }
}

/*
 * Pomocná funkce pro iterativní inorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{
  while (tree)
  {
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
  if (!tree)
    return;
  stack_bst_t stack;
  stack_bst_init(&stack);
  // fill stack with left most
  bst_leftmost_inorder(tree, &stack);

  while (!stack_bst_empty(&stack))
  {
    bst_node_t *current = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    bst_add_node_to_items(current, items);

    // If right subtree exist
    if (current->right)
      bst_leftmost_inorder(current->right, &stack);
  }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit)
{
  while (tree)
  {
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
    tree = (tree)->left;
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
  bool fromLeft = false;
  stack_bst_t s1;
  stack_bool_t s2;
  stack_bst_init(&s1);
  stack_bool_init(&s2);
  bst_leftmost_postorder(tree, &s1, &s2);
  while (stack_bst_empty(&s1))
  {
    tree = stack_bst_top(&s1);
    fromLeft = stack_bool_top(&s2);
    stack_bool_pop(&s2);
    if (fromLeft)
    {
      stack_bool_push(&s2, false);
      bst_leftmost_postorder(tree, &s1, &s2);
    }
    else
    {
      stack_bst_pop(&s1);
      fprintf(stderr,"[%c,%d] ",tree->key,tree->value);
      bst_add_node_to_items(tree, items);
    }
  }
  fprintf(stderr,"\n");
}
