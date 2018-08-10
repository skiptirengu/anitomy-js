'use strict'

const anitomy = require('./build/Release/anitomy-js')

/**
 * @param {array|string} input 
 * @param {object} options 
 * @returns {Promise<object>}
 */
function parseAsync (input, options) {
  return new Promise((resolve, reject) => {
    try {
      anitomy.parseAsync(input, resolve, options)
    } catch (err) {
      reject(err)
    }
  })
}

/**
 * @param {array|string} input 
 * @param {object} options 
 * @returns {array|object}
 */
function parseSync (input, options) {
  return anitomy.parseSync(input, options)
}

module.export = {
  parseAsync,
  parseSync
}