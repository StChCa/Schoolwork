#!/usr/bin/python
import json
from bson import json_util
import bottle
from bottle import route, run, get, request, abort
import datetime
#import mongopython
#from mongopython import read_document

id = 0
# setup URI paths for REST service
@route('/hello', method='GET')
def get_greeting():
  global id
  id = id + 1
  try:
    request.query.name
    name=request.query.name
    if name:
      string="{ hello: \""+name+"\" }"
    else:
      return '<p>Error. Missing \"name\" argument.</p>'
  except NameError:
    abort(404, 'No Parameter for id %s' % id)

  if not string:
    abort(404, 'No id %s' % id)
  return json.loads(json.dumps(string, indent=4, default=json_util.default))

@route('/strings', method='POST')
def post_strings():
  global id
  id = id + 1
  try:
    raw = request.body.read()
    jsonData = json.loads(raw)
    return jsonData
  except NameError:
    abort(404,'Something went wrong')


if __name__ == '__main__':
  #app.run(debug=True)
  run(host='localhost', port=8080)