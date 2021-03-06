-- Example Lua remap plugin. Load this with the following remap.comfig line:
--
-- map http://test.foo.com http://foo.foo.com @plugin=lua.so @pparam=/path/to/example.lua

-- Pull in the Traffic Server API.
local TS = require 'ts'

require 'string'
require 'math'

-- Compulsory remap hook. We are given a request object that we can modify if necessary.
function remap(request)
  -- Get a copy of the current URL.
  url = request:url()

  TS.debug('example', string.format('remapping %s://%s', url.scheme, url.host))

  -- Do some header manipulation, just to mess with the origin.
  request.headers['added-bool'] = true
  request.headers['added-int'] = 1
  request.headers['added-string'] = 'ttt'
  request.headers['added-table'] = {}
  request.headers['deleted'] = nil

  -- We can also print to stdout using Lua standard library.
  print(string.format('request URL is %s://%s:%d/%s',
        url.scheme, url.host, url.port, url.path and url.path or ''))

  -- Modify components of the URL ... everybody loves slashdot.
  url.host = 'www.slashdot.org'
  url.port = 80
  url.method = 'POST'

  -- Plugin chain evaluation rules:
  --    redirect: plugin chain terminates
  --    reject: plugin chain terminates
  --    rewrite: plugin chain continues

  chance = math.random(4)
  if chance == 1 then
    -- Send a 301 redirect to the new URL.
    request:redirect(url)
  elseif chance == 2 then
    -- Reject the request with an optional message.
    request:reject(400, "Denied")
  elseif chance == 3 then
    -- Reject the request with a response body. We sniff the body to set the content type.
    request:reject(500, [[
      <HEAD></TITLE></HEAD>
      <BODY>Internal error, sorry</BODY>
      ]])
  else
    -- Rewrite the request URL. The remap plugin chain continues and other plugins
    request:rewrite(url)
  end

end
