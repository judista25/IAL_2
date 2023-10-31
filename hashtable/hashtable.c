/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key)
{
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++)
  {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table)
{
  for (int i = 0; i < MAX_HT_SIZE; i++)
  {
    (*table)[i] = NULL;
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key)
{
  int i = get_hash(key);
  ht_item_t *head = (*table)[i];
  while (head)
  {
    // key found
    if (!strcmp(key, head->key))
      return head;
    head = head->next;
  }

  return NULL;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value)
{
  ht_item_t *existing = ht_search(table, key);
  if (existing)
  {
    existing->value = value;
    return;
  }
  ht_item_t *tmp = malloc(sizeof(ht_item_t));
  tmp->key = key;
  tmp->value = value;
  int hash = get_hash(key);
  tmp->next = (*table)[hash];
  (*table)[hash] = tmp;
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key)
{
  ht_item_t *searched = ht_search(table, key);
  if (!searched)
    return NULL;

  return &(searched->value);
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key)
{
  int hash = get_hash(key);
  ht_item_t *head = (*table)[hash];
  ht_item_t *previous = head;
  while (head)
  {
    if (!strcmp(head->key, key))
    {
      previous->next = head->next;
      free(head->key);
      free(head);
      return;
    }
    previous = head;
    head = head->next;
  }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table)
{
  for (int i = 0; i < MAX_HT_SIZE; i++)
  {
    ht_item_t *head = (*table)[i];
    ht_item_t *tmp = head;
    while (head)
    {
      tmp = head;
      head = head->next;
      free(tmp->key);
      free(tmp);
    }
    (*table)[i] = NULL;
  }
}
