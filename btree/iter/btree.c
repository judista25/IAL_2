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
  while (true)
  {
    if ((*tree) != NULL)
    {
      if ((*tree)->key == key)
      {
        (*tree)->value = value;
        return;
      }
      // key is smaller go left
      else if ((*tree)->key > key)
      {
        if ((*tree)->left != NULL)
          (*tree) = (*tree)->left;
        // if next is empty insert
        else
        {
          bst_node_t *tmp = malloc(sizeof(bst_node_t));
          tmp->key = key;
          tmp->value = value;
          (*tree)->left = NULL;
          break;
        }
      }
      // key is bigger go right
      else if ((*tree)->right != NULL)
      {
        (*tree) = (*tree)->left;
      }
      // if next is empty insert
      else
      {
        bst_node_t *tmp = malloc(sizeof(bst_node_t));
        tmp->key = key;
        tmp->value = value;
        (*tree)->right = tmp;
        break;
      }
    }
    else
      break;
  }
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
    if ((*tree)->right->right == NULL)
    {
      target->key = (*tree)->right->key;
      target->value = (*tree)->right->value;
      // free the element
      free((*tree)->right);
      (*tree)->right = NULL;
    }
    else
    {
      (*tree) = (*tree)->right;
    }
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
        bst_node_t *tmp = malloc(sizeof(bst_node_t));
        bst_replace_by_rightmost(tmp, &((*tree)->left));
        (*tree) = tmp;
        return;
      }
      else if ((*tree)->right)
      {
        // no left tree
        free((*tree));
        (*tree) = (*tree)->right;
        return;
      }
      // no tree on the left or right so just free
      else
      {
        free((*tree));
        return;
      }
    }
    else
    {
      // if right exist look if fits
      if ((*tree)->right)
      {
        if ((*tree)->right->key == key)
        {
          if ((*tree)->right->left)
          {
            bst_node_t *tmp = malloc(sizeof(bst_node_t));
            bst_replace_by_rightmost(tmp, &((*tree)->right->left));
            (*tree)->right = tmp;
            return;
          }
          else if ((*tree)->right->right)
          {
            // no left tree
            free((*tree)->right);
            (*tree)->right = (*tree)->right->right;
            return;
          }
          // no tree on the left or right so just free
          else
          {
            free((*tree)->right);
            return;
          }
        }
      }
      // if left exist look if fits
      if ((*tree)->left)
      {
        if ((*tree)->left->key == key)
        {
          if ((*tree)->left->left)
          {
            bst_node_t *tmp = malloc(sizeof(bst_node_t));
            bst_replace_by_rightmost(tmp, &((*tree)->left->left));
            (*tree)->left = tmp;
            return;
          }
          else if ((*tree)->left->right)
          {
            // no left tree
            free((*tree)->left);
            (*tree)->left = (*tree)->left->right;
            return;
          }
          // no tree on the left or right so just free
          else
          {
            free((*tree)->left);
            return;
          }
        }
      }
      if ((*tree)->key < key)
        (*tree) = (*tree)->left;
      else
        (*tree) = (*tree)->right;
    }
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
  if (!*tree)
    return;
  stack_bst_t stack;
  stack_bst_init(&stack);
  int i = 0;
  stack_bst_push(&stack, *tree);
  while (i <= stack.top)
  {
    if (stack.items[i]->left)
      stack_bst_push(&stack, stack.items[i]->left);
    if (stack.items[i]->right)
      stack_bst_push(&stack, stack.items[i]->right);
    i++;
  }
  for (; i >= 0; i--)
  {
    free(stack.items[i]);
    // stack_bst_pop(&stack);
  }
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
    bst_add_node_to_items(current, items);

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
}
