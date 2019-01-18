#ifndef ISSUER_HELPER_H
#define ISSUER_HELPER_H

#include "account.h"

#include "list/list.h"

list_t* getSuggestableIssuers();
char*   getFavIssuer(const struct oidc_account* account, list_t* suggastable);
void    printSuggestIssuer(list_t* suggastable);
void    printIssuerHelp(const char* url);
char* getUsableResponseTypes(const struct oidc_account* account, list_t* flows);
char* getUsableGrantTypes(const struct oidc_account* account, list_t* flows);
int   compIssuerUrls(const char* a, const char* b);

#endif  // ISSUER_HELPER_H
