//////////////////////////////////////////////////////////////////////////////////////////////
// 
// Base class for all Conditions and Operations. We share the "linked" list, and the
// resource management / requirements.
//
#ifndef __STATEMENT_H__
#define __STATEMENT_H__ 1

#define UNUSED __attribute__ ((unused))
static char UNUSED rcsId__statement_h[] = "@(#) $Id$ built on " __DATE__ " " __TIME__;

#include <string>
#include <vector>
#include <ts/ts.h>

#include "resources.h"
#include "parser.h"
#include "lulu.h"


// URL data (both client and server)
enum UrlQualifiers {
  URL_QUAL_NONE,
  URL_QUAL_HOST,
  URL_QUAL_PORT,
  URL_QUAL_PATH,
  URL_QUAL_QUERY,
  URL_QUAL_MATRIX,
  URL_QUAL_SCHEME,
  URL_QUAL_URL
};


class Statement
{
 public:
  Statement()
    : _next(NULL), _pdata(NULL), _rsrc(RSRC_NONE), _initialized(false), _hook(TS_HTTP_READ_RESPONSE_HDR_HOOK)
  {
    TSDebug(PLUGIN_NAME_DBG, "Calling CTOR for Statement");
  }

  virtual ~Statement() {
    TSDebug(PLUGIN_NAME_DBG, "Calling DTOR for Statement");
    free_pdata();
  }

  // Private data
  void set_pdata(void* pdata) { _pdata = pdata; }
  void* get_pdata() const { return(_pdata); }
  virtual void free_pdata() { TSfree(_pdata); _pdata = NULL; }

  // Which hook are we adding this statement to?
  bool set_hook(TSHttpHookID hook);
  const TSHttpHookID get_hook() const { return _hook; }

  // Which hooks are this "statement" applicable for? Used during parsing only.
  void add_allowed_hook(const TSHttpHookID hook) { _allowed_hooks.push_back(hook); }

  // Linked list.
  void append(Statement* stmt);

  const ResourceIDs get_resource_ids() const;

  virtual void initialize(Parser& p) {
    TSReleaseAssert(_initialized == false);
    initialize_hooks();
    _initialized = true;
  }
  bool initialized() const { return _initialized; }

 protected:
  virtual void initialize_hooks();

  UrlQualifiers parse_url_qualifier(const std::string& q);
  void require_resources(const ResourceIDs ids) { _rsrc = static_cast<ResourceIDs>(_rsrc | ids); }

  Statement* _next; // Linked list

private:
  DISALLOW_COPY_AND_ASSIGN(Statement);

  void* _pdata;
  ResourceIDs _rsrc;
  bool _initialized;
  std::vector<TSHttpHookID> _allowed_hooks;
  TSHttpHookID _hook;
};


#endif // __STATEMENT_H

