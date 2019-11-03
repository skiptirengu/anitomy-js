const anitomy = require('./build/Release/anitomy-js')
const { promisify } = require('util')

const parseAsync = (...args) => promisify(anitomy.parseAsync)(...args)
const parseSync = (...args) => anitomy.parseSync(...args)
const parse = parseAsync

module.exports = { parse, parseAsync, parseSync }
