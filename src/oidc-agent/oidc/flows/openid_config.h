#ifndef OPENID_CONFIG_H
#define OPENID_CONFIG_H

#include "account/account.h"
#include "utils/oidc_error.h"

oidc_error_t getIssuerConfig(struct oidc_account* account);

#endif
