#include "registration.h"

#include "account/account.h"
#include "account/issuer_helper.h"
#include "defines/agent_values.h"
#include "defines/oidc_values.h"
#include "defines/settings.h"
#include "oidc-agent/http/http_ipc.h"
#include "utils/json.h"
#include "utils/listUtils.h"
#include "utils/logger.h"
#include "utils/portUtils.h"
#include "utils/stringUtils.h"

char* generateRedirectUris() {
  char* redirect_uri0 = portToUri(HTTP_DEFAULT_PORT);
  char* redirect_uri1 = portToUri(getRandomPort());
  char* redirect_uri2 = portToUri(HTTP_FALLBACK_PORT);
  char* redirect_uri3 = oidc_strcat(AGENT_CUSTOM_SCHEME, "redirect");

  cJSON* json = generateJSONArray(redirect_uri0, redirect_uri1, redirect_uri2,
                                  redirect_uri3, NULL);
  secFree(redirect_uri0);
  secFree(redirect_uri1);
  secFree(redirect_uri2);
  secFree(redirect_uri3);
  char* uris = jsonToStringUnformatted(json);
  secFreeJson(json);
  return uris;
}

char* getRegistrationPostData(const struct oidc_account* account,
                              list_t*                    flows) {
  char* client_name    = account_getClientName(account);
  char* response_types = getUsableResponseTypes(account, flows);
  char* grant_types    = getUsableGrantTypes(account, flows);
  char* redirect_uris_json =
      account_getRedirectUris(account)
          ? listToJSONArrayString(account_getRedirectUris(account))
          : generateRedirectUris();
  cJSON* json = generateJSONObject(
      OIDC_KEY_APPLICATIONTYPE, cJSON_String, OIDC_APPLICATIONTYPES_WEB,
      OIDC_KEY_CLIENTNAME, cJSON_String, client_name, OIDC_KEY_RESPONSETYPES,
      cJSON_Array, response_types, OIDC_KEY_GRANTTYPES, cJSON_Array,
      grant_types, OIDC_KEY_SCOPE, cJSON_String, account_getScope(account),
      OIDC_KEY_REDIRECTURIS, cJSON_Array, redirect_uris_json, NULL);
  secFree(response_types);
  secFree(grant_types);
  secFree(redirect_uris_json);
  char* json_str = jsonToStringUnformatted(json);
  secFreeJson(json);
  return json_str;
}

char* dynamicRegistration(struct oidc_account* account, list_t* flows,
                          const char* access_token) {
  logger(DEBUG, "Performing dynamic Registration flow");
  if (!strValid(account_getRegistrationEndpoint(account))) {
    oidc_errno = OIDC_ENOSUPREG;
    return NULL;
  }
  char* body = getRegistrationPostData(account, flows);
  if (body == NULL) {
    return NULL;
  }
  logger(DEBUG, "Data to send: %s", body);
  struct curl_slist* headers =
      curl_slist_append(NULL, HTTP_HEADER_CONTENTTYPE_JSON);
  if (strValid(access_token)) {
    char* auth_header =
        oidc_sprintf(HTTP_HEADER_AUTHORIZATION_BEARER_FMT, access_token);
    headers = curl_slist_append(headers, auth_header);
    secFree(auth_header);
  }
  char* res =
      httpsPOST(account_getRegistrationEndpoint(account), body, headers,
                account_getCertPath(account), account_getClientId(account),
                account_getClientSecret(account));
  curl_slist_free_all(headers);
  secFree(body);
  if (res == NULL) {
    return NULL;
  }
  return res;
}
