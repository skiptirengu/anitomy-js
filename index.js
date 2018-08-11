'use strict'

var anitomy = require('./build/Release/anitomy-js')
var util = require('util')

/**
 * @param {array|string} input
 * @param {function} [callback]
 * @param {object} [options]
 * @returns {Promise<object>|undefined}
 */
function parseAsync (input, callback, options) {
  var args = arguments.length

  if (args === 2 && typeof callback === 'object') {
    options = callback
    callback = null
  } else if (args === 2) {
    options = {}
  } else if (args === 1) {
    options = {}
    callback = null
  }

  if (callback && callback.length === 1) {
    var newCallback = util.deprecate(
      callback,
      'Using a callback with only one argument is deprecated. Switch to using a node style callback (err, data) or use the promise api.', 'anitomy-js#parseAsync'
    )
    return anitomy.parseAsync(input, newCallback, options)
  }

  var promise = new Promise(function (resolve, reject) {
    try {
      anitomy.parseAsync(input, resolve, options)
    } catch (err) {
      reject(err)
    }
  })

  return promise
    .then(function (parsed) {
      if (callback) {
        callback(undefined, parsed)
      } else {
        return Promise.resolve(parsed)
      }
    })
    .catch(function (err) {
      if (callback) {
        callback(err)
      } else {
        throw err
      }
    })
}

/**
 * @param {array|string} input
 * @param {object} [options]
 * @returns {array|object}
 */
function parseSync (input, options) {
  options = options || {}
  return anitomy.parseSync(input, options)
}

module.exports = {
  parse: parseAsync,
  parseAsync,
  parseSync,
  anitomy
}
