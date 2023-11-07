/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
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
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value)
{
  if (tree != NULL)
  {
    if (tree->key == key)
    {
      *value = tree->value;
      return true;
    }
    return false || bst_search(tree->left, key, value) || bst_search(tree->right, key, value);
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
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value)
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
        bst_insert(&((*tree)->left), key, value);
      // if next is empty insert
      else
      {
        bst_node_t *tmp = malloc(sizeof(bst_node_t));
        tmp->key = key;
        tmp->value = value;
        (*tree)->left = NULL;
      }
    }
    // key is bigger go right
    else if ((*tree)->right != NULL)
    {
      bst_insert(&((*tree)->right), key, value);
    }
    // if next is empty insert
    else
    {
      bst_node_t *tmp = malloc(sizeof(bst_node_t));
      tmp->key = key;
      tmp->value = value;
      (*tree)->right = tmp;
    }
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
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
    bst_replace_by_rightmost(target, &((*tree)->right));
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
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key)
{
  if (tree != NULL)
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
        bst_delete(&((*tree)->left), key);
      else
        bst_delete(&((*tree)->right), key);
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
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree)
{
  if ((*tree))
  {
    bst_dispose(&(*tree)->left);
    bst_dispose(&(*tree)->right);
    free(*tree);
    //*tree = NULL;
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
  if (tree)
  {
    bst_add_node_to_items(tree, items);
    bst_preorder(tree->left, items);
    bst_preorder(tree->right, items);
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
  if (tree)
  {
    bst_preorder(tree->left, items);
    bst_add_node_to_items(tree, items);
    bst_preorder(tree->right, items);
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
  if (tree)
  {
    bst_preorder(tree->left, items);
    bst_preorder(tree->right, items);
    bst_add_node_to_items(tree, items);
  }
}
