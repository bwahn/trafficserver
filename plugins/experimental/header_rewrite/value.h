//////////////////////////////////////////////////////////////////////////////////////////////
// Public interface for creating all values.
// 
//
#ifndef __VALUE_H__
#define __VALUE_H__ 1

#define UNUSED __attribute__ ((unused))
static char UNUSED rcsId__value_h[] = "@(#) $Id$ built on " __DATE__ " " __TIME__;

#include <string>
#include <ts/ts.h>

#include "resources.h"
#include "statement.h"
#include "condition.h"
#include "factory.h"
#include "parser.h"


///////////////////////////////////////////////////////////////////////////////
// Base class for all Values (this is also the interface).
//
// TODO: This is very incomplete, we need to support linked lists of these,
// which evaluate each component and create a "joined" final string.
//
class Value : Statement
{
public:
  Value()
    : _value(""), _int_value(-1), _cond_val(NULL)
  {
    TSDebug(PLUGIN_NAME_DBG, "Calling CTOR for Value");
  };

  void
  set_value(const std::string& val)
  {
    _value = val;
    if (_value.substr(0,2) == "%{") {
      Parser parser(_value);

      _cond_val = condition_factory(parser.get_op());
      if (_cond_val) {
        _cond_val->initialize(parser);
      }
    }
    _int_value = strtol(_value.c_str(), NULL, 10);
  }

  void
  append_value(std::string& s, const Resources& res) const {
    if (_cond_val) {
      _cond_val->append_value(s, res);
    } else {
      s += _value;
    }
  }

  const std::string& get_value() const { return _value; }
  int get_int_value() const { return _int_value; }

  bool empty() const { return _value.empty(); }

private:
  DISALLOW_COPY_AND_ASSIGN(Value);

  std::string _value;
  int _int_value;
  Condition* _cond_val;
};


#endif // __VALUE_H
